workspace "ImGui"
    language "C++"
    architecture "x86_64"
    location "Generated"

    flags
    {
        "MultiProcessorCompile"
    }

    configurations { "Debug", "Release", "Dist" }

    -- Binary and Object paths for the compiled/built project.
    targetdir ("Build/Binaries/%{prj.name}/%{cfg.longname}")
    objdir ("Build/Artifacts/%{prj.name}/%{cfg.longname}")

    project "ImGui"
        kind "StaticLib"

        files
        {
            "include/imconfig.h",
            "include/imgui.h",
            "include/imgui.cpp",
            "include/imgui_draw.cpp",
            "include/imgui_internal.h",
            "include/imgui_widgets.cpp",
            "include/imgui_tables.cpp",

            "include/imstb_rectpack.h",
            "include/imstb_textedit.h",
            "include/imstb_truetype.h",

            "include/imgui_demo.cpp"
        }

        filter { "system:windows" }
            systemversion "latest"
            cppdialect "C++17"

        -- Debug Configuration
        --      Doesn't optimize or strip any symbols.
        --      Should obviously not be released to the public.
        filter { "configurations:Debug" }
            defines {
                "_CRT_SECURE_NO_WARNINGS",
            }
            runtime "Debug"
            optimize "Debug"
            symbols "Full"

        -- Release Configuration
        --      Does optimize, but doesn't strip any symbols.
        --      Should not be released to the public.
        filter { "configurations:Release" }
            defines {
                "_CRT_SECURE_NO_WARNINGS",
            }
            runtime "Release"
            optimize "Off"
            symbols "On"

        -- Distributable Configuration
        --      Optimizes and strips out all symbols.
        --      Should be released to the public.
        filter { "configurations:Dist" }
            defines {
                "_CRT_SECURE_NO_WARNINGS",
            }
            runtime "Release"
            optimize "Off"
            symbols "Off"

        filter {}
