project "Lib-Renderer"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    targetname "ShinkiroRenderer"

    targetdir ("../../Build/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../../Build/Artifacts/" .. outputdir .. "/%{prj.name}")

    pchheader "Renderer/_Common.h"
    pchsource "Source/Renderer/_Common.cpp"

    files
    {
        -- "%{wks.location}/Libraries/glad/include/**.h",
        -- "%{wks.location}/Libraries/glad/src/glad.c",

        -- "%{wks.location}/Libraries/glfw/include/**.h",

        -- "%{wks.location}/Libraries/glm/include/**.hpp",
        -- "%{wks.location}/Libraries/glm/include/**.inl",

        -- "%{wks.location}/Libraries/imgui/include/**.h",
		-- "%{wks.location}/Libraries/imgui/include/**.cpp",

        -- "%{wks.location}/Libraries/stb_image/include/**.h",
        "%{wks.location}/Libraries/stb_image/include/**.cpp",

        "**.h",
        "**.hpp",
        "**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Libraries/assimp/include",
        "%{wks.location}/Libraries/glad/include",
        "%{wks.location}/Libraries/glfw/include",
        "%{wks.location}/Libraries/glm/include",
        "%{wks.location}/Libraries/imgui/include",
        "%{wks.location}/Libraries/stb_image/include",

        "%{wks.location}/Projects/Lib-Asset/Include",
        "%{wks.location}/Projects/Lib-Audio/Include",
        "%{wks.location}/Projects/Lib-Core/Include",
        "%{wks.location}/Projects/Lib-Platform/Include",

        "./Include",
    }

    libdirs
    {
        "%{wks.location}/Libraries/assimp/lib",
        "%{wks.location}/Libraries/glad/lib",
        "%{wks.location}/Libraries/glfw/lib",
        "%{wks.location}/Libraries/glm/lib",
    }

    links
    {
        "Lib-Audio",
        "Lib-Asset",
        "Lib-Core",
        "Lib-Platform",
    }

    dependson
    {
        "Lib-Audio",
        "Lib-Asset",
        "Lib-Core",
        "Lib-Platform",
    }

    defines
    {
        "SHINKIRO_RENDERER_EXPORTS",
        "ASSIMP_BUILD_NO_EXPORT"
    }

    filter { "files:**/Libraries/**.c" }
        flags { "NoPCH" }
    filter { "files:**/Libraries/**.cpp" }
        flags { "NoPCH" }
    filter {}

    filter { "configurations:Debug" }
        links { "assimp_debug", "zlib_debug", "glad_debug", "glfw3", "glm_debug" }

    filter { "configurations:Dist" }
        links { "assimp_dist", "zlib_dist", "glad_dist", "glfw3", "glm_dist" }

    filter { "configurations:Release" }
        links { "assimp_release", "zlib_release", "glad_release", "glfw3", "glm_release" }

    filter "system:windows"
        systemversion "latest"
        buildoptions { "/FS", "/MP" }
        defines { "PLATFORM_WINDOWS" }
