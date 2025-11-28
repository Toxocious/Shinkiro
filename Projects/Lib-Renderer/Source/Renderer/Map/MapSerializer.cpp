#include <Renderer/_Common.h>

#include <Renderer/Map/MapSerializer.h>

#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>

//

#include <Renderer/Map/MapManager.h>

#include <Core/IApplication.h>

#include <Asset/AssetBundleManager.h>
#include <Audio/AudioManager.h>

#include <Core/Util/Log.h>
//

using json = nlohmann::json;

namespace Shinkiro::Renderer
{
    bool MapSerializer::SaveMap( const std::string & filepath, const MapData & mapData, const std::vector<SceneObject> & objects )
    {
        json j;

        // 1. Meta Data
        j["meta"] = {
            { "name",       mapData.name       },
            { "width",      mapData.width      },
            { "height",     mapData.height     },
            { "tileWidth",  mapData.tileWidth  },
            { "tileHeight", mapData.tileHeight }
        };

        // 2. Tilesets
        j["tilesets"] = json::array();
        for ( const auto & ts : mapData.tilesets )
        {
            j["tilesets"].push_back( {
                { "firstGid",    ts.firstGid    },
                { "imageSource", ts.imageSource },
                { "tileWidth",   ts.tileWidth   },
                { "tileHeight",  ts.tileHeight  },
                { "imageWidth",  ts.imageWidth  },
                { "imageHeight", ts.imageHeight },
                { "columns",     ts.columns     },
                { "rows",        ts.rows        }
            } );
        }

        // 3. Layers
        j["layers"] = json::array();
        for ( const auto & layer : mapData.layers )
        {
            j["layers"].push_back( {
                { "name",    layer.name    },
                { "visible", layer.visible },
                { "data",    layer.data    }
            } );
        }

        // 4. Scene Objects (The 3D stuff!)
        j["objects"] = json::array();
        for ( const auto & obj : objects )
        {
            j["objects"].push_back( {
                { "id",    obj.id                                             },
                { "model", obj.modelName                                      },
                { "pos",   { obj.position.x, obj.position.y, obj.position.z } },
                { "rot",   { obj.rotation.x, obj.rotation.y, obj.rotation.z } },
                { "scale", { obj.scale.x, obj.scale.y, obj.scale.z }          }
            } );
        }

        // 5. Properties
        j["properties"] = json::object();
        for ( const auto & [key, val] : mapData.properties )
        {
            std::visit( [&]( auto && arg )
                        {
                            j["properties"][key] = arg;
                        },
                        val );
        }

        // Write to file
        std::ofstream o( filepath );
        if ( !o.is_open() )
        {
            SHNK_CORE_ERROR( "Failed to open file for writing: {}", filepath );

            return false;
        }

        o << std::setw( 4 ) << j << std::endl;
        SHNK_CORE_INFO( "Map saved successfully to {}", filepath );

        return true;
    }

    bool MapSerializer::LoadMap( const std::string & filepath, MapData & outMapData, std::vector<SceneObject> & outObjects )
    {
        std::ifstream i( filepath );
        if ( !i.is_open() )
        {
            SHNK_CORE_ERROR( "Failed to open file for reading: {}", filepath );
            return false;
        }

        // Read entire file into string
        std::stringstream buffer;
        buffer << i.rdbuf();

        return ParseMap( buffer.str(), outMapData, outObjects );
    }

