project "Lib-Audio"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    targetname "ShinkiroAudio"

    targetdir ("../../Build/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../../Build/Artifacts/" .. outputdir .. "/%{prj.name}")

    pchheader "Audio/_Common.h"
    pchsource "Source/Audio/_Common.cpp"

    includedirs
    {
        "%{wks.location}/Libraries/miniaudio/include",

        "%{wks.location}/Projects/Lib-Core/Include",

        "./Include",
    }

    files
    {
        -- "%{wks.location}/Libraries/miniaudio/include/**.h",
        "%{wks.location}/Libraries/miniaudio/src/miniaudio.c",


        "%{wks.location}/Projects/Lib-Core/Include/Core/Util/Log.h",
        "%{wks.location}/Projects/Lib-Core/Include/Core/Interfaces/AudioManagerInterface.h",

        "**.h",
        "**.hpp",
        "**.cpp",
    }

    links
    {
        "Lib-Core",
    }

    dependson
    {
        "Lib-Core",
    }

    defines
    {
        "SHINKIRO_AUDIO_EXPORTS"
    }

    filter { "files:**/Libraries/miniaudio/**.c" }
        flags { "NoPCH" }

    filter {}

    filter "system:windows"
        systemversion "latest"

        buildoptions { "/FS", "/MP" }

        defines
        {
            "PLATFORM_WINDOWS"
        }
