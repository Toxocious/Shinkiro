project "spdlog"
    kind "StaticLib"
    language "C++"
    staticruntime "off"

    targetdir ("Build/" .. outputdir .. "/%{prj.name}")
    objdir ("Build/" .. outputdir .. "/%{prj.name}/Artifacts")

    files
    {
        "include/spdlog/**/*.h",

        "src/**.cpp",
    }

    includedirs
    {
        "include",
    }

    defines
    {
        "SPDLOG_COMPILED_LIB",
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