    bool MapSerializer::ParseMap( const std::string & jsonData, MapData & outMapData, std::vector<SceneObject> & outObjects )
    {
        json j;
        try
        {
            j = json::parse( jsonData );
        }
        catch ( json::parse_error & e )
        {
            SHNK_CORE_ERROR( "JSON Parse Error: {}", e.what() );
            return false;
        }

        // 1. Meta
        outMapData.name       = j["meta"].value( "name", "Untitled" );
        outMapData.width      = j["meta"].value( "width", 0 );
        outMapData.height     = j["meta"].value( "height", 0 );
        outMapData.tileWidth  = j["meta"].value( "tileWidth", 16 );
        outMapData.tileHeight = j["meta"].value( "tileHeight", 16 );

        // 2. Tilesets
        outMapData.tilesets.clear();
        if ( j.contains( "tilesets" ) )
        {
            for ( const auto & tsJson : j["tilesets"] )
            {
                Tileset ts;
                ts.firstGid    = tsJson.value( "firstGid", 1 );
                ts.imageSource = tsJson.value( "imageSource", "" );
                ts.tileWidth   = tsJson.value( "tileWidth", 16 );
                ts.tileHeight  = tsJson.value( "tileHeight", 16 );
                ts.imageWidth  = tsJson.value( "imageWidth", 0 );
                ts.imageHeight = tsJson.value( "imageHeight", 0 );
                ts.columns     = tsJson.value( "columns", 0 );
                ts.rows        = tsJson.value( "rows", 0 );
                outMapData.tilesets.push_back( ts );
            }
        }

        // 3. Layers
        outMapData.layers.clear();
        if ( j.contains( "layers" ) )
        {
            for ( const auto & lJson : j["layers"] )
            {
                TileLayer l;
                l.name    = lJson.value( "name", "Layer" );
                l.visible = lJson.value( "visible", true );
                l.width   = outMapData.width;
                l.height  = outMapData.height;

                if ( lJson.contains( "data" ) )
                {
                    l.data = lJson["data"].get<std::vector<int>>();
                }
                else
                {
                    l.data.resize( l.width * l.height, 0 );
                }

                outMapData.layers.push_back( l );
            }
        }

        // 4. Objects
        outObjects.clear();
        if ( j.contains( "objects" ) )
        {
            for ( const auto & objJson : j["objects"] )
            {
                SceneObject obj;
                obj.id        = objJson.value( "id", 0 );
                obj.modelName = objJson.value( "model", "" );

                // Safe Load Position
                if ( objJson.contains( "pos" ) && objJson["pos"].is_array() && objJson["pos"].size() >= 3 )
                {
                    auto pos     = objJson["pos"];
                    obj.position = glm::vec3( pos[0], pos[1], pos[2] );
                }
                else
                {
                    obj.position = glm::vec3( 0.0f );
                }

                // Safe Load Rotation
                if ( objJson.contains( "rot" ) && objJson["rot"].is_array() && objJson["rot"].size() >= 3 )
                {
                    auto rot     = objJson["rot"];
                    obj.rotation = glm::vec3( rot[0], rot[1], rot[2] );
                }
                else
                {
                    obj.rotation = glm::vec3( 0.0f );
                }

                // Safe Load Scale
                if ( objJson.contains( "scale" ) && objJson["scale"].is_array() && objJson["scale"].size() >= 3 )
                {
                    auto scale = objJson["scale"];
                    obj.scale  = glm::vec3( scale[0], scale[1], scale[2] );
                }
                else
                {
                    obj.scale = glm::vec3( 1.0f );
                }

                outObjects.push_back( obj );
            }
        }

        // 5. Properties
        if ( j.contains( "properties" ) )
        {
            for ( auto & [key, val] : j["properties"].items() )
            {
                if ( val.is_string() )
                {
                    outMapData.properties[key] = val.get<std::string>();
                }
                else if ( val.is_boolean() )
                {
                    outMapData.properties[key] = val.get<bool>();
                }
                else if ( val.is_number_float() )
                {
                    outMapData.properties[key] = val.get<float>();
                }
                else if ( val.is_number_integer() )
                {
                    outMapData.properties[key] = val.get<int>();
                }
            }
        }

        SHNK_CORE_INFO( "Map parsed from memory: {} ({}x{}) with {} objects.", outMapData.name, outMapData.width, outMapData.height, outObjects.size() );

        return true;
    }
}
