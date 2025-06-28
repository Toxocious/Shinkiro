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
        -- "%{wks.location}/Libraries/stb_image/include",
        "%{wks.location}/Libraries/spdlog/include",

        "./Include",
    }

    files
    {
        -- "%{wks.location}/Libraries/stb_image/include/**.h",
		-- "%{wks.location}/Libraries/stb_image/include/**.cpp",
        "%{wks.location}/Libraries/spdlog/include/**.h",
		"%{wks.location}/Libraries/spdlog/include/**.cpp",

        "**.h",
        "**.hpp",
        "**.cpp"
    }

    libdirs
    {
        "%{wks.location}/Libraries/spdlog/lib"
    }

    filter { "configurations:Debug" }
		links {
			-- "assimp_debug",
			-- "glad_debug",
			-- "glm_debug",
			-- "glfw3",
			-- "imgui_debug",
			"spdlogd",
		}

	filter { "configurations:Dist" }
		links {
			-- "assimp_dist",
			-- "glad_dist",
			-- "glm_dist",
			-- "glfw3",
			-- "imgui_dist",
			"spdlog",
		}

	filter { "configurations:Release" }
		links {
			-- "assimp_release",
			-- "glad_release",
			-- "glm_release",
			-- "glfw3",
			-- "imgui_release",
			"spdlog",
		}

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "PLATFORM_WINDOWS"
        }
