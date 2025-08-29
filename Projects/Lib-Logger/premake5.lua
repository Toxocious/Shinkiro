project "Lib-Logger"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    targetname "ShinkiroLogger"

    targetdir ("../../Build/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../../Build/Artifacts/" .. outputdir .. "/%{prj.name}")

    pchheader "Log/_Common.h"
    pchsource "Source/Log/_Common.cpp"

    defines
    {
        "SHINKIRO_LOGGER_EXPORTS"
    }

    includedirs
    {
        "%{wks.location}/Libraries/spdlog/include",

        "./Include",
    }

    files
    {

        "%{wks.location}/Libraries/spdlog/include/**.h",

        "**.h",
        "**.hpp",
        "**.cpp",
    }

    libdirs
    {
        "%{wks.location}/Libraries/spdlog/lib",
    }

    filter { "files:**/Libraries/spdlog/**.cpp" }
        flags { "NoPCH" }

    filter {}

    filter { "configurations:Debug" }
		links {
			"spdlogd",
		}

	filter { "configurations:Dist" }
		links {
			"spdlog",
		}

	filter { "configurations:Release" }
		links {
			"spdlog",
		}

    filter "system:windows"
        systemversion "latest"

        buildoptions { "/FS", "/MP" }

        defines
        {
            "PLATFORM_WINDOWS"
        }
