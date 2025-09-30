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
        "%{wks.location}/Projects/Lib-Core/Include/**.h",

        "**.h",
        "**.hpp",
        "**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Projects/Lib-Asset/Include",
        "%{wks.location}/Projects/Lib-Core/Include",

        "./Include",
    }

    links
    {
        "Lib-Asset",
        "Lib-Core",
    }

    dependson
    {
        "Lib-Asset",
        "Lib-Core",
    }

    prebuildcommands {
        "{MKDIR} %{wks.location}" .. _G.AssetPackerBinaryDir,
    }

    postbuildcommands {
        -- Copy assets
        copy_if_needed("%{wks.location}" .. _G.AssetsPath, "%{wks.location}" .. AssetPackerBinaryDir .. "\\Assets"),

        -- Copy all Lib-* DLLs to the Asset Packer executable directory
        copy_if_needed("%{wks.location}" .. _G.CoreAssetPath, "%{wks.location}" .. AssetPackerBinaryDir),
        copy_if_needed("%{wks.location}" .. _G.CorePath, "%{wks.location}" .. AssetPackerBinaryDir),
    }

    filter { "system:windows" }
        systemversion "latest"
