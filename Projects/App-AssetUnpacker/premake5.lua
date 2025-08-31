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
        "**.h",
        "**.hpp",
        "**.cpp"
    }

    includedirs
    {
        "../Lib-Asset/Include",

        "./Include",
    }

    links
    {
        "Lib-Asset",
    }

    dependson
    {
        "Lib-Asset",
    }

    prebuildcommands {
        "{MKDIR} %{wks.location}" .. _G.AssetUnpackerBinaryDir,
    }

    postbuildcommands {
        -- Copy all Lib-* DLLs to the Asset Unpacker executable directory
        copy_if_needed("%{wks.location}" .. _G.CoreAssetPath, "%{wks.location}" .. AssetUnpackerBinaryDir),
        copy_if_needed("%{wks.location}" .. _G.CoreLoggerPath, "%{wks.location}" .. AssetUnpackerBinaryDir),
    }

    filter { "system:windows" }
        systemversion "latest"
