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
        void Use() const
        {
            glUseProgram( m_ID );
        }

    public:
        unsigned int GetID() const
        {
            return m_ID;
        }

    public:
        void SetBool( const std::string & name, bool value ) const
        {
            glUniform1i( GetUniformLocation( name ), ( int ) value );
        }

        void SetInt( const std::string & name, int value ) const
        {
            glUniform1i( GetUniformLocation( name ), value );
        }

        void SetFloat( const std::string & name, float value ) const
        {
            glUniform1f( GetUniformLocation( name ), value );
        }

        void SetVec2( const std::string & name, const glm::vec2 & value ) const
        {
            glUniform2fv( GetUniformLocation( name ), 1, &value[0] );
        }

        void SetVec3( const std::string & name, const glm::vec3 & value ) const
        {
            glUniform3fv( GetUniformLocation( name ), 1, &value[0] );
        }

        void SetVec4( const std::string & name, const glm::vec4 & value ) const
        {
            glUniform4fv( GetUniformLocation( name ), 1, &value[0] );
        }

        void SetMat2( const std::string & name, const glm::mat2 & mat ) const
        {
            glUniformMatrix2fv( GetUniformLocation( name ), 1, GL_FALSE, &mat[0][0] );
        }

        void SetMat3( const std::string & name, const glm::mat3 & mat ) const
        {
            glUniformMatrix3fv( GetUniformLocation( name ), 1, GL_FALSE, &mat[0][0] );
        }

        void SetMat4( const std::string & name, const glm::mat4 & mat ) const
        {
            glUniformMatrix4fv( GetUniformLocation( name ), 1, GL_FALSE, &mat[0][0] );
        }

    public:
        unsigned int CreateShaderProgram( const std::string & vertexPath, const std::string & fragmentPath );

    public:
        int GetUniformLocation( const std::string & name ) const
        {
            if ( m_UniformCache.find( name ) != m_UniformCache.end() )
            {
                return m_UniformCache[name];
            }

            int location = glGetUniformLocation( m_ID, name.c_str() );
            if ( location == -1 )
            {
                SHNK_CORE_WARN( "Uniform '{}' not found in shader!", name );
            }

            m_UniformCache[name] = location;

            return location;
        }

    private:
        unsigned int                                 m_ID;
        mutable std::unordered_map<std::string, int> m_UniformCache;
    };
}

#endif
