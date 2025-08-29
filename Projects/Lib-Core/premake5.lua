project "Lib-Core"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    targetname "ShinkiroCore"

    targetdir ("../../Build/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../../Build/Artifacts/" .. outputdir .. "/%{prj.name}")

    defines
    {
        "SHINKIRO_CORE_EXPORTS"
    }

    includedirs
    {
        "%{wks.location}/Libraries/glad/include",
        "%{wks.location}/Libraries/glfw/include",
		"%{wks.location}/Libraries/glm/include",

        "%{wks.location}/Libraries/miniaudio/include",

        "%{wks.location}/Libraries/stb_image/include",

        "%{wks.location}/Libraries/spdlog/include",

        "%{wks.location}/Projects/Lib-Asset/Include",
        "%{wks.location}/Projects/Lib-Audio/Include",
        "%{wks.location}/Projects/Lib-Logger/Include",
        "%{wks.location}/Projects/Lib-Platform/Include",

        "./Include",
    }

    files
    {
        "%{wks.location}/Libraries/glad/include/**.h",
		"%{wks.location}/Libraries/glad/src/glad.c",

		"%{wks.location}/Libraries/glfw/include/**.h",

        "%{wks.location}/Libraries/glm/include/**.hpp",
		"%{wks.location}/Libraries/glm/include/**.inl",

        "%{wks.location}/Libraries/miniaudio/include/**.h",
        "%{wks.location}/Libraries/miniaudio/src/miniaudio.c",

        "%{wks.location}/Libraries/stb_image/include/**.h",
		"%{wks.location}/Libraries/stb_image/include/**.cpp",

        "%{wks.location}/Libraries/spdlog/include/**.h",
		"%{wks.location}/Libraries/spdlog/include/**.cpp",

        -- "%{wks.location}/Projects/Lib-Asset/Include/**.h",
        -- "%{wks.location}/Projects/Lib-Audio/Include/**.h",
        "%{wks.location}/Projects/Lib-Logger/Include/**.h",
        "%{wks.location}/Projects/Lib-Logger/Include/**.cpp",
        -- "%{wks.location}/Projects/Lib-Platform/Include/**.h",
        -- "%{wks.location}/Projects/Lib-Platform/Include/**.cpp",

        "**.h",
        "**.hpp",
        "**.cpp"
    }

    libdirs
    {
        "%{wks.location}/Libraries/glad/lib",
		"%{wks.location}/Libraries/glfw/lib",
		"%{wks.location}/Libraries/glm/lib",
        "%{wks.location}/Libraries/spdlog/lib"
    }

    links
    {
        "Lib-Asset",
        "Lib-Audio",
        "Lib-Logger",
    }

    dependson
    {
        "Lib-Asset",
        "Lib-Audio",
        "Lib-Logger",
    }

    filter { "configurations:Debug" }
		links {
			"glad_debug",
			"glm_debug",
			"glfw3",
			"spdlogd",
		}

	filter { "configurations:Dist" }
		links {
			"glad_dist",
			"glm_dist",
			"glfw3",
			"spdlog",
		}

	filter { "configurations:Release" }
		links {
			"glad_release",
			"glm_release",
			"glfw3",
			"spdlog",
		}

    filter "system:windows"
        systemversion "latest"

        buildoptions { "/FS" }

        defines
        {
            "PLATFORM_WINDOWS"
        }
