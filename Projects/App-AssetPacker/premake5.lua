project "AssetPacker"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetname "ShinkiroAssetPacker"

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

    includeDependencies();
    linkDependencies();

    filter { "system:windows" }
        systemversion "latest"
