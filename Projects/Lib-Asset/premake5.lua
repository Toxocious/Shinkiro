project "Lib-Asset"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    targetname "ShinkiroAsset"

    targetdir ("../../Build/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../../Build/Artifacts/" .. outputdir .. "/%{prj.name}")

    pchheader "Asset/_Common.h"
    pchsource "Source/Asset/_Common.cpp"

    files
    {
        "%{wks.location}/Libraries/stb_image/include/**.h",
        "%{wks.location}/Libraries/stb_image/include/**.cpp",

		"%{wks.location}/Libraries/zstd/include/**.h",

        "%{wks.location}/Projects/Lib-Logger/Include/**.h",

        "**.h",
        "**.hpp",
        "**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Libraries/stb_image/include",
        "%{wks.location}/Libraries/spdlog/include",

		"%{wks.location}/Libraries/zstd/include",

        "%{wks.location}/Projects/Lib-Logger/Include",

        "./Include",
    }

    libdirs
    {
		"%{wks.location}/Libraries/spdlog/lib",
        "%{wks.location}/Libraries/zstd/lib",
    }

    links
    {
        "Lib-Logger",
    }

    dependson
    {
        "Lib-Logger",
    }

    defines
    {
        "SHINKIRO_ASSET_EXPORTS"
    }

    filter { "files:**/Libraries/stb_image/**.cpp" }
        flags { "NoPCH" }

    filter { "files:**/Libraries/zstd/**.c" }
        flags { "NoPCH" }

    filter {}

    filter { "configurations:Debug" }
		links {
			"zstd_static_debug",
		}

	filter { "configurations:Dist" }
		links {
			"zstd_static_release",
		}

	filter { "configurations:Release" }
		links {
			"zstd_static_release",
		}

    filter "system:windows"
        systemversion "latest"

        buildoptions { "/FS", "/MP" }

        defines
        {
            "PLATFORM_WINDOWS"
        }
