project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

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
        "../Lib-Core/Include",
        "../Lib-Asset/Include",

        "./Include",
    }

    links
    {
        "Lib-Core",
        "Lib-Asset",
    }

    dependson
    {
        "Lib-Core",
        "Lib-Asset",
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
