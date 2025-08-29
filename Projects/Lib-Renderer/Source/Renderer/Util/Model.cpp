#include <Renderer/_Common.h>

#include <Renderer/Util/Model.h>

namespace Shinkiro::Renderer
{
    int Model::s_modelCount = 0;

    /**
     * @brief Render the model by drawing each mesh.
     * @param shader The shader to use for rendering.
     */
    void Model::Draw( Shader & shader )
    {
        for ( unsigned int i = 0; i < meshes.size(); i++ )
        {
            meshes[i].Draw( shader );
        }
    }

    /**
     * @brief Render the model's transparent meshes, sorted back to front.
     * @param shader The shader to use for rendering.
     * @param cameraPosition The position of the camera for sorting.
     * @param model The model matrix of the object being rendered.
     */
    void Model::DrawTransparent( Shader & shader, const glm::vec3 & cameraPosition, const glm::mat4 & model )
    {
        // Store original state
        GLboolean last_enable_blend     = glIsEnabled( GL_BLEND );
        GLboolean last_enable_cull_face = glIsEnabled( GL_CULL_FACE );
        GLboolean last_depth_mask;
        glGetBooleanv( GL_DEPTH_WRITEMASK, &last_depth_mask );

        // Set state for transparent drawing
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glDisable( GL_CULL_FACE );
        glDepthMask( GL_FALSE );

        // Sort meshes from back to front
        std::sort( meshes.begin(), meshes.end(), [&]( const Mesh & a, const Mesh & b )
                   {
                       glm::vec3 worldPosA = glm::vec3( model * glm::vec4( a.m_Center, 1.0f ) );
                       glm::vec3 worldPosB = glm::vec3( model * glm::vec4( b.m_Center, 1.0f ) );
                       float     distA     = glm::distance( cameraPosition, worldPosA );
                       float     distB     = glm::distance( cameraPosition, worldPosB );
                       return distA > distB;
                   } );

        for ( unsigned int i = 0; i < meshes.size(); i++ )
        {
            meshes[i].Draw( shader );
        }

        // Restore original state
        glDepthMask( last_depth_mask );
        if ( last_enable_cull_face )
        {
            glEnable( GL_CULL_FACE );
        }
        if ( !last_enable_blend )
        {
            glDisable( GL_BLEND );
        }
    }

