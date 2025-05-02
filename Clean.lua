require('os')
require('premake')

-- Custom clean action
newaction {
    trigger = "clean",
    description = "Clean all build artifacts and generated files",
    execute = function()
        print("Cleaning build artifacts...")

        -- Directories to clean
        local dirs = {
            ".vs",
            "Build",
            "Generated"
        }

        -- Files to delete
        local files = {
            "**.sln",
            "**.vcxproj",
            "**.vcxproj.filters",
            "**.vcxproj.user",
            "**/*.make",
            "**/*.make",
            "**/Makefile"
        }

        -- Remove directories
        for _, dir in ipairs(dirs) do
            print("Removing " .. dir .. "...")
            os.rmdir(dir)
        end

        -- Remove files
        for _, file in ipairs(files) do
            print("Removing " .. file .. "...")
            os.remove(os.matchfiles(file))
        end

        print("Clean completed!")
    end
}
