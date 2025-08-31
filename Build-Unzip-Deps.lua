local sevenZipPath = "C:\\Progra~1\\7-Zip\\7z.exe"

newaction {
    trigger = "unzip-deps",
    description = "Unzip all lib.zip and lib.7z files in all libraries, skipping if 'lib' exists",

    execute = function()
        print("Unzipping dependency archives...")

        local archiveFiles = os.matchfiles("Libraries/*/lib.7z")
        print("Found archives: ", #archiveFiles)

        for _, archiveFile in ipairs(archiveFiles) do
            local libDir = path.getdirectory(archiveFile)
            local targetDir = libDir .. "/lib"

            if os.isdir(targetDir) then
                print("Skipping " .. archiveFile .. " because " .. targetDir .. " already exists")
            else
                print("Unzipping " .. archiveFile .. " to " .. libDir)

                local cmd = sevenZipPath .. ' x "' .. archiveFile .. '" -o"' .. libDir .. '" -y > nul 2>&1'
                os.execute(cmd)
            end
        end

        print("Done.")
    end
}
