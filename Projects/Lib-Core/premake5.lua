project "Lib-Core"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"

    targetname "ShinkiroCore"

    targetdir ("../../Build/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../../Build/Artifacts/" .. outputdir .. "/%{prj.name}")

    files
    {
        -- "%{wks.location}/Libraries/stb_image/include/**.h",
		-- "%{wks.location}/Libraries/stb_image/include/**.cpp",

        "**.h",
        "**.hpp",
        "**.cpp"
    }

    includedirs
    {
        -- "%{wks.location}/Libraries/stb_image/include",

        "./Include",
    }

    defines
    {
        "CORE_EXPORTS"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "PLATFORM_WINDOWS"
        }
