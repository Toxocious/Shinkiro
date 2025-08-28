#pragma once
#ifndef SHINKIRO_RENDERER_MESH_H
#    define SHINKIRO_RENDERER_MESH_H

#    include <glad/glad.h>
#    include <glm/glm.hpp>
#    include <glm/gtc/matrix_transform.hpp>

#    include <Renderer/Util/Shader.h>

#    include <string>
#    include <vector>

namespace Shinkiro::Renderer
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    struct Texture
    {
        unsigned int id;
        std::string  type;
        std::string  path;
    };

    class Mesh
    {
    public:
        Mesh( std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures )
            : m_Vertices( vertices ), m_Indices( indices ), m_Textures( textures )
        {
            SetupMesh();
        }

    public:
        void Draw( Shader & shader );

    private:
        void SetupMesh();

    public:
        std::vector<Vertex>       m_Vertices;
        std::vector<unsigned int> m_Indices;
        std::vector<Texture>      m_Textures;
        glm::vec3                 m_Center;

        unsigned int m_NumVertices   = 0;
        unsigned int m_NumTriangles  = 0;
        int          m_MaterialIndex = -1;

        bool m_HasUVSet0       = false;
        bool m_HasUVSet1       = false;
        bool m_HasVertexColors = false;

    private:
        unsigned int m_VAO, m_VBO, m_EBO;
    };
}

#endif