    /**
     * @brief Load a model from file using Assimp.
     * @param path The file path to the model.
     * @param textureDirectory Optional directory to load textures from. If empty, uses model's directory.
     */
    void Model::loadModel( std::string const & path, std::string const & textureDirectory )
    {
        this->path = path;

        Assimp::Importer importer;
        const aiScene *  scene = importer.ReadFile( path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace );

        if ( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
        {
            SHNK_CORE_ERROR( "ERROR::ASSIMP:: {}: {}", path, importer.GetErrorString() );
            return;
        }

        // If a texture directory is provided, use it. Otherwise, derive from model path.
        if ( !textureDirectory.empty() )
        {
            this->directory = textureDirectory;
        }
        else
        {
            this->directory = path.substr( 0, path.find_last_of( '/' ) );
        }

        processNode( scene->mRootNode, scene );

        SHNK_CORE_INFO( "Model loaded: {} with {} meshes from directory '{}'.", path, meshes.size(), this->directory );
    }

    /**
     * @brief Recursively process a node in the Assimp scene graph.
     * @param node The current node to process.
     * @param scene The Assimp scene containing the node.
     */
    void Model::processNode( aiNode * node, const aiScene * scene )
    {
        for ( unsigned int i = 0; i < node->mNumMeshes; i++ )
        {
            aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back( processMesh( mesh, scene ) );
        }

        for ( unsigned int i = 0; i < node->mNumChildren; i++ )
        {
            processNode( node->mChildren[i], scene );
        }
    }

    /**
     * @brief Process an Assimp mesh into our Mesh structure.
     * @param mesh The Assimp mesh to process.
     * @param scene The Assimp scene containing the mesh.
     * @return The processed Mesh.
     */
    Mesh Model::processMesh( aiMesh * mesh, const aiScene * scene )
    {
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;
        glm::vec3                 center_sum( 0.0f );

        for ( unsigned int i = 0; i < mesh->mNumVertices; i++ )
        {
            Vertex vertex;
            vertex.Position = glm::vec3( mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z );
            if ( mesh->HasNormals() )
            {
                vertex.Normal = glm::vec3( mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z );
            }
            if ( mesh->mTextureCoords[0] )
            {
                vertex.TexCoords = glm::vec2( mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y );
            }
            else
            {
                vertex.TexCoords = glm::vec2( 0.0f, 0.0f );
            }
            vertices.push_back( vertex );
            center_sum += vertex.Position;
        }

        for ( unsigned int i = 0; i < mesh->mNumFaces; i++ )
        {
            aiFace face = mesh->mFaces[i];
            for ( unsigned int j = 0; j < face.mNumIndices; j++ )
            {
                indices.push_back( face.mIndices[j] );
            }
        }

        aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = loadMaterialTextures( material, aiTextureType_DIFFUSE, "texture_diffuse" );
        textures.insert( textures.end(), diffuseMaps.begin(), diffuseMaps.end() );

        std::vector<Texture> specularMaps = loadMaterialTextures( material, aiTextureType_SPECULAR, "texture_specular" );
        textures.insert( textures.end(), specularMaps.begin(), specularMaps.end() );

        Mesh resultMesh( vertices, indices, textures );
        if ( mesh->mNumVertices > 0 )
        {
            resultMesh.m_Center = center_sum / ( float ) mesh->mNumVertices;
        }
        else
        {
            resultMesh.m_Center = glm::vec3( 0.0f );
        }

        return resultMesh;
    }

    /**
     * @brief Load material textures of a specific type.
     * @param mat The material to load textures from.
     * @param type The type of texture to load (e.g., diffuse, specular
     * etc.).
     * @param typeName The name to assign to the texture type.
     * @return A vector of loaded textures.
     */
    std::vector<Texture> Model::loadMaterialTextures( aiMaterial * mat, aiTextureType type, std::string typeName )
    {
        std::vector<Texture> textures;
        for ( unsigned int i = 0; i < mat->GetTextureCount( type ); i++ )
        {
            aiString str;
            mat->GetTexture( type, i, &str );
            bool skip = false;
            for ( unsigned int j = 0; j < textures_loaded.size(); j++ )
            {
                if ( std::strcmp( textures_loaded[j].path.data(), str.C_Str() ) == 0 )
                {
                    textures.push_back( textures_loaded[j] );
                    skip = true;
                    break;
                }
            }
            if ( !skip )
            {
                Texture texture;
                texture.id   = TextureFromFile( str.C_Str(), this->directory );
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back( texture );
                textures_loaded.push_back( texture );
            }
        }
        return textures;
    }

    /**
     * @brief Load a texture from file and generate an OpenGL texture ID.
     * @param path The file path to the texture.
     * @param directory The directory to load the texture from.
     * @param gamma Whether to apply gamma correction.
     * @return The OpenGL texture ID.
     */
    unsigned int TextureFromFile( const char * path, const std::string & directory, bool gamma )
    {
        std::string filename = std::string( path );

        // Find the last path separator (either '/' or '\') and take the substring after it.
        size_t last_slash_idx = filename.find_last_of( "/\\" );
        if ( std::string::npos != last_slash_idx )
        {
            filename.erase( 0, last_slash_idx + 1 );
        }

        // Prepend the correct directory to the filename.
        std::string full_path = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures( 1, &textureID );

        int             width, height, nrComponents;
        unsigned char * data = stbi_load( full_path.c_str(), &width, &height, &nrComponents, 0 );
        if ( data )
        {
            GLenum format;
            if ( nrComponents == 1 )
            {
                format = GL_RED;
            }
            else if ( nrComponents == 3 )
            {
                format = GL_RGB;
            }
            else if ( nrComponents == 4 )
            {
                format = GL_RGBA;
            }

            glBindTexture( GL_TEXTURE_2D, textureID );
            glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data );
            glGenerateMipmap( GL_TEXTURE_2D );

            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

            stbi_image_free( data );
            SHNK_CORE_INFO( "Loaded texture: {}", full_path );
        }
        else
        {
            SHNK_CORE_ERROR( "Texture failed to load at path: {}", full_path );
        }

        return textureID;
    }
}
