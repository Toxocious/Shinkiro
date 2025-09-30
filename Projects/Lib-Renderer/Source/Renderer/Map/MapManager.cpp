#include <Renderer/_Common.h>

#include <Renderer/Map/MapManager.h>

#include <Core/IApplication.h>

#include <Asset/AssetBundleManager.h>
#include <Audio/AudioManager.h>

#include <Core/Util/Log.h>

namespace Shinkiro::Renderer
{
    MapManager::MapManager()
        : m_Renderer( std::make_unique<MapRenderer>() )
    {
    }

    MapManager::~MapManager()
    {
    }

    bool MapManager::Initialize()
    {
        SHNK_CORE_INFO( "Initializing Map Manager" );
        return m_Renderer->Initialize();
    }

    void MapManager::Shutdown()
    {
        SHNK_CORE_INFO( "Shutting down Map Manager" );
        ClearCache();
        m_Renderer->Shutdown();
    }

    bool MapManager::LoadMap( const std::string & mapPath )
    {
        const auto MapAssetData = Shinkiro::Core::App->GetBundleManager().GetAssetData( mapPath );

        if ( MapAssetData.empty() )
        {
            SHNK_CORE_ERROR( "Failed to load map asset data: {}", mapPath );
            return false;
        }

        std::filesystem::path path( mapPath );
        std::string           mapName = path.stem().string();

        if ( m_CachedMaps.contains( mapPath ) )
        {
            SHNK_CORE_INFO( "Map '{}' is already cached.", mapPath );
            return true;
        }

        SHNK_CORE_INFO( "Parsing and caching new map: {}", mapPath );

        MapData mapData = m_Parser.ParseFromMemory( MapAssetData.data(), MapAssetData.size(), mapPath.c_str() );
        if ( mapData.name.empty() )
        {
            SHNK_CORE_ERROR( "Failed to parse map file: {}", mapPath );
            return false;
        }

        m_Renderer->SetMapData( mapData );
        m_Renderer->LoadTilesetTextures( mapData );
        m_CachedMaps[mapPath] = std::move( mapData );

        SetActiveMap( mapPath );
        LoadBGM();

        return true;
    }

    void MapManager::LoadBGM()
    {
        if ( !m_ActiveMap )
        {
            return;
        }

        if ( auto mapOST = m_ActiveMap->GetProperty<std::string>( "MapMusic" ) )
        {
            const auto bgmData = Shinkiro::Core::App->GetBundleManager().GetAssetData( "Audio/BGM/" + *mapOST );
            if ( !bgmData.empty() )
            {
                SHNK_CORE_INFO( "Loaded map BGM: '{}'", *mapOST );

                Shinkiro::Core::App->GetAudioManager().PlayOST( bgmData );
                Shinkiro::Core::App->GetAudioManager().SetOSTVolume( 0.1f );
            }
            else
            {
                SHNK_CORE_WARN( "Map OST asset not found: {}", *mapOST );
            }
        }
    }

    void MapManager::SetActiveMap( const std::string & mapPath )
    {
        auto it = m_CachedMaps.find( mapPath );
        if ( it != m_CachedMaps.end() )
        {
            m_ActiveMap = &it->second;
            SHNK_CORE_INFO( "Active map set to: {}", mapPath );
        }
        else
        {
            SHNK_CORE_ERROR( "Failed to set active map. Map '{}' not found.", mapPath );
        }
    }

    void MapManager::DeactivateMap()
    {
        if ( m_ActiveMap )
        {
            SHNK_CORE_INFO( "Deactivating map: {}", m_ActiveMap->name );
            m_ActiveMap = nullptr;
        }
    }

    void MapManager::EvictFromCache( const std::string & mapPath )
    {
        auto it = m_CachedMaps.find( mapPath );
        if ( it == m_CachedMaps.end() )
        {
            SHNK_CORE_WARN( "Attempted to evict non-existent map '{}' from cache.", mapPath );
            return;
        }

        if ( m_ActiveMap && m_ActiveMap->name == mapPath )
        {
            m_ActiveMap = nullptr;
        }

        SHNK_CORE_INFO( "Evicting map '{}' from cache.", mapPath );
        m_Renderer->UnloadTilesetTextures( it->second );
        m_CachedMaps.erase( it );
    }

    void MapManager::ClearCache()
    {
        SHNK_CORE_INFO( "Clearing map cache." );
        for ( auto const & [name, mapData] : m_CachedMaps )
        {
            m_Renderer->UnloadTilesetTextures( mapData );
        }
        m_ActiveMap = nullptr;
        m_CachedMaps.clear();
    }

    void MapManager::Render( const Camera & camera, int screenWidth, int screenHeight )
    {
        if ( !m_ActiveMap )
        {
            return;
        }

        glm::mat4 view       = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix( screenWidth, screenHeight );

        m_Renderer->DrawMap( *m_ActiveMap, camera, view, projection );
        // m_Renderer->DrawGrid( m_ActiveMap->width, m_ActiveMap->height, 0.0f, view, projection );
        m_Renderer->DrawSkybox( view, projection );
    }

    void MapManager::Update()
    {
        // Handle any per-frame map logic here, e.g., animations, entity updates.
    }

    MapData * MapManager::GetActiveMap()
    {
        return m_ActiveMap;
    }

    bool MapManager::SaveActiveMap()
    {
        if ( !m_ActiveMap )
        {
            SHNK_CORE_WARN( "No active map to save." );
            return false;
        }

        return m_Parser.SaveMap( *m_ActiveMap );
    }
}
