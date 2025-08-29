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
        "%{wks.location}/Libraries/spdlog/include",

        "%{wks.location}/Projects/Lib-Logger/Include",

        "./Include",
    }

    files
    {
        "%{wks.location}/Libraries/miniaudio/include/**.h",
        "%{wks.location}/Libraries/miniaudio/src/miniaudio.c",

        "%{wks.location}/Libraries/spdlog/include/**.h",

        "%{wks.location}/Projects/Lib-Logger/Include/**.h",

        "**.h",
        "**.hpp",
        "**.cpp",
    }

    libdirs
    {
        "%{wks.location}/Libraries/spdlog/lib",
    }

    links
    {
        "Lib-Logger",
    }

    dependson
    {
        "Lib-Logger",
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
