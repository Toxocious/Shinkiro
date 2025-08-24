project "Lib-Asset"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    targetname "ShinkiroAsset"

    targetdir ("../../Build/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../../Build/Artifacts/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{wks.location}/Libraries/stb_image/include/**.h",
		"%{wks.location}/Libraries/stb_image/include/**.cpp",

		"%{wks.location}/Libraries/zstd/include/**.h",

        "**.h",
        "**.hpp",
        "**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Libraries/stb_image/include",

		"%{wks.location}/Libraries/zstd/include",

        "./Include",
    }

    libdirs
    {
        "%{wks.location}/Libraries/zstd/lib",
    }

    defines
    {
        "SHINKIRO_ASSET_EXPORTS"
    }

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

        buildoptions { "/FS" }

        defines
        {
            "PLATFORM_WINDOWS"
        }
