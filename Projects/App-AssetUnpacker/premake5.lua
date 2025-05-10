project "AssetUnpacker"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

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
        "../Core-Asset/Include",

        "./Include",
    }

    links
    {
        "Core-Asset",
    }

    dependson
    {
        "Core-Asset",
    }

    includeDependencies();
    linkDependencies();

    filter { "system:windows" }
        systemversion "latest"
