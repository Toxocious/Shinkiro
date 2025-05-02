project "Core-Input"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"

    targetname "SkinkiroInput"

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
        "CORE_INPUT_EXPORTS"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "PLATFORM_WINDOWS"
        }
