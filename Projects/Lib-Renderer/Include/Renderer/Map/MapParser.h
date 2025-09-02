#ifndef SHINKIRO_RENDERER_MAP_PARSER_H
#define SHINKIRO_RENDERER_MAP_PARSER_H

#include <Renderer/Util/TinyXML.h>

//
#include <glm/glm.hpp>

//
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace Shinkiro::Renderer
{
    /**
     * @struct Tileset
     * @brief Holds data for a single tileset, including its texture.
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
        unsigned int textureID   = 0;
        std::string  imageSource;
    };

    /**
     * @struct TileLayer
     * @brief Holds the data for a single tile layer from the TMX map.
     */
    struct TileLayer
    {
        std::string      name;
        int              visible;
        int              width  = 0;
        int              height = 0;
        std::vector<int> data; // Contains the Global Tile IDs (GIDs)
    };

    /**
     * @struct MapData
     * @brief Holds all the parsed data for a TMX map.
     */
    struct MapData
    {
        std::string name;

        int width      = 0;
        int height     = 0;
        int tileWidth  = 0;
        int tileHeight = 0;

        int m_VisibleLayerCount = 0;

        std::vector<TileLayer> layers;
        std::vector<Tileset>   tilesets;

        std::map<std::string, std::variant<std::string, int, float, bool>> properties;

        /**
         * @brief Sets the Global Tile ID (GID) for a tile at a specific location on a given layer.
         * @param layerIndex The index of the layer to modify.
         * @param x The x-coordinate of the tile on the map grid.
         * @param y The y-coordinate of the tile on the map grid.
         * @param gid The new Global Tile ID to set.
         * @return True if the tile was set successfully, false if coordinates or layer index were out of bounds.
         */
        bool SetTile( int layerIndex, int x, int y, int gid )
        {
            // Bounds check for the layer index
            if ( layerIndex < 0 || layerIndex >= layers.size() )
            {
                return false;
            }

            TileLayer & layer = layers[layerIndex];

            // Bounds check for the tile coordinates
            if ( x < 0 || x >= layer.width || y < 0 || y >= layer.height )
            {
                return false;
            }

            // Calculate the index in the 1D data vector and set the GID
            layer.data[y * layer.width + x] = gid;

            return true;
        }

        /**
         * @brief Gets the Global Tile ID (GID) for a tile at a specific location on a given layer.
         * @param layerIndex The index of the layer to check.
         * @param x The x-coordinate of the tile on the map grid.
         * @param y The y-coordinate of the tile on the map grid.
         * @return The GID of the tile. Returns 0 if out of bounds or empty.
         */
        int GetTile( int layerIndex, int x, int y ) const
        {
            // Bounds check for the layer index
            if ( layerIndex < 0 || layerIndex >= layers.size() )
            {
                return 0;
            }

            const TileLayer & layer = layers[layerIndex];

            // Bounds check for the tile coordinates
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
            newLayer.visible = 1;

            newLayer.data.assign( this->width * this->height, 0 );
            layers.push_back( newLayer );

            m_VisibleLayerCount++;
        }
    };

    /**
     * @class MapParser
     * @brief Parses a CSV-encoded .tmx file created by the Tiled Map Editor.
     */
    class MapParser
    {
    public:
        /**
         * @brief Parses the specified .tmx file.
         * @param filename The path to the .tmx file.
         * @return A MapData struct containing the parsed map information or an empty MapData on failure.
         */
        MapData Parse( const char * filename );

        /**
         * @brief Parses the specified map from memory.
         * @param filename The byte data of the map.
         * @return A MapData struct containing the parsed map information or an empty MapData on failure.
         */
        MapData ParseFromMemory( const void * data, size_t size, const char * sourcePath );

        /**
         * @brief Saves the current map data to a .tmx file.
         * @param mapData The MapData struct containing the map information to save.
         * @param filename The path to the .tmx file where the map data will be saved.
         */
        bool SaveMap( const MapData & mapData );

    private:
        /**
         * @brief Helper function to parse a <Tileset> element.
         * @param setElement The XML element representing the set.
         * @return A TileSet struct.
         */
        Tileset ParseTileset( tinyxml2::XMLElement * TilesetElement );

        /**
         * @brief Helper function to parse a single <layer> element.
         * @param layerElement The XML element representing the layer.
         * @return A TileLayer struct containing the parsed layer data.
         */
        std::optional<TileLayer> ParseLayer( tinyxml2::XMLElement * layerElement );

    private:
        std::string m_FilePath;
    };
}

#endif
