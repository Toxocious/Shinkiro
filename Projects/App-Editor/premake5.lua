project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetname "ShinkiroEditor"

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
        "../Lib-Asset/Include",
        "../Lib-Audio/Include",
        "../Lib-Input/Include",
        "../Lib-Renderer/Include",

        "./Include",
    }

    links
    {
        "Lib-Asset",
        "Lib-Audio",
        "Lib-Input",
        "Lib-Renderer"
    }

    dependson
    {
        "Lib-Asset",
        "Lib-Audio",
        "Lib-Input",
        "Lib-Renderer"
    }

    defines
    {
        "PLATFORM_WINDOWS"
    }

    includeDependencies();
    linkDependencies();

    filter { "system:windows" }
        systemversion "latest"

		links
        {
            "OpenGL32"
        }

	filter { "system:not windows" }
		links
        {
            "GL"
        }

    filter { "configurations:Debug" }
        kind "ConsoleApp"

	filter { "configurations:Release" }
        kind "WindowedApp"

    filter { "configurations:Dist" }
        kind "WindowedApp"
