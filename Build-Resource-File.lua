-- scripts/genresources.lua

local M = {}

-- helper: convert "1.2.3" -> "1,2,3,0"
local function versionToComma(v)
    local parts = {}
    for p in string.gmatch(v, "%d+") do
        table.insert(parts, p)
    end
    while #parts < 4 do
        table.insert(parts, "0")
    end
    return table.concat(parts, ",")
end

-- generate .rc file from template
function M.generate()
    -- pull metadata from global table
    local meta = _G.appMetadata or {}
    local appVersion  = meta.version  or "1.0.0"
    local companyName = meta.company  or "My Company"
    local productName = meta.product  or "My Product"
    local exeName     = meta.exe      or "MyApp.exe"

    local inputFile  = "Assets/Resources.rc.template"
    local outputFile = "Assets/Resources.rc"

    local infile = io.open(inputFile, "r")
    if not infile then
        print("Error: missing " .. inputFile)
        return
    end
    local content = infile:read("*all")
    infile:close()

    local vars = {
        APP_VERSION       = appVersion,
        APP_VERSION_COMMA = versionToComma(appVersion),
        COMPANY_NAME      = companyName,
        PRODUCT_NAME      = productName,
        EXE_NAME          = exeName
    }

    for k,v in pairs(vars) do
        content = content:gsub("@" .. k .. "@", v)
    end

    local outfile = io.open(outputFile, "w+")
    outfile:write(content)
    outfile:close()

    print("Generated " .. outputFile .. " with version " .. appVersion)
end

-- Register Premake action for manual use
newaction {
    trigger     = "genresources",
    description = "Generate Windows resource file from template",
    execute = function ()
        M.generate()
    end
}

return M
