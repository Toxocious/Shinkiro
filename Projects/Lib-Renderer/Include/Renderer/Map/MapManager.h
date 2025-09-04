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

        /**
         * @brief Initializes the Map Manager and its resources.
         */
        bool Initialize();

        /**
         * @brief Shuts down the Map Manager and cleans up resources.
         */
        void Shutdown();

        /**
         * @brief Updates the map manager state.
         */
        void Update();

        /**
         * @brief Renders the active map using the provided camera and screen dimensions.
         * @param camera The camera to use for rendering.
         * @param screenWidth The width of the screen.
         * @param screenHeight The height of the screen.
         */
        void Render( const Shinkiro::Renderer::Camera & camera, int screenWidth, int screenHeight );

        /**
         * @brief Loads and caches a map from the specified file path.
         * @param mapPath The file path to the map file.
         * @return True if the map was loaded and cached successfully, false otherwise.
         */
        bool LoadMap( const std::string & mapPath );

        /**
         * @brief Loads the BGM for the map.
         */
        void LoadBGM();

        /**
         * @brief Sets the active map by name.
         * @param mapName The name of the map to set as active.
         */
        void SetActiveMap( const std::string & mapName );

        /**
         * @brief Deactivates the currently active map.
         */
        void DeactivateMap();

        /**
         * @brief Removes a map from the loaded maps cache.
         * @param mapName The name of the map to evict from the cache.
         */
        void EvictFromCache( const std::string & mapName );

        /**
         * @brief Clears all cached maps.
         */
        void ClearCache();

        /**
         * @brief Retrieves a pointer to the currently active map.
         * @return A pointer to the active MapData, or nullptr if no map is active.
         */
        MapData * GetActiveMap();

        /**
         * @brief Saves the currently active map back to its file.
         * @return True if the map was saved successfully, false otherwise.
         */
        bool SaveActiveMap();

    private:
        std::unique_ptr<Shinkiro::Renderer::MapRenderer> m_Renderer;
        Shinkiro::Renderer::MapParser                    m_Parser;

        std::unordered_map<std::string, MapData> m_CachedMaps;
        Shinkiro::Renderer::MapData *            m_ActiveMap = nullptr;
    };
}

#endif
