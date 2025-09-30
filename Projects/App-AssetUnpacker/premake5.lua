project "AssetUnpacker"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetname "ShinkiroAssetUnpacker"

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
        "{MKDIR} %{wks.location}" .. _G.AssetUnpackerBinaryDir,
    }

    postbuildcommands {
        -- Copy all Lib-* DLLs to the Asset Unpacker executable directory
        copy_if_needed("%{wks.location}" .. _G.CoreAssetPath, "%{wks.location}" .. AssetUnpackerBinaryDir),
        copy_if_needed("%{wks.location}" .. _G.CorePath, "%{wks.location}" .. AssetUnpackerBinaryDir),
    }

    filter { "system:windows" }
        systemversion "latest"
