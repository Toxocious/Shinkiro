project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetname "SkinkiroEditor"

    targetdir ("../../Build/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../../Build/Artifacts/" .. outputdir .. "/%{prj.name}")

    entrypoint "mainCRTStartup"

    files
    {
        "**.h",
        "**.hpp",
        "**.cpp"
    }

    includedirs
    {
        "../Core-Audio/Include",
        "../Core-Input/Include",
        "../Core-Renderer/Include",

        "./Include",
    }

    links
    {
        "Core-Audio",
        "Core-Input",
        "Core-Renderer"
    }

    dependson
    {
        "Core-Audio",
        "Core-Input",
        "Core-Renderer"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "PLATFORM_WINDOWS"
        }
