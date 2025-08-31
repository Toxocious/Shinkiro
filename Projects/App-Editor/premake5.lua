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
        "%{wks.location}/Libraries/miniaudio/include/**.h",
        "%{wks.location}/Libraries/miniaudio/src/miniaudio.c",

        "**.h",
        "**.hpp",
        "**.cpp",
    }

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
        "PLATFORM_WINDOWS",
    }

    prebuildcommands {
        "{MKDIR} %{wks.location}" .. _G.EditorBinaryDir,
    }

    postbuildcommands {
        -- Copy assets
        copy_if_needed("%{wks.location}" .. _G.AssetsPath, "%{wks.location}" .. AssetPackerBinaryDir .. "\\Assets"),
        copy_if_needed("%{wks.location}" .. _G.AssetBundlesPath, "%{wks.location}" .. EditorBinaryDir .. "\\AssetBundles"),

        -- Copy required DLLs for tools
        copy_if_needed("%{wks.location}" .. _G.CoreAssetPath, "%{wks.location}" .. AssetPackerBinaryDir),
        copy_if_needed("%{wks.location}" .. _G.CoreAssetPath, "%{wks.location}" .. AssetUnpackerBinaryDir),
        copy_if_needed("%{wks.location}" .. _G.CoreLoggerPath, "%{wks.location}" .. AssetPackerBinaryDir),
        copy_if_needed("%{wks.location}" .. _G.CoreLoggerPath, "%{wks.location}" .. AssetUnpackerBinaryDir),

        -- Copy all Lib-* DLLs to the Editor executable directory
        copy_if_needed("%{wks.location}" .. _G.CorePath, "%{wks.location}" .. EditorBinaryDir),
        copy_if_needed("%{wks.location}" .. _G.CoreAssetPath, "%{wks.location}" .. EditorBinaryDir),
        copy_if_needed("%{wks.location}" .. _G.CoreAudioPath, "%{wks.location}" .. EditorBinaryDir),
        copy_if_needed("%{wks.location}" .. _G.CoreLoggerPath, "%{wks.location}" .. EditorBinaryDir),
        copy_if_needed("%{wks.location}" .. _G.CoreRendererPath, "%{wks.location}" .. EditorBinaryDir),
        copy_if_needed("%{wks.location}" .. _G.CorePlatformPath, "%{wks.location}" .. EditorBinaryDir),
    }

    includeDependencies();
    linkDependencies();

    filter { "system:windows" }
        systemversion "latest"
        buildoptions { "/FS", "/MP" }
        files { '%{wks.location}/Assets/Resources.rc', '**.ico' }
        vpaths { [ '%{wks.location}/Assets/*' ] = { '**.ico' } }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }

    filter { "configurations:Debug" }
        kind "ConsoleApp"

    filter { "configurations:Release" }
        kind "WindowedApp"

    filter { "configurations:Dist" }
        kind "WindowedApp"
