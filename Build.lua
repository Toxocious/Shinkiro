-- Custom Actions
require "Clean"
require "Build-Resource-File"
require "Build-Unzip-Deps"

-- Build Utility Functions
include "./Build-Utils.lua"

-- Current Git branch
local gitBranch = getCurrentGitBranch()

-- Output Directory Name
outputdir = "%{cfg.buildcfg}-%{string.gsub(cfg.system, '^%l', string.upper)}-%{cfg.architecture}"

-- Compiled binary and artifact directory locations
_G.BinaryDir = "Build\\Binaries\\%{outputdir}\\%{prj.name}"
_G.ObjectDir = "Build\\Artifacts\\%{outputdir}\\%{prj.name}"

-- Assets Path
_G.AssetsPath = "Assets"
_G.AssetBundlesPath = "AssetBundles"

-- Core DLL Paths
_G.CorePath = "Build\\Binaries\\%{outputdir}\\Lib-Core\\ShinkiroCore.dll"
_G.CoreAssetPath = "Build\\Binaries\\%{outputdir}\\Lib-Asset\\ShinkiroAsset.dll"
_G.CoreAudioPath = "Build\\Binaries\\%{outputdir}\\Lib-Audio\\ShinkiroAudio.dll"
_G.CorePlatformPath = "Build\\Binaries\\%{outputdir}\\Lib-Platform\\ShinkiroPlatform.dll"
_G.CoreRendererPath = "Build\\Binaries\\%{outputdir}\\Lib-Renderer\\ShinkiroRenderer.dll"

-- Executable Paths
_G.EditorBinaryDir = "Build\\Binaries\\%{outputdir}\\Editor"
_G.AssetPackerBinaryDir = "Build\\Binaries\\%{outputdir}\\AssetPacker"
_G.AssetUnpackerBinaryDir = "Build\\Binaries\\%{outputdir}\\AssetUnpacker"

-- Primary Workspace
workspace "Shinkiro"
    architecture "x86_64"
    startproject "Editor"

    targetdir (EditorBinaryDir)

    configurations
    {
        "Debug",
        "Release",
        "Dist",
    }

    _G.appMetadata = {
        version = "0.1.0",
        company = "Shinkiro Studios",
        product = "Shinkiro Engine",
        exe     = "ShinkiroEditor.exe"
    }

    -- Build options
    buildoptions {
        "/wd4251", -- Disable C4251 warning (needs to have dll-interface)
    }

    -- Linker options
    linkoptions {
        "/NODEFAULTLIB:MSVCRT" -- Disables LNK4098: defaultlib 'MSVCRT' conflicts…
    }

    -- Configurations for Debug, Release, an Distribution
    filter "configurations:Debug"
        defines {
            "SHINKIRO_DEBUG",
            "SHINKIRO_ENABLE_ASSERTS",
            "SHINKIRO_PLATFORM_WINDOWS",
            "ASSIMP_BUILD_NO_EXPORT",
            "ASSIMP_STATIC",
            "DSPDLOG_COMPILED_LIB",

            "_CRT_SECURE_NO_WARNINGS",
            "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",

            "BUILD_TYPE=\"" .. gitBranch .. "\"",
            'SHINKIRO_VERSION="' .. (_G.appMetadata.version or "0.0.0") .. '"',
        }
        runtime "Debug"
        optimize "Debug"
        symbols "Full"

    filter "configurations:Release"
        defines {
            "SHINKIRO_RELEASE",
            "SHINKIRO_ENABLE_ASSERTS",
            "SHINKIRO_PLATFORM_WINDOWS",
            "ASSIMP_BUILD_NO_EXPORT",
            "ASSIMP_STATIC",
            "SPDLOG_COMPILED_LIB",

            "_CRT_SECURE_NO_WARNINGS",
            "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",

            "BUILD_TYPE=\"Beta\"",
            'SHINKIRO_VERSION="' .. (_G.appMetadata.version or "0.0.0") .. '"',
        }
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        defines {
            "SHINKIRO_DIST",
            "SHINKIRO_ENABLE_ASSERTS",
            "SHINKIRO_PLATFORM_WINDOWS",
            "ASSIMP_BUILD_NO_EXPORT",
            "ASSIMP_STATIC",
            "SPDLOG_COMPILED_LIB",

            "_CRT_SECURE_NO_WARNINGS",
            "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",

            "BUILD_TYPE=\"Dist\"",
            'SHINKIRO_VERSION="' .. (_G.appMetadata.version or "0.0.0") .. '"',
        }
        runtime "Release"
        optimize "Full"
        symbols "Off"

    filter {}

    -- Include all project Lib-* DLL configurations
    include "Projects/Lib-Core"
    include "Projects/Lib-Asset"
    include "Projects/Lib-Audio"
    include "Projects/Lib-Platform"
    include "Projects/Lib-Renderer"

    -- Include all project App-* executable configurations
    include "Projects/App-Editor"
    include "Projects/App-AssetPacker"
    include "Projects/App-AssetUnpacker"
