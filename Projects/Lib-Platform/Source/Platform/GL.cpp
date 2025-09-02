#include <Platform/_Common.h>

#include <Platform/GL.h>

namespace Shinkiro::Platform::OpenGL
{
#define DEFINE_GL_FUNC( type, name ) PLATFORM_API type name = nullptr;

    DEFINE_GL_FUNC( PFNGLCREATESHADERPROC, glCreateShader )
    DEFINE_GL_FUNC( PFNGLDELETESHADERPROC, glDeleteShader )
    DEFINE_GL_FUNC( PFNGLSHADERSOURCEPROC, glShaderSource )
    DEFINE_GL_FUNC( PFNGLCOMPILESHADERPROC, glCompileShader )
    DEFINE_GL_FUNC( PFNGLGETSHADERIVPROC, glGetShaderiv )
    DEFINE_GL_FUNC( PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog )
    DEFINE_GL_FUNC( PFNGLCREATEPROGRAMPROC, glCreateProgram )
    DEFINE_GL_FUNC( PFNGLDELETEPROGRAMPROC, glDeleteProgram )
    DEFINE_GL_FUNC( PFNGLATTACHSHADERPROC, glAttachShader )
    DEFINE_GL_FUNC( PFNGLLINKPROGRAMPROC, glLinkProgram )
    DEFINE_GL_FUNC( PFNGLUSEPROGRAMPROC, glUseProgram )
    DEFINE_GL_FUNC( PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation )
    DEFINE_GL_FUNC( PFNGLUNIFORM1IPROC, glUniform1i )
    DEFINE_GL_FUNC( PFNGLUNIFORM2FVPROC, glUniform2fv )
    DEFINE_GL_FUNC( PFNGLUNIFORM3FVPROC, glUniform3fv )
    DEFINE_GL_FUNC( PFNGLUNIFORM4FVPROC, glUniform4fv )
    DEFINE_GL_FUNC( PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv )
    DEFINE_GL_FUNC( PFNGLGENBUFFERSPROC, glGenBuffers )
    DEFINE_GL_FUNC( PFNGLDELETEBUFFERSPROC, glDeleteBuffers )
    DEFINE_GL_FUNC( PFNGLBINDBUFFERPROC, glBindBuffer )
    DEFINE_GL_FUNC( PFNGLBUFFERDATAPROC, glBufferData )
    DEFINE_GL_FUNC( PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray )
    DEFINE_GL_FUNC( PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer )
    DEFINE_GL_FUNC( PFNGLGENTEXTURESPROC, glGenTextures )
    DEFINE_GL_FUNC( PFNGLDELETETEXTURESPROC, glDeleteTextures )
    DEFINE_GL_FUNC( PFNGLBINDTEXTUREPROC, glBindTexture )
    DEFINE_GL_FUNC( PFNGLTEXPARAMETERIPROC, glTexParameteri )
    DEFINE_GL_FUNC( PFNGLACTIVETEXTUREPROC, glActiveTexture )
    DEFINE_GL_FUNC( PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays )
    DEFINE_GL_FUNC( PFNGLBINDVERTEXARRAYPROC, glBindVertexArray )
    DEFINE_GL_FUNC( PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays )
    DEFINE_GL_FUNC( PFNGLDRAWARRAYSPROC, glDrawArrays )
    DEFINE_GL_FUNC( PFNGLDRAWELEMENTSPROC, glDrawElements )
    DEFINE_GL_FUNC( PFNGLDEPTHFUNCPROC, glDepthFunc )
    DEFINE_GL_FUNC( PFNGLDEPTHMASKPROC, glDepthMask )
    DEFINE_GL_FUNC( PFNGLENABLEPROC, glEnable )
    DEFINE_GL_FUNC( PFNGLDISABLEPROC, glDisable )
    DEFINE_GL_FUNC( PFNGLBLENDFUNCPROC, glBlendFunc )
    DEFINE_GL_FUNC( PFNGLGETBOOLEANVPROC, glGetBooleanv )
    DEFINE_GL_FUNC( PFNGLISENABLEDPROC, glIsEnabled )
    DEFINE_GL_FUNC( PFNGLGETPROGRAMIVPROC, glGetProgramiv )
    DEFINE_GL_FUNC( PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog )
    DEFINE_GL_FUNC( PFNGLTEXIMAGE2DPROC, glTexImage2D )
    DEFINE_GL_FUNC( PFNGLGENERATEMIPMAPPROC, glGenerateMipmap )
    DEFINE_GL_FUNC( PFNGLCLEARCOLORPROC, glClearColor )
    DEFINE_GL_FUNC( PFNGLCLEARPROC, glClear )
    DEFINE_GL_FUNC( PFNGLUNIFORM1FPROC, glUniform1f )
    DEFINE_GL_FUNC( PFNGLUNIFORMMATRIX2FVPROC, glUniformMatrix2fv )
    DEFINE_GL_FUNC( PFNGLUNIFORMMATRIX3FVPROC, glUniformMatrix3fv )
    DEFINE_GL_FUNC( PFNGLVIEWPORTPROC, glViewport )

#undef DEFINE_GL_FUNC

