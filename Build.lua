-- Custom Actions
require "Clean"
require "Build-Resource-File"

-- Build Utility Functions
include "./Build-Utils.lua"

-- Current Git branch
local gitBranch = getCurrentGitBranch()

-- Output Directory Name
outputdir = "%{cfg.buildcfg}-%{string.gsub(cfg.system, '^%l', string.upper)}-%{cfg.architecture}"

-- Compiled binary and artifact directory locations
local BinaryDir = "Build\\Binaries\\%{outputdir}\\%{prj.name}"
local ObjectDir = "Build\\Artifacts\\%{outputdir}\\%{prj.name}"

-- Assets Path
local AssetsPath = "Assets"
local AssetBundlesPath = "AssetBundles"

-- Core DLL Paths
local CorePath = "Build\\Binaries\\%{outputdir}\\Lib-Core\\ShinkiroCore.dll"
local CoreAssetPath = "Build\\Binaries\\%{outputdir}\\Lib-Asset\\ShinkiroAsset.dll"
local CoreAudioPath = "Build\\Binaries\\%{outputdir}\\Lib-Audio\\ShinkiroAudio.dll"
local CoreLoggerPath = "Build\\Binaries\\%{outputdir}\\Lib-Logger\\ShinkiroLogger.dll"
local CorePlatformPath = "Build\\Binaries\\%{outputdir}\\Lib-Platform\\ShinkiroPlatform.dll"
local CoreRendererPath = "Build\\Binaries\\%{outputdir}\\Lib-Renderer\\ShinkiroRenderer.dll"

-- Executable Paths
local EditorBinaryDir = "Build\\Binaries\\%{outputdir}\\Editor"
local AssetPackerBinaryDir = "Build\\Binaries\\%{outputdir}\\AssetPacker"
local AssetUnpackerBinaryDir = "Build\\Binaries\\%{outputdir}\\AssetUnpacker"

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

    -- Configurations for Debug, Release, an Distribution
    filter "configurations:Debug"
        defines {
            "SHINKIRO_DEBUG",
            "SHINKIRO_ENABLE_ASSERTS",
            "SHINKIRO_PLATFORM_WINDOWS",
            "DSPDLOG_COMPILED_LIB",
            "_CRT_SECURE_NO_WARNINGS",
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
            "SPDLOG_COMPILED_LIB",
            "_CRT_SECURE_NO_WARNINGS",
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
            "SPDLOG_COMPILED_LIB",
            "_CRT_SECURE_NO_WARNINGS",
            "BUILD_TYPE=\"Dist\"",
            'SHINKIRO_VERSION="' .. (_G.appMetadata.version or "0.0.0") .. '"',
        }
        runtime "Release"
        optimize "On"
        symbols "Off"

    filter {}

    -- Include all project DLL configurations
    include "Projects/Lib-Core"
    include "Projects/Lib-Asset"
    include "Projects/Lib-Audio"
    include "Projects/Lib-Logger"
    include "Projects/Lib-Platform"
    include "Projects/Lib-Renderer"

    -- Include all project executable configurations
    include "Projects/App-Editor"
    include "Projects/App-AssetPacker"
    include "Projects/App-AssetUnpacker"

    -- Pre build commands for Editor project to create the Editor binary directory in case it isn't there
    -- This is necessary for the post build commands to actually copy the DLLs over
    prebuildcommands {
        "{MKDIR} %{wks.location}" .. EditorBinaryDir,
        "{MKDIR} %{wks.location}" .. AssetPackerBinaryDir,
        "{MKDIR} %{wks.location}" .. AssetUnpackerBinaryDir,
    }

    -- Post build commands for Core projects to copy DLLs to the Editor directory
    postbuildcommands {
        -- Copy the Assets directory to the AssetPacker executable directory
        "{COPYDIR} %{wks.location}" .. AssetsPath .. " %{wks.location}" .. AssetPackerBinaryDir .. "\\Assets",

        -- Copy AssetBundles to the Editor executable directory
        "{COPYDIR} %{wks.location}" .. AssetBundlesPath .. " %{wks.location}" .. EditorBinaryDir .. "\\AssetBundles",

        -- Copy Lib-Asset DLL to the AssetPacker and AssetUnpacker executable directories
        "{COPY} %{wks.location}" .. CoreAssetPath .. " %{wks.location}" .. AssetPackerBinaryDir,
        "{COPY} %{wks.location}" .. CoreAssetPath .. " %{wks.location}" .. AssetUnpackerBinaryDir,

        -- Copy all Lib-* DLLs to the Editor executable directory
        "{COPY} %{wks.location}" .. CorePath .. " %{wks.location}" .. EditorBinaryDir,
        "{COPY} %{wks.location}" .. CoreAssetPath .. " %{wks.location}" .. EditorBinaryDir,
        "{COPY} %{wks.location}" .. CoreAudioPath .. " %{wks.location}" .. EditorBinaryDir,
        "{COPY} %{wks.location}" .. CoreLoggerPath .. " %{wks.location}" .. EditorBinaryDir,
        "{COPY} %{wks.location}" .. CoreRendererPath .. " %{wks.location}" .. EditorBinaryDir,
        "{COPY} %{wks.location}" .. CorePlatformPath .. " %{wks.location}" .. EditorBinaryDir,
    }
