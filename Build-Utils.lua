-- Helper function to get current git branch
function getCurrentGitBranch()
    local pipe = io.popen("git rev-parse --abbrev-ref HEAD")
    if pipe then
        local branch = pipe:read("*l")
        pipe:close()

        return branch
    end

    return "unknown"
end

-- Helper function to copy over required directories and DLLs to a destination directory.
function copy_if_needed(src, dst)
    if os.host() == "windows" then
        if path.hasextension(src) then
            return "{COPY} " .. src .. " " .. dst
        else
            return "{COPYDIR} " .. src .. " " .. dst
        end
    end
end

-- Link our dependencies.
function linkDependencies()
    includedirs {
        "%{wks.location}/Libraries/assimp/include",
        "%{wks.location}/Libraries/glad/include",
        "%{wks.location}/Libraries/glfw/include",
        "%{wks.location}/Libraries/glm/include",
        "%{wks.location}/Libraries/imgui/include",
        "%{wks.location}/Libraries/lucide/include",
        "%{wks.location}/Libraries/nlohmann/include",
        "%{wks.location}/Libraries/stb_image/include",
        "%{wks.location}/Libraries/zstd/include",
    }

    libdirs {
        "%{wks.location}/Libraries/assimp/lib",
        "%{wks.location}/Libraries/glad/lib",
        "%{wks.location}/Libraries/glfw/lib",
        "%{wks.location}/Libraries/glm/lib",
        "%{wks.location}/Libraries/imgui/lib",
        "%{wks.location}/Libraries/zstd/lib",
    }

    -- Static linking flags for Assimp
    filter { "kind:not SharedLib", "configurations:Debug" }
        defines { "ASSIMP_STATIC" }
        links {
            "assimp_debug",
            "glad_debug",
            "glm_debug",
            "glfw3",
            "imgui_debug",
            "zstd_static_debug",
        }

    filter { "kind:not SharedLib", "configurations:Release" }
        defines { "ASSIMP_STATIC" }
        links {
            "assimp_release",
            "glad_release",
            "glm_release",
            "glfw3",
            "imgui_release",
            "zstd_static_release",
        }

    filter { "kind:not SharedLib", "configurations:Dist" }
        defines { "ASSIMP_STATIC" }
        links {
            "assimp_dist",
            "glad_dist",
            "glm_dist",
            "glfw3",
            "imgui_dist",
            "zstd_static_release",
        }

    filter {}
end


-- Include the files that we use from our dependencies.
function includeDependencies()
	includedirs {
		"%{wks.location}/Libraries/assimp/include",
		"%{wks.location}/Libraries/glad/include",
		"%{wks.location}/Libraries/glfw/include",
		"%{wks.location}/Libraries/glm/include",
		"%{wks.location}/Libraries/imgui/include",
		"%{wks.location}/Libraries/lucide/include",
        "%{wks.location}/Libraries/nlohmann/include",
		"%{wks.location}/Libraries/stb_image/include",
		"%{wks.location}/Libraries/zstd/include",
	}

	files {
		"%{wks.location}/Libraries/assimp/include/**.h",
		"%{wks.location}/Libraries/assimp/include/**.hpp",
		"%{wks.location}/Libraries/assimp/include/**.inl",

		"%{wks.location}/Libraries/glad/include/**.h",
		"%{wks.location}/Libraries/glad/src/glad.c",

		"%{wks.location}/Libraries/glfw/include/**.h",

		"%{wks.location}/Libraries/glm/include/**.hpp",
		"%{wks.location}/Libraries/glm/include/**.inl",

		"%{wks.location}/Libraries/imgui/include/**.h",
		"%{wks.location}/Libraries/imgui/include/**.cpp",

        "%{wks.location}/Libraries/lucide/include/lucide/IconsLucide.h",

        "Libraries/nlohmann/include/**.hpp",

		"%{wks.location}/Libraries/stb_image/include/**.h",
		"%{wks.location}/Libraries/stb_image/include/**.cpp",

		"%{wks.location}/Libraries/zstd/include/**.h",
	}
end
