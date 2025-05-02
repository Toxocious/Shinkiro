-- Custom Actions
require "Clean"

-- Build Utility Functions
include "./Build-Utils.lua"

-- Output Directory Name
outputdir = "%{cfg.buildcfg}-%{string.gsub(cfg.system, '^%l', string.upper)}-%{cfg.architecture}"

-- Compiled binary and artifact directory locations
local BinaryDir = "Build\\Binaries\\%{outputdir}\\%{prj.name}"
local ObjectDir = "Build\\Artifacts\\%{outputdir}\\%{prj.name}"

-- Core DLL Paths
local CoreAudioBinaryDllPath = "Build\\Binaries\\%{outputdir}\\Core-Audio\\SkinkiroAudio.dll"
local CoreInputBinaryDllPath = "Build\\Binaries\\%{outputdir}\\Core-Input\\SkinkiroInput.dll"
local CoreRendererBinaryDllPath = "Build\\Binaries\\%{outputdir}\\Core-Renderer\\SkinkiroRenderer.dll"

-- Executable Paths
local EditorBinaryDir = "Build\\Binaries\\%{outputdir}\\Editor"

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
        runtime "Debug"
        optimize "Debug"
        symbols "Full"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"
        symbols "Off"

    -- Include all project configurations
    include "Projects/Core-Audio"
    include "Projects/Core-Input"
    include "Projects/Core-Renderer"
    include "Projects/Editor"

    -- Pre build commands for Editor project to create the Editor binary directory in case it isn't there
    -- This is necessary for the post build commands to actually copy the DLLs over
    prebuildcommands {
        "{MKDIR} %{wks.location}" .. EditorBinaryDir,
    }

    -- Post build commands for Core projects to copy DLLs to the Editor directory
    postbuildcommands {
        "{COPY} %{wks.location}" .. CoreAudioBinaryDllPath .. " %{wks.location}" .. EditorBinaryDir,
        "{COPY} %{wks.location}" .. CoreInputBinaryDllPath .. " %{wks.location}" .. EditorBinaryDir,
        "{COPY} %{wks.location}" .. CoreRendererBinaryDllPath .. " %{wks.location}" .. EditorBinaryDir,
    }
