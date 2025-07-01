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

        "**.h",
        "**.hpp",
        "**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Libraries/stb_image/include",

        "./Include",
    }

    defines
    {
        "SHINKIRO_ASSET_EXPORTS"
    }

    filter "system:windows"
        systemversion "latest"

        buildoptions { "/FS" }

        defines
        {
            "PLATFORM_WINDOWS"
        }
