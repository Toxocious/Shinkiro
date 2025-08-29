#ifndef SHINKIRO_RENDERER_SHADER_H
#define SHINKIRO_RENDERER_SHADER_H

#include <Log/Log.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace Shinkiro::Renderer
{
    class Shader
    {
    public:
        Shader( const std::string & vertexPath, const std::string & fragmentPath );

    public:
        void use() const
        {
            glUseProgram( m_ID );
        }

    public:
        unsigned int getID() const
        {
            return m_ID;
        }

    public:
        void setBool( const std::string & name, bool value ) const
        {
            glUniform1i( getUniformLocation( name ), ( int ) value );
        }

        void setInt( const std::string & name, int value ) const
        {
            glUniform1i( getUniformLocation( name ), value );
        }

        void setFloat( const std::string & name, float value ) const
        {
            glUniform1f( getUniformLocation( name ), value );
        }

        void setVec2( const std::string & name, const glm::vec2 & value ) const
        {
            glUniform2fv( getUniformLocation( name ), 1, &value[0] );
        }

        void setVec3( const std::string & name, const glm::vec3 & value ) const
        {
            glUniform3fv( getUniformLocation( name ), 1, &value[0] );
        }

        void setVec4( const std::string & name, const glm::vec4 & value ) const
        {
            glUniform4fv( getUniformLocation( name ), 1, &value[0] );
        }

        void setMat2( const std::string & name, const glm::mat2 & mat ) const
        {
            glUniformMatrix2fv( getUniformLocation( name ), 1, GL_FALSE, &mat[0][0] );
        }

        void setMat3( const std::string & name, const glm::mat3 & mat ) const
        {
            glUniformMatrix3fv( getUniformLocation( name ), 1, GL_FALSE, &mat[0][0] );
        }

        void setMat4( const std::string & name, const glm::mat4 & mat ) const
        {
            glUniformMatrix4fv( getUniformLocation( name ), 1, GL_FALSE, &mat[0][0] );
        }

    public:
        unsigned int CreateShaderProgram( const std::string & vertexPath, const std::string & fragmentPath );

    public:
        int getUniformLocation( const std::string & name ) const
        {
            if ( m_uniformCache.find( name ) != m_uniformCache.end() )
            {
                return m_uniformCache[name];
            }

            int location = glGetUniformLocation( m_ID, name.c_str() );
            if ( location == -1 )
            {
                SHNK_CORE_WARN( "Uniform '{}' not found in shader!", name );
            }

            m_uniformCache[name] = location;

            return location;
        }

    private:
        unsigned int                                 m_ID;
        mutable std::unordered_map<std::string, int> m_uniformCache;
    };
}

#endif
