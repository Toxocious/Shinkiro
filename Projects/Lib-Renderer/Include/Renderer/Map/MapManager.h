#ifndef SHINKIRO_RENDERER_MAP_MANAGER_H
#define SHINKIRO_RENDERER_MAP_MANAGER_H

//
// #include <Core/Camera.h>

#include <Renderer/Camera/Camera.h>
#include <Renderer/Map/MapParser.h>
#include <Renderer/Util/Shader.h>

//
#include <glm/glm.hpp>

//
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace Shinkiro::Renderer
{
    /**
     * @class Renderer
     * @brief Handles all OpenGL rendering for the map.
     *
     * This class sets up shaders, buffers, and draws the map data as a series of cubes.
     */
    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        /**
         * @brief Initializes OpenGL resources needed for rendering.
         * @return True on success, false on failure.
         */
        bool init();

        /**
         * @brief Loads tileset textures.
         * @param mapData The parsed map data containing tileset information.
         */
        void loadTilesetTextures( MapData & mapData );

        /**
         * @brief Draws a highlight at the specified tile coordinates.
         * @param x The X coordinate of the tile to highlight.
         * @param y The Y coordinate of the tile to highlight.
         * @param layer_y_offset The Y offset for the layer to draw the highlight on.
         * @param view The view matrix from the camera.
         * @param projection The projection matrix from the camera.
         */
        void DrawHighlight( int x, int y, float layer_y_offset, const glm::mat4 & view, const glm::mat4 & projection );

        /**
         * @brief Renders the entire map.
         * @param mapData The parsed map data to render.
         * @param view The view matrix from the camera.
         * @param projection The projection matrix from the camera.
         */
        // void drawMap( const MapData & mapData, const Camera & camera, const glm::mat4 & view, const glm::mat4 & projection );
        void drawMap( MapData & mapData, const Camera & camera, const glm::mat4 & view, const glm::mat4 & projection );

        /**
         * @brief Finds the appropriate tileset for a given GID.
         * @param gid The Global Tile ID to find the tileset for.
         * @param tilesets The list of available tilesets.
         * @return A pointer to the Tileset that contains the GID, or nullptr if not found.
         */
        const Tileset * findTilesetForGid( int gid, const std::vector<Tileset> & tilesets ) const;

        /**
         * @brief Draws the skybox.
         * @param view The view matrix from the camera.
         * @param projection The projection matrix from the camera.
         */
        void drawSkybox( const glm::mat4 & view, const glm::mat4 & projection );

        /**
         * @brief Draws a 3D grid on the map's base plane.
         * @param mapWidth The width of the map.
         * @param mapHeight The height of the map.
         * @param view The view matrix from the camera.
         * @param projection The projection matrix from the camera.
         */
        void drawGrid( int mapWidth, int mapHeight, float layer_y_offset, const glm::mat4 & view, const glm::mat4 & projection );

    public:
        int getRenderedTileCount() const
        {
            return m_renderedTileCount;
        }

    private:
        unsigned int loadCubemap( std::vector<std::string> faces );

    private:
        int m_renderedTileCount = 0;

        int  m_visibleLayerCount = 0;
        bool m_layersCounted     = false;

        // Map Tile Shader
        std::unique_ptr<Shader> m_MapTileShader;

        unsigned int m_cubeVAO = 0;
        unsigned int m_cubeVBO = 0;

        // Map Tile Highlighting Shader
        std::unique_ptr<Shader> m_MapTileHighlighterShader;

        // Map Skybox Shader
        std::unique_ptr<Shader> m_MapSkyboxShader;

        unsigned int m_skyboxVAO      = 0;
        unsigned int m_skyboxVBO      = 0;
        unsigned int m_cubemapTexture = 0;

    private:
        static constexpr float m_SkyboxVertices[] = {
            // positions
            -1.0f,
            1.0f,
            -1.0f,
            -1.0f,
            -1.0f,
            -1.0f,
            1.0f,
            -1.0f,
            -1.0f,
            1.0f,
            -1.0f,
            -1.0f,
            1.0f,
            1.0f,
            -1.0f,
            -1.0f,
            1.0f,
            -1.0f,

            -1.0f,
            -1.0f,
            1.0f,
            -1.0f,
            -1.0f,
            -1.0f,
            -1.0f,
            1.0f,
            -1.0f,
            -1.0f,
            1.0f,
            -1.0f,
            -1.0f,
            1.0f,
            1.0f,
            -1.0f,
            -1.0f,
            1.0f,

            1.0f,
            -1.0f,
            -1.0f,
            1.0f,
            -1.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            -1.0f,
            1.0f,
            -1.0f,
            -1.0f,

            -1.0f,
            -1.0f,
            1.0f,
            -1.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            -1.0f,
            1.0f,
            -1.0f,
            -1.0f,
            1.0f,

            -1.0f,
            1.0f,
            -1.0f,
            1.0f,
            1.0f,
            -1.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            -1.0f,
            1.0f,
            1.0f,
            -1.0f,
            1.0f,
            -1.0f,

            -1.0f,
            -1.0f,
            -1.0f,
            -1.0f,
            -1.0f,
            1.0f,
            1.0f,
            -1.0f,
            -1.0f,
            1.0f,
            -1.0f,
            -1.0f,
            -1.0f,
            -1.0f,
            1.0f,
            1.0f,
            -1.0f,
            1.0f
        };

        static constexpr float m_TileVertices[] = {
            // Back face
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            0.0f,
            0.5f,
            -0.5f,
            -0.5f,
            1.0f,
            0.0f,
            0.5f,
            0.5f,
            -0.5f,
            1.0f,
            1.0f,
            0.5f,
            0.5f,
            -0.5f,
            1.0f,
            1.0f,
            -0.5f,
            0.5f,
            -0.5f,
            0.0f,
            1.0f,
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            0.0f,
            // Front face
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            0.5f,
            -0.5f,
            0.5f,
            1.0f,
            0.0f,
            0.5f,
            0.5f,
            0.5f,
            1.0f,
            1.0f,
            0.5f,
            0.5f,
            0.5f,
            1.0f,
            1.0f,
            -0.5f,
            0.5f,
            0.5f,
            0.0f,
            1.0f,
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            // etc. for all 6 faces
            -0.5f,
            0.5f,
            0.5f,
            1.0f,
            0.0f,
            -0.5f,
            0.5f,
            -0.5f,
            1.0f,
            1.0f,
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            1.0f,
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            1.0f,
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            -0.5f,
            0.5f,
            0.5f,
            1.0f,
            0.0f,

            0.5f,
            0.5f,
            0.5f,
            1.0f,
            0.0f,
            0.5f,
            0.5f,
            -0.5f,
            1.0f,
            1.0f,
            0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            1.0f,
            0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            1.0f,
            0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            0.5f,
            0.5f,
            0.5f,
            1.0f,
            0.0f,

            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            1.0f,
            0.5f,
            -0.5f,
            -0.5f,
            1.0f,
            1.0f,
            0.5f,
            -0.5f,
            0.5f,
            1.0f,
            0.0f,
            0.5f,
            -0.5f,
            0.5f,
            1.0f,
            0.0f,
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            1.0f,

            -0.5f,
            0.5f,
            -0.5f,
            0.0f,
            1.0f,
            0.5f,
            0.5f,
            -0.5f,
            1.0f,
            1.0f,
            0.5f,
            0.5f,
            0.5f,
            1.0f,
            0.0f,
            0.5f,
            0.5f,
            0.5f,
            1.0f,
            0.0f,
            -0.5f,
            0.5f,
            0.5f,
            0.0f,
            0.0f,
            -0.5f,
            0.5f,
            -0.5f,
            0.0f,
            1.0f
        };
    };
}

#endif
