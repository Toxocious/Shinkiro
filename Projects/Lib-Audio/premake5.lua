project "Lib-Audio"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    targetname "ShinkiroAudio"

    targetdir ("../../Build/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../../Build/Artifacts/" .. outputdir .. "/%{prj.name}")

    files
    {
        "**.h",
        "**.hpp",
        "**.cpp"
    }

    includedirs
    {
        "./Include",
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
