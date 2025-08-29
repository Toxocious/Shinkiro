project "AssetPacker"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetname "ShinkiroAssetPacker"

    targetdir ("../../Build/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../../Build/Artifacts/" .. outputdir .. "/%{prj.name}")

    entrypoint "mainCRTStartup"

    files
    {
        "%{wks.location}/Projects/Lib-Asset/Include/**.h",
        "%{wks.location}/Projects/Lib-Logger/Include/**.h",

        "**.h",
        "**.hpp",
        "**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Projects/Lib-Asset/Include",
        "%{wks.location}/Projects/Lib-Logger/Include",

        "./Include",
    }

    links
    {
        "Lib-Asset",
        "Lib-Logger",
    }

    dependson
    {
        "Lib-Asset",
        "Lib-Logger",
    }

    filter { "system:windows" }
        systemversion "latest"
