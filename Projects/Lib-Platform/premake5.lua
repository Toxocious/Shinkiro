project "Lib-Platform"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    targetname "ShinkiroPlatform"

    targetdir ("../../Build/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../../Build/Artifacts/" .. outputdir .. "/%{prj.name}")

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

        "%{wks.location}/Projects/Lib-Asset/Include/**.h",
        "%{wks.location}/Projects/Lib-Audio/Include/**.h",
        "%{wks.location}/Projects/Lib-Core/Include/**.h",
        "%{wks.location}/Projects/Lib-Logger/Include/**.h",

        "**.h",
        "**.hpp",
        "**.cpp"
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
        "%{wks.location}/Projects/Lib-Core/Include",
        "%{wks.location}/Projects/Lib-Logger/Include",

        "./Include",
    }

    libdirs
    {
        "%{wks.location}/Libraries/glad/lib",
		"%{wks.location}/Libraries/glfw/lib",
		"%{wks.location}/Libraries/glm/lib",
		"%{wks.location}/Libraries/spdlog/lib",
    }

    links
    {
        "Lib-Audio",
        "Lib-Asset",
        "Lib-Core",
        "Lib-Logger",
    }

    dependson
    {
        "Lib-Audio",
        "Lib-Asset",
        "Lib-Core",
        "Lib-Logger",
    }

    defines
    {
        "SHINKIRO_PLATFORM_EXPORTS"
    }

    filter { "configurations:Debug" }
		links {
			"glad_debug",
			"glfw3",
			"glm_debug",
		}

	filter { "configurations:Dist" }
		links {
			"glad_dist",
			"glfw3",
			"glm_dist",
		}

	filter { "configurations:Release" }
		links {
			"glad_release",
			"glfw3",
			"glm_release",
		}

    filter "system:windows"
        systemversion "latest"

        buildoptions { "/FS" }

        defines
        {
            "PLATFORM_WINDOWS"
        }
