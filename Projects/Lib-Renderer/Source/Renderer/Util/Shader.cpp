#include <Renderer/_Common.h>

#include <Renderer/Util/Shader.h>

#include <Core/IApplication.h>

#include <Platform/GL.h>
#include <Platform/Modules/Window.h>

#include <Asset/AssetBundleManager.h>

#include <glad/glad.h>

namespace Shinkiro::Renderer
{
    Shader::Shader( const std::string & vertexPath, const std::string & fragmentPath )
    {
        m_ID = CreateShaderProgram( vertexPath, fragmentPath );
        if ( m_ID == 0 )
        {
            SHNK_CORE_ERROR( "Failed to create shader program from '{}' and '{}'", vertexPath, fragmentPath );
        }

        Shinkiro::Platform::OpenGL::Initialize( Shinkiro::Core::App->GetWindow()->GetGLFWWindow() );
    }

    unsigned int Shader::CreateShaderProgram( const std::string & vertexPath, const std::string & fragmentPath )
    {
        auto vertexBytes   = Shinkiro::Core::App->GetBundleManager().GetAssetData( vertexPath );
        auto fragmentBytes = Shinkiro::Core::App->GetBundleManager().GetAssetData( fragmentPath );

        if ( vertexBytes.empty() )
        {
            SHNK_CORE_ERROR( "Vertex shader '{}' is empty or missing!", vertexPath );
            return 0;
        }
        if ( fragmentBytes.empty() )
        {
            SHNK_CORE_ERROR( "Fragment shader '{}' is empty or missing!", fragmentPath );
            return 0;
        }

        std::string vertexSource(
            reinterpret_cast<const char *>( vertexBytes.data() ),
            ( vertexBytes.size() > 0 && vertexBytes.back() == 0 ) ? vertexBytes.size() - 1 : vertexBytes.size()
        );

        std::string fragmentSource(
            reinterpret_cast<const char *>( fragmentBytes.data() ),
            ( fragmentBytes.size() > 0 && fragmentBytes.back() == 0 ) ? fragmentBytes.size() - 1 : fragmentBytes.size()
        );

        auto compileShader = [this]( const std::string & shaderPath, GLenum type, const std::string & source ) -> unsigned int
        {
            unsigned int shader = Shinkiro::Platform::OpenGL::glCreateShader( type );
            if ( shader == 0 )
            {
                SHNK_CORE_ERROR( "Shinkiro::Platform::OpenGL::glCreateShader failed for type {}", type );
                return 0;
            }

            const char * src = source.c_str();
            Shinkiro::Platform::OpenGL::glShaderSource( shader, 1, &src, nullptr );
            Shinkiro::Platform::OpenGL::glCompileShader( shader );

            int success = 0;
            Shinkiro::Platform::OpenGL::glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
            if ( !success )
            {
                char infoLog[512];
                Shinkiro::Platform::OpenGL::glGetShaderInfoLog( shader, 512, nullptr, infoLog );

                const char * shaderType = ( type == GL_VERTEX_SHADER ) ? "VERTEX" : "FRAGMENT";
                SHNK_CORE_ERROR( "Failed to compile shader '{}' (type = {}) {}", shaderPath, shaderType, infoLog );

                Shinkiro::Platform::OpenGL::glDeleteShader( shader );
                return 0;
            }

            return shader;
        };

        unsigned int vertexShader = compileShader( vertexPath, GL_VERTEX_SHADER, vertexSource );
        if ( vertexShader == 0 )
        {
            Shinkiro::Platform::OpenGL::glDeleteShader( vertexShader );
            return 0;
        }

        unsigned int fragmentShader = compileShader( fragmentPath, GL_FRAGMENT_SHADER, fragmentSource );
        if ( fragmentShader == 0 )
        {
            Shinkiro::Platform::OpenGL::glDeleteShader( fragmentShader );
            return 0;
        }

        unsigned int shaderProgram = Shinkiro::Platform::OpenGL::glCreateProgram();
        Shinkiro::Platform::OpenGL::glAttachShader( shaderProgram, vertexShader );
        Shinkiro::Platform::OpenGL::glAttachShader( shaderProgram, fragmentShader );
        Shinkiro::Platform::OpenGL::glLinkProgram( shaderProgram );

        int success = 0;
        Shinkiro::Platform::OpenGL::glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );
        if ( !success )
        {
            char infoLog[512];
            Shinkiro::Platform::OpenGL::glGetProgramInfoLog( shaderProgram, 512, nullptr, infoLog );
            SHNK_CORE_ERROR( "Failed to link program shader '{}' -> {}", shaderProgram, infoLog );
            Shinkiro::Platform::OpenGL::glDeleteShader( vertexShader );
            Shinkiro::Platform::OpenGL::glDeleteShader( fragmentShader );
            Shinkiro::Platform::OpenGL::glDeleteProgram( shaderProgram );
            return 0;
        }

        Shinkiro::Platform::OpenGL::glDeleteShader( vertexShader );
        Shinkiro::Platform::OpenGL::glDeleteShader( fragmentShader );

        return shaderProgram;
    }
}