    bool Initialize( GLFWwindow * window )
    {
        glfwMakeContextCurrent( window );

        if ( !gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress ) )
        {
            return false;
        }

        // Copy all GLAD function pointers to exported variables
        glCreateShader            = ::glCreateShader;
        glDeleteShader            = ::glDeleteShader;
        glShaderSource            = ::glShaderSource;
        glCompileShader           = ::glCompileShader;
        glGetShaderiv             = ::glGetShaderiv;
        glGetShaderInfoLog        = ::glGetShaderInfoLog;
        glCreateProgram           = ::glCreateProgram;
        glDeleteProgram           = ::glDeleteProgram;
        glAttachShader            = ::glAttachShader;
        glLinkProgram             = ::glLinkProgram;
        glUseProgram              = ::glUseProgram;
        glGetUniformLocation      = ::glGetUniformLocation;
        glUniform1i               = ::glUniform1i;
        glUniform2fv              = ::glUniform2fv;
        glUniform3fv              = ::glUniform3fv;
        glUniform4fv              = ::glUniform4fv;
        glUniformMatrix4fv        = ::glUniformMatrix4fv;
        glGenBuffers              = ::glGenBuffers;
        glDeleteBuffers           = ::glDeleteBuffers;
        glBindBuffer              = ::glBindBuffer;
        glBufferData              = ::glBufferData;
        glEnableVertexAttribArray = ::glEnableVertexAttribArray;
        glVertexAttribPointer     = ::glVertexAttribPointer;
        glGenTextures             = ::glGenTextures;
        glDeleteTextures          = ::glDeleteTextures;
        glBindTexture             = ::glBindTexture;
        glTexParameteri           = ::glTexParameteri;
        glActiveTexture           = ::glActiveTexture;
        glGenVertexArrays         = ::glGenVertexArrays;
        glBindVertexArray         = ::glBindVertexArray;
        glDeleteVertexArrays      = ::glDeleteVertexArrays;
        glDrawArrays              = ::glDrawArrays;
        glDrawElements            = ::glDrawElements;
        glDepthFunc               = ::glDepthFunc;
        glDepthMask               = ::glDepthMask;
        glEnable                  = ::glEnable;
        glDisable                 = ::glDisable;
        glBlendFunc               = ::glBlendFunc;
        glGetBooleanv             = ::glGetBooleanv;
        glIsEnabled               = ::glIsEnabled;
        glGetProgramiv            = ::glGetProgramiv;
        glGetProgramInfoLog       = ::glGetProgramInfoLog;
        glTexImage2D              = ::glTexImage2D;
        glGenerateMipmap          = ::glGenerateMipmap;
        glClearColor              = ::glClearColor;
        glClear                   = ::glClear;
        glUniform1f               = ::glUniform1f;
        glUniformMatrix2fv        = ::glUniformMatrix2fv;
        glUniformMatrix3fv        = ::glUniformMatrix3fv;
        glViewport                = ::glViewport;

        return true;
    }
}
