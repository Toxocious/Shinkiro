#ifndef SHINKIRO_RENDERER_MAP_MANAGER_H
#define SHINKIRO_RENDERER_MAP_MANAGER_H

#include <Renderer/_Defs.h>

#include <Renderer/Camera/Camera.h>

#include <Renderer/Map/MapParser.h>
#include <Renderer/Map/MapRenderer.h>

#include <memory>
#include <string>
#include <unordered_map>

namespace Shinkiro::Renderer
{
    class RENDER_API MapManager
    {
    public:
        MapManager();
        ~MapManager();

        bool Initialize();
        void Shutdown();

        bool LoadMap( const std::string & mapPath );
        void SetActiveMap( const std::string & mapName );
        void DeactivateMap();

        void EvictFromCache( const std::string & mapName );
        void ClearCache();

        void Render( const Shinkiro::Renderer::Camera & camera, int screenWidth, int screenHeight );
        void Update();

        MapData * GetActiveMap();
        bool      SaveActiveMap();

    private:
        std::unique_ptr<Shinkiro::Renderer::MapRenderer> m_Renderer;
        Shinkiro::Renderer::MapParser                    m_Parser;

        std::unordered_map<std::string, MapData> m_CachedMaps;
        Shinkiro::Renderer::MapData *            m_ActiveMap = nullptr;
    };
}

#endif
