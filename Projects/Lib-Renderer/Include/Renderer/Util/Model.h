#ifndef SHINKIRO_RENDERER_MODEL_H
#define SHINKIRO_RENDERER_MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image/stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <Renderer/Util/Mesh.h>
#include <Renderer/Util/Shader.h>

#include <Core/Util/Log.h>

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace Shinkiro::Renderer
{
    enum ModelCategory
    {
        POKEMON,

        GRASS,
        ROCK,
        TREE,
    };

    class Model
    {
    public:
        Model( const std::string & name, std::string const & path, ModelCategory category, std::string const & textureDirectory = "", bool gamma = false )
            : m_Name( name ), m_Category( category ), m_GammaCorrection( gamma )
        {
            SHNK_CORE_INFO( "Loading model: {} from {}", name, path );

            LoadModel( path, textureDirectory );
            s_ModelCount++;
        }

        ~Model()
        {
            s_ModelCount--;
        }

        /**
         * @brief Render the model by drawing each mesh.
         * @param shader The shader to use for rendering.
         */
        void Draw( Shader & shader );

        /**
         * @brief Render the model's transparent meshes, sorted back to front.
         * @param shader The shader to use for rendering.
         * @param cameraPosition The position of the camera for sorting.
         * @param model The model matrix of the object being rendered.
         */
        void DrawTransparent( Shader & shader, const glm::vec3 & cameraPosition, const glm::mat4 & model );

    public:
        static int GetModelCount()
        {
            return s_ModelCount;
        }

    private:
        /**
         * @brief Load a model from file using Assimp.
         * @param path The file path to the model.
         * @param textureDirectory Optional directory to load textures from. If empty, uses model's directory.
         */
        void LoadModel( std::string const & path, std::string const & textureDirectory );

        /**
         * @brief Recursively process a node in the Assimp scene graph.
         * @param node The current node to process.
         * @param scene The Assimp scene containing the node.
         */
        void ProcessNode( aiNode * node, const aiScene * scene );

        /**
         * @brief Process an Assimp mesh into our Mesh structure.
         * @param mesh The Assimp mesh to process.
         * @param scene The Assimp scene containing the mesh.
         * @return The processed Mesh.
         */
        Mesh ProcessMesh( aiMesh * mesh, const aiScene * scene );

        /**
         * @brief Load material textures of a specific type.
         * @param mat The material to load textures from.
         * @param type The type of texture to load (e.g., diffuse, specular
         * etc.).
         * @param typeName The name to assign to the texture type.
         * @return A vector of loaded textures.
         */
        std::vector<Texture> LoadMaterialTextures( aiMaterial * mat, aiTextureType type, std::string typeName );

        /**
         * @brief Load a texture from file and generate an OpenGL texture ID.
         * @param path The file path to the texture.
         * @param directory The directory to load the texture from.
         * @param gamma Whether to apply gamma correction.
         * @return The OpenGL texture ID.
         */
        unsigned int TextureFromFile( const char * path, const std::string & directory, bool gamma = false );

    public:
        std::string m_Path;

        std::vector<Texture> m_TexturesLoaded;
        std::vector<Mesh>    m_Meshes;
        bool                 m_GammaCorrection;

        std::string   m_Name;
        std::string   m_Directory;
        ModelCategory m_Category;

    private:
        static int s_ModelCount;
    };
}

#endif
