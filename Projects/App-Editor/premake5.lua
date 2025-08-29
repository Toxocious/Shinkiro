project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetname "ShinkiroEditor"

    targetdir ("../../Build/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../../Build/Artifacts/" .. outputdir .. "/%{prj.name}")

    entrypoint "mainCRTStartup"

    -- Set the program icon
    filter { "system:windows" }
        files { '%{wks.location}/Assets/Resources.rc', '**.ico' }
        vpaths { [ '%{wks.location}/Assets/*' ] = { '**.ico' } }

    includedirs
    {
        "%{wks.location}/Libraries/miniaudio/include",

        "%{wks.location}/Projects/Lib-Asset/Include",
        "%{wks.location}/Projects/Lib-Audio/Include",
        "%{wks.location}/Projects/Lib-Core/Include",
        "%{wks.location}/Projects/Lib-Logger/Include",
        "%{wks.location}/Projects/Lib-Platform/Include",
        "%{wks.location}/Projects/Lib-Renderer/Include",

        "./Include",
    }

    files
    {
        "%{wks.location}/Libraries/miniaudio/include/**.h",
        "%{wks.location}/Libraries/miniaudio/src/miniaudio.c",

        "%{wks.location}/Projects/Lib-Core/Include/**.h",
        "%{wks.location}/Projects/Lib-Asset/Include/**.h",
        "%{wks.location}/Projects/Lib-Audio/Include/**.h",
        "%{wks.location}/Projects/Lib-Logger/Include/**.h",
        "%{wks.location}/Projects/Lib-Platform/Include/**.h",
        "%{wks.location}/Projects/Lib-Renderer/Include/**.h",

        "**.h",
        "**.hpp",
        "**.cpp",
    }

    links
    {
        "Lib-Core",
        "Lib-Asset",
        "Lib-Audio",
        "Lib-Logger",
        "Lib-Platform",
        "Lib-Renderer",
    }

    dependson
    {
        "Lib-Core",
        "Lib-Asset",
        "Lib-Audio",
        "Lib-Logger",
        "Lib-Platform",
        "Lib-Renderer",
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
