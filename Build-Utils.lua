-- Link our dependencies.
function linkDependencies()
	includedirs {
		"%{wks.location}/Libraries/assimp/include",
		"%{wks.location}/Libraries/glad/include",
		"%{wks.location}/Libraries/glfw/include",
		"%{wks.location}/Libraries/glm/include",
		"%{wks.location}/Libraries/imgui/include",
		"%{wks.location}/Libraries/spdlog/include",
		-- "%{wks.location}/Libraries/stb_image/include",
	}

	libdirs {
		"%{wks.location}/Libraries/assimp/lib",
		"%{wks.location}/Libraries/glad/lib",
		"%{wks.location}/Libraries/glfw/lib",
		"%{wks.location}/Libraries/glm/lib",
		"%{wks.location}/Libraries/imgui/lib",
		"%{wks.location}/Libraries/spdlog/lib",
	}

	-- Our static lib should not link against our dependencies
	filter { "kind:not SharedLib", "configurations:Debug" }
		links {
			"assimp_debug",
			"glad_debug",
			"glm_debug",
			"glfw3",
			"imgui_debug",
			"spdlogd",
		}

	filter { "kind:not SharedLib", "configurations:Dist" }
		links {
			"assimp_dist",
			"glad_dist",
			"glm_dist",
			"glfw3",
			"imgui_dist",
			"spdlog",
		}

	filter { "kind:not SharedLib", "configurations:Release" }
		links {
			"assimp_release",
			"glad_release",
			"glm_release",
			"glfw3",
			"imgui_release",
			"spdlog",
		}

end

-- Include the files that we use from our dependencies.
function includeDependencies()
	includedirs {
		"%{wks.location}/Libraries/assimp/include",
		"%{wks.location}/Libraries/glad/include",
		"%{wks.location}/Libraries/glfw/include",
		"%{wks.location}/Libraries/glm/include",
		"%{wks.location}/Libraries/imgui/include",
		"%{wks.location}/Libraries/spdlog/include",
		-- "%{wks.location}/Libraries/stb_image/include",
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

		"%{wks.location}/Libraries/spdlog/include/**.h",

		-- "%{wks.location}/Libraries/stb_image/include/**.h",
		-- "%{wks.location}/Libraries/stb_image/include/**.cpp",
	}
end
