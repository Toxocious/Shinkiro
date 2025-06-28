project "Lib-Renderer"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    targetname "ShinkiroRenderer"

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
        "CORE_RENDERER_EXPORTS"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "PLATFORM_WINDOWS"
        }
