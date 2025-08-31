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

    prebuildcommands {
        "{MKDIR} %{wks.location}" .. _G.AssetPackerBinaryDir,
    }

    postbuildcommands {
        -- Copy assets
        copy_if_needed("%{wks.location}" .. _G.AssetsPath, "%{wks.location}" .. AssetPackerBinaryDir .. "\\Assets"),

        -- Copy all Lib-* DLLs to the Asset Packer executable directory
        copy_if_needed("%{wks.location}" .. _G.CoreAssetPath, "%{wks.location}" .. AssetPackerBinaryDir),
        copy_if_needed("%{wks.location}" .. _G.CoreLoggerPath, "%{wks.location}" .. AssetPackerBinaryDir),
    }

    filter { "system:windows" }
        systemversion "latest"
