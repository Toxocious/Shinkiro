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

        "%{wks.location}/Libraries/stb_image/include",

        "%{wks.location}/Libraries/spdlog/include",

        -- "%{wks.location}/Projects/Lib-Asset/Include",
        "../Lib-Asset/Include",

        "./Include",
    }

    files
    {
        "%{wks.location}/Libraries/glad/include/**.h",
		"%{wks.location}/Libraries/glad/src/glad.c",

		"%{wks.location}/Libraries/glfw/include/**.h",

        "%{wks.location}/Libraries/glm/include/**.hpp",
		"%{wks.location}/Libraries/glm/include/**.inl",

        "%{wks.location}/Libraries/stb_image/include/**.h",
		"%{wks.location}/Libraries/stb_image/include/**.cpp",

        "%{wks.location}/Libraries/spdlog/include/**.h",
		"%{wks.location}/Libraries/spdlog/include/**.cpp",

        -- "%{wks.location}/Projects/Lib-Asset/Include/**.h",
        "../Lib-Asset/Include/**.h",

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
    }

    dependson
    {
        "Lib-Asset",
    }

    filter { "configurations:Debug" }
		links {
			-- "assimp_debug",
			"glad_debug",
			"glm_debug",
			"glfw3",
			-- "imgui_debug",
			"spdlogd",
		}

	filter { "configurations:Dist" }
		links {
			-- "assimp_dist",
			"glad_dist",
			"glm_dist",
			"glfw3",
			-- "imgui_dist",
			"spdlog",
		}

	filter { "configurations:Release" }
		links {
			-- "assimp_release",
			"glad_release",
			"glm_release",
			"glfw3",
			-- "imgui_release",
			"spdlog",
		}

    filter "system:windows"
        systemversion "latest"

        buildoptions { "/FS" }

        defines
        {
            "PLATFORM_WINDOWS"
        }
