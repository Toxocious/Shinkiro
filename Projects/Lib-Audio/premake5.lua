project "Lib-Audio"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    targetname "ShinkiroAudio"

    targetdir ("../../Build/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../../Build/Artifacts/" .. outputdir .. "/%{prj.name}")

    includedirs
    {
        "%{wks.location}/Libraries/miniaudio/include",

        "./Include",
    }

    files
    {
        "%{wks.location}/Libraries/miniaudio/include/**.h",
        "%{wks.location}/Libraries/miniaudio/src/miniaudio.c",

        "**.h",
        "**.hpp",
        "**.cpp",
    }

    defines
    {
        "SHINKIRO_AUDIO_EXPORTS"
    }

    filter "system:windows"
        systemversion "latest"

        buildoptions { "/FS" }

        defines
        {
            "PLATFORM_WINDOWS"
        }
