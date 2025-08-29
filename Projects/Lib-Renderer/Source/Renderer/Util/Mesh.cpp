#include <Renderer/_Common.h>

#include <Renderer/Util/Mesh.h>

namespace Shinkiro::Renderer
{
    /**
     * @brief Render the mesh.
     * @param shader The shader to use for rendering.
     */
    void Mesh::Draw( Shader & shader )
    {
        unsigned int diffuseNr  = 1;
        unsigned int specularNr = 1;
        for ( unsigned int i = 0; i < m_Textures.size(); i++ )
        {
            glActiveTexture( GL_TEXTURE0 + i );
            std::string number;
            std::string name = m_Textures[i].type;
            if ( name == "texture_diffuse" )
            {
                number = std::to_string( diffuseNr++ );
            }
            else if ( name == "texture_specular" )
            {
                number = std::to_string( specularNr++ );
            }

            shader.setInt( ( "material." + name + number ).c_str(), i );
            glBindTexture( GL_TEXTURE_2D, m_Textures[i].id );
        }

        glBindVertexArray( m_VAO );
        glDrawElements( GL_TRIANGLES, static_cast<unsigned int>( m_Indices.size() ), GL_UNSIGNED_INT, 0 );
        glBindVertexArray( 0 );

        glActiveTexture( GL_TEXTURE0 );
    }

    /**
     * @brief Initializes all the buffer objects/arrays.
     */
    void Mesh::SetupMesh()
    {
        glGenVertexArrays( 1, &m_VAO );
        glGenBuffers( 1, &m_VBO );
        glGenBuffers( 1, &m_EBO );

        glBindVertexArray( m_VAO );
        glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
        glBufferData( GL_ARRAY_BUFFER, m_Vertices.size() * sizeof( Vertex ), &m_Vertices[0], GL_STATIC_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_EBO );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof( unsigned int ), &m_Indices[0], GL_STATIC_DRAW );

        // Vertex Positions
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void * ) 0 );
        // Vertex Normals
        glEnableVertexAttribArray( 1 );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void * ) offsetof( Vertex, Normal ) );
        // Vertex Texture Coords
        glEnableVertexAttribArray( 2 );
        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void * ) offsetof( Vertex, TexCoords ) );

        glBindVertexArray( 0 );
    }
}
