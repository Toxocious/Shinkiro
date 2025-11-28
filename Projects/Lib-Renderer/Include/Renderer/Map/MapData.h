#pragma once
#ifndef SHINKIRO_RENDERER_MAP_DATA_H
#    define SHINKIRO_RENDERER_MAP_DATA_H

#    include <Renderer/_Defs.h>

#    include <glm/glm.hpp>

#    include <map>
#    include <string>
#    include <variant>
#    include <vector>

namespace Shinkiro::Renderer
{
    /**
     * @struct SceneObject
     * @brief Holds data for a single scene object.
     */
    struct SceneObject
    {
        std::string modelName; // Key to find the Model* in assets
        glm::vec3   position;
        glm::vec3   rotation;
        glm::vec3   scale;
        int         id;
    };

    /**
     * @struct Tileset
     * @brief Holds data for a single tileset.
     */
    struct Tileset
    {
        int          firstGid    = 0;
        int          tileWidth   = 0;
        int          tileHeight  = 0;
        int          imageWidth  = 0;
        int          imageHeight = 0;
        int          columns     = 0;
        int          rows        = 0;
        std::string  imageSource;
        unsigned int textureID = 0;
    };

    /**
     * @struct TileLayer
     * @brief Holds the data for a single tile layer.
     */
    struct TileLayer
    {
        std::string name;
        int         width   = 0;
        int         height  = 0;
        bool        visible = true;

        std::vector<int> data; // Contains the Global Tile IDs (GIDs)
    };

    /**
     * @struct MapData
     * @brief Holds all the data for a map.
     */
    struct MapData
    {
        std::string name;

        int width      = 0;
        int height     = 0;
        int tileWidth  = 0;
        int tileHeight = 0;

        int m_visibleLayerCount = 0;

        std::vector<TileLayer> layers;
        std::vector<Tileset>   tilesets;

        std::map<std::string, std::variant<std::string, int, float, bool>> properties;

        /**
         * @brief Sets the tile GID at the specified layer and coordinates.
         * @return True if successful, false if out of bounds.
         */
        bool SetTile( int layerIndex, int x, int y, int gid )
        {
            if ( layerIndex < 0 || layerIndex >= layers.size() )
            {
                return false;
            }

            TileLayer & layer = layers[layerIndex];
            if ( x < 0 || x >= layer.width || y < 0 || y >= layer.height )
            {
                return false;
            }

            layer.data[y * layer.width + x] = gid;

            return true;
        }

        /**
         * @brief Gets the tile GID at the specified layer and coordinates.
         * @return The tile GID, or 0 if out of bounds.
         */
        int GetTile( int layerIndex, int x, int y ) const
        {
            if ( layerIndex < 0 || layerIndex >= layers.size() )
            {
                return 0;
            }

            const TileLayer & layer = layers[layerIndex];
            if ( x < 0 || x >= layer.width || y < 0 || y >= layer.height )
            {
                return 0;
            }

            return layer.data[y * layer.width + x];
        }

        /**
         * @brief Adds a new, empty layer to the map.
         * @param name The name for the new layer.
         */
        void AddLayer( const std::string & name )
        {
            TileLayer newLayer;
            newLayer.name    = name;
            newLayer.width   = this->width;
            newLayer.height  = this->height;
            newLayer.visible = true;

            newLayer.data.assign( this->width * this->height, 0 );
            layers.push_back( newLayer );

            m_visibleLayerCount++;
        }

        /**
         * @brief Gets a specific map property by name.
         * @tparam T The expected type of the property (std::string, int, float, or bool).
         * @param name The name of the property to retrieve.
         * @return An std::optional containing the property value if found and of the correct type, otherwise std::nullopt.
         */
        template <typename T>
        std::optional<T> GetProperty( const std::string & name ) const
        {
            auto it = properties.find( name );
            if ( it != properties.end() )
            {
                if ( std::holds_alternative<T>( it->second ) )
                {
                    return std::get<T>( it->second );
                }
            }
            return std::nullopt;
        }
    };
}

#endif
