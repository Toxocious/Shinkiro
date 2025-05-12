#ifdef PLATFORM_WINDOWS
#    ifdef SHINKIRO_CORE_EXPORTS
#        define CORE_API __declspec( dllexport )
#    else
#        define CORE_API __declspec( dllimport )
#    endif
#else
#    define CORE_API
#endif

#pragma once

#include <string>
#include <vector>

class CORE_API Application
{
public:
    Application();
    ~Application();

public:
    bool Initialize( std::string name, std::string version, int height, int width );
    bool Start();
    bool CleanUp();

    bool PreUpdate();
    bool Update();
    bool PostUpdate();

private:
    std::string m_Name;
    std::string m_Version;
    int         m_Height;
    int         m_Width;
};

extern Application * App;
