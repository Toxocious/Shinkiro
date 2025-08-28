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

#include <Log/Log.h>

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace Shinkiro::Renderer
{
    unsigned int TextureFromFile( const char * path, const std::string & directory, bool gamma = false );

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
        std::vector<Texture> textures_loaded;
        std::vector<Mesh>    meshes;
        bool                 gammaCorrection;

        std::string   name;
        std::string   directory;
        ModelCategory category;

        Model( const std::string & name, std::string const & path, ModelCategory category, std::string const & textureDirectory = "", bool gamma = false )
            : name( name ), category( category ), gammaCorrection( gamma )
        {
            SHNK_CORE_INFO( "Loading model: {} from {}", name, path );

            loadModel( path, textureDirectory );
            s_modelCount++;
        }

        ~Model()
        {
            s_modelCount--;
        }

        void Draw( Shader & shader );
        void DrawTransparent( Shader & shader, const glm::vec3 & cameraPosition, const glm::mat4 & model );

    public:
        static int GetModelCount()
        {
            return s_modelCount;
        }

    public:
        std::string path;

    private:
        void                 loadModel( std::string const & path, std::string const & textureDirectory );
        void                 processNode( aiNode * node, const aiScene * scene );
        Mesh                 processMesh( aiMesh * mesh, const aiScene * scene );
        std::vector<Texture> loadMaterialTextures( aiMaterial * mat, aiTextureType type, std::string typeName );

    private:
        static int s_modelCount;
    };
}

#endif
