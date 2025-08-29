#include <Renderer/_Common.h>

#include <Renderer/Util/Shader.h>

#include <Core/IApplication.h>

#include <Asset/AssetBundleManager.h>

namespace Shinkiro::Renderer
{
    Shader::Shader( const std::string & vertexPath, const std::string & fragmentPath )
    {
        m_ID = CreateShaderProgram( vertexPath, fragmentPath );
        if ( m_ID == 0 )
        {
            SHNK_CORE_ERROR( "Failed to create shader program from '{}' and '{}'", vertexPath, fragmentPath );
        }
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

        std::string vertexSource( reinterpret_cast<const char *>( vertexBytes.data() ), vertexBytes.size() );
        std::string fragmentSource( reinterpret_cast<const char *>( fragmentBytes.data() ), fragmentBytes.size() );

        auto compileShader = []( GLenum type, const std::string & source ) -> unsigned int
        {
            unsigned int shader = glCreateShader( type );
            const char * src    = source.c_str();
            glShaderSource( shader, 1, &src, nullptr );
            glCompileShader( shader );

            int success = 0;
            glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
            if ( !success )
            {
                char infoLog[512];
                glGetShaderInfoLog( shader, 512, nullptr, infoLog );
                const char * shaderType = ( type == GL_VERTEX_SHADER ) ? "VERTEX" : "FRAGMENT";
                SHNK_CORE_ERROR( "ERROR::SHADER::{}::COMPILATION_FAILED\n{}", shaderType, infoLog );
                glDeleteShader( shader );
                return 0;
            }

            return shader;
        };

        unsigned int vertexShader = compileShader( GL_VERTEX_SHADER, vertexSource );
        if ( vertexShader == 0 )
        {
            return 0;
        }

        unsigned int fragmentShader = compileShader( GL_FRAGMENT_SHADER, fragmentSource );
        if ( fragmentShader == 0 )
        {
            glDeleteShader( vertexShader );
            return 0;
        }

        unsigned int shaderProgram = glCreateProgram();
        glAttachShader( shaderProgram, vertexShader );
        glAttachShader( shaderProgram, fragmentShader );
        glLinkProgram( shaderProgram );

        int success = 0;
        glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );
        if ( !success )
        {
            char infoLog[512];
            glGetProgramInfoLog( shaderProgram, 512, nullptr, infoLog );
            SHNK_CORE_ERROR( "ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}", infoLog );
            glDeleteShader( vertexShader );
            glDeleteShader( fragmentShader );
            glDeleteProgram( shaderProgram );
            return 0;
        }

        glDeleteShader( vertexShader );
        glDeleteShader( fragmentShader );

        return shaderProgram;
    }
}
