#ifndef SHINKIRO_RENDERER_MAP_MANAGER_H
#define SHINKIRO_RENDERER_MAP_MANAGER_H

#include <Renderer/_Defs.h>

#include <Renderer/Map/MapParser.h>
#include <Renderer/Map/MapRenderer.h>

#include <unordered_map>

namespace Shinkiro::Renderer
{
    class RENDER_API MapManager
    {
    public:
        MapManager();
        ~MapManager();

    public:
    private:
        // Contains a list of all valid and available maps.
        std::unordered_map<std::string, MapData> m_Maps;

        // Contains a list of all maps that have been loaded by the client.
        std::unordered_map<std::string, MapData> m_CachedMaps;
    };
}

#endif
