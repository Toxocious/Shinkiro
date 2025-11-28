#pragma once
#ifndef SHINKIRO_RENDERER_MAP_SERIALIZER_H
#    define SHINKIRO_RENDERER_MAP_SERIALIZER_H

#    include <Renderer/Map/MapData.h>

#    include <string>
#    include <vector>

namespace Shinkiro::Renderer
{
    class RENDER_API MapSerializer
    {
    public:
        MapSerializer()  = default;
        ~MapSerializer() = default;

    public:
        /**
         * @brief Saves the map and scene objects to a JSON file.
         * @param filepath The path to save the map file.
         * @param mapData The MapData to save.
         * @param objects The scene objects to save.
         * @return True on success, false on failure.
         */
        bool SaveMap( const std::string & filepath, const MapData & mapData, const std::vector<SceneObject> & objects );

        /**
         * @brief Loads the map and scene objects from a JSON file.
         * @param filepath The path to the map file.
         * @param outMapData The destination MapData struct.
         * @param outObjects The destination vector for scene objects.
         * @return True on success, false on failure.
         */
        bool LoadMap( const std::string & filepath, MapData & outMapData, std::vector<SceneObject> & outObjects );

        /**
         * @brief Parses map data from a JSON string/buffer in memory.
         * @param jsonData The JSON string containing map data.
         * @param outMapData The destination MapData struct.
         * @param outObjects The destination vector for scene objects.
         * @return True on success, false on failure.
         */
        bool ParseMap( const std::string & jsonData, MapData & outMapData, std::vector<SceneObject> & outObjects );
    };
}

#endif
