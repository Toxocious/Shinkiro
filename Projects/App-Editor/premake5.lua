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
        "%{wks.location}/Libraries/miniaudio/include",

        "../Lib-Asset/Include",
        "../Lib-Audio/Include",
        "../Lib-Core/Include",
        "../Lib-Logger/Include",
        "../Lib-Platform/Include",

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
    }

    links
    {
        "Lib-Core",
        "Lib-Asset",
        "Lib-Audio",
        "Lib-Logger",
        "Lib-Platform",
    }

    dependson
    {
        "Lib-Core",
        "Lib-Asset",
        "Lib-Audio",
        "Lib-Logger",
        "Lib-Platform",
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
