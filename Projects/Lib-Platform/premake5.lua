project "Lib-Platform"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    targetname "ShinkiroPlatform"

    targetdir ("../../Build/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../../Build/Artifacts/" .. outputdir .. "/%{prj.name}")

    pchheader "Platform/_Common.h"
    pchsource "Source/Platform/_Common.cpp"

    files
    {
        "%{wks.location}/Libraries/glad/include/**.h",
		"%{wks.location}/Libraries/glad/src/glad.c",

		"%{wks.location}/Libraries/glfw/include/**.h",

        -- "%{wks.location}/Libraries/glm/include/**.hpp",
		-- "%{wks.location}/Libraries/glm/include/**.inl",

        -- "%{wks.location}/Libraries/imgui/include/**.h",
		"%{wks.location}/Libraries/imgui/include/**.cpp",

        -- "%{wks.location}/Libraries/miniaudio/include/**.h",
        -- "%{wks.location}/Libraries/miniaudio/src/miniaudio.c",

        -- "%{wks.location}/Libraries/stb_image/include/**.h",
		"%{wks.location}/Libraries/stb_image/include/**.cpp",

        "%{wks.location}/Projects/Lib-Asset/Include/**.h",
        "%{wks.location}/Projects/Lib-Audio/Include/**.h",
        "%{wks.location}/Projects/Lib-Core/Include/**.h",

        "**.h",
        "**.hpp",
        "**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Libraries/glad/include",
        "%{wks.location}/Libraries/glfw/include",
		"%{wks.location}/Libraries/glm/include",
        "%{wks.location}/Libraries/imgui/include",
        "%{wks.location}/Libraries/lucide/include",
        "%{wks.location}/Libraries/miniaudio/include",
        "%{wks.location}/Libraries/stb_image/include",

        "%{wks.location}/Projects/Lib-Asset/Include",
        "%{wks.location}/Projects/Lib-Audio/Include",
        "%{wks.location}/Projects/Lib-Core/Include",

        "./Include",
    }

    libdirs
    {
        "%{wks.location}/Libraries/glad/lib",
		"%{wks.location}/Libraries/glfw/lib",
		"%{wks.location}/Libraries/glm/lib",
		"%{wks.location}/Libraries/imgui/lib",
    }

    links
    {
        "Lib-Audio",
        "Lib-Asset",
        "Lib-Core",
    }

    dependson
    {
        "Lib-Audio",
        "Lib-Asset",
        "Lib-Core",
    }

    defines
    {
        "SHINKIRO_PLATFORM_EXPORTS"
    }

    filter { "files:**/Libraries/glad/**.c" }
        flags { "NoPCH" }
    filter { "files:**/Libraries/glfw/**.cpp" }
        flags { "NoPCH" }
    filter { "files:**/Libraries/glm/**.cpp" }
        flags { "NoPCH" }
    filter { "files:**/Libraries/miniaudio/**.c" }
        flags { "NoPCH" }
    filter { "files:**/Libraries/imgui/**.cpp" }
        flags { "NoPCH" }
    filter { "files:**/Libraries/stb_image/**.cpp" }
        flags { "NoPCH" }

    filter {}

    filter { "configurations:Debug" }
		links {
			"glad_debug",
			"glfw3",
			"glm_debug",
			"imgui_debug",
		}

	filter { "configurations:Dist" }
		links {
			"glad_dist",
			"glfw3",
			"glm_dist",
			"imgui_dist",
		}

	filter { "configurations:Release" }
		links {
			"glad_release",
			"glfw3",
			"glm_release",
			"imgui_release",
		}

    filter "system:windows"
        systemversion "latest"

        buildoptions { "/FS", "/MP" }

        defines
        {
            "PLATFORM_WINDOWS"
        }
