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

        "../Lib-Core/Include",
        "../Lib-Asset/Include",
        "../Lib-Audio/Include",

        "./Include",
    }

    files
    {
        "%{wks.location}/Libraries/miniaudio/include/**.h",
        "%{wks.location}/Libraries/miniaudio/src/miniaudio.c",

        "%{wks.location}/Projects/Lib-Core/Include/**.h",
        "%{wks.location}/Projects/Lib-Asset/Include/**.h",
        "%{wks.location}/Projects/Lib-Audio/Include/**.h",
    }

    links
    {
        "Lib-Core",
        "Lib-Asset",
        "Lib-Audio",
    }

    dependson
    {
        "Lib-Core",
        "Lib-Asset",
        "Lib-Audio",
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
