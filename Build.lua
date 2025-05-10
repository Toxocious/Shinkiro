-- Custom Actions
require "Clean"

-- Build Utility Functions
include "./Build-Utils.lua"

-- Output Directory Name
outputdir = "%{cfg.buildcfg}-%{string.gsub(cfg.system, '^%l', string.upper)}-%{cfg.architecture}"

-- Compiled binary and artifact directory locations
local BinaryDir = "Build\\Binaries\\%{outputdir}\\%{prj.name}"
local ObjectDir = "Build\\Artifacts\\%{outputdir}\\%{prj.name}"

-- Assets Path
local AssetsPath = "Assets"

-- Core DLL Paths
local CoreAssetPath = "Build\\Binaries\\%{outputdir}\\Core-Asset\\SkinkiroAsset.dll"
local CoreAudioPath = "Build\\Binaries\\%{outputdir}\\Core-Audio\\SkinkiroAudio.dll"
local CoreInputPath = "Build\\Binaries\\%{outputdir}\\Core-Input\\SkinkiroInput.dll"
local CoreRendererPath = "Build\\Binaries\\%{outputdir}\\Core-Renderer\\SkinkiroRenderer.dll"

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

    -- Configurations for Debug, Release, an Distribution
    filter "configurations:Debug"
        defines {
            "SHINKIRO_DEBUG",
            "SHINKIRO_ENABLE_ASSERTS",
            "SHINKIRO_PLATFORM_WINDOWS",
            "DSPDLOG_COMPILED_LIB",
            "_CRT_SECURE_NO_WARNINGS",
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
        }
        runtime "Release"
        optimize "On"
        symbols "Off"

    filter {}

    -- Include all project DLL configurations
    include "Projects/Core-Asset"
    include "Projects/Core-Audio"
    include "Projects/Core-Input"
    include "Projects/Core-Renderer"

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

        -- Copy Core-Asset DLL to the AssetPacker and AssetUnpacker executable directories
        "{COPY} %{wks.location}" .. CoreAssetPath .. " %{wks.location}" .. AssetPackerBinaryDir,
        "{COPY} %{wks.location}" .. CoreAssetPath .. " %{wks.location}" .. AssetUnpackerBinaryDir,

        -- Copy all Core-* DLLs to the Editor executable directory
        "{COPY} %{wks.location}" .. CoreAssetPath .. " %{wks.location}" .. EditorBinaryDir,
        "{COPY} %{wks.location}" .. CoreAudioPath .. " %{wks.location}" .. EditorBinaryDir,
        "{COPY} %{wks.location}" .. CoreInputPath .. " %{wks.location}" .. EditorBinaryDir,
        "{COPY} %{wks.location}" .. CoreRendererPath .. " %{wks.location}" .. EditorBinaryDir,
    }
