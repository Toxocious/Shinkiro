#include <Renderer/_Common.h>

#include <Renderer/Map/MapParser.h>

#include <Core/IApplication.h>

#include <Asset/AssetBundleManager.h>

#include <Log/Log.h>

namespace Shinkiro::Renderer
{
    MapData MapParser::Parse( const char * filePath )
    {
        using namespace tinyxml2;

        MapData               mapData;
        tinyxml2::XMLDocument doc;

        if ( doc.LoadFile( filePath ) != XML_SUCCESS )
        {
            SHNK_CORE_ERROR( "Error loading map TMX file: {}", filePath );
            return mapData;
        }

        m_FilePath   = filePath;
        mapData.name = std::filesystem::path( filePath ).stem().string();

        XMLElement * mapElement = doc.RootElement();
        if ( !mapElement )
        {
            SHNK_CORE_ERROR( "Error: Could not find root <map> element in {}", filePath );
            return mapData;
        }

        mapData.width      = mapElement->IntAttribute( "width" );
        mapData.height     = mapElement->IntAttribute( "height" );
        mapData.tileWidth  = mapElement->IntAttribute( "tilewidth" );
        mapData.tileHeight = mapElement->IntAttribute( "tileheight" );

        XMLElement * propertiesElement = mapElement->FirstChildElement( "properties" );
        if ( propertiesElement )
        {
            for ( XMLElement * propertyElement = propertiesElement->FirstChildElement( "property" );
                  propertyElement != nullptr;
                  propertyElement = propertyElement->NextSiblingElement( "property" ) )
            {
                const char * propName     = propertyElement->Attribute( "name" );
                const char * propValueStr = propertyElement->Attribute( "value" );
                const char * propTypeStr  = propertyElement->Attribute( "type" );

                if ( propName && propValueStr )
                {
                    std::string type = propTypeStr ? propTypeStr : "string";

                    if ( type == "int" )
                    {
                        mapData.properties[propName] = std::stoi( propValueStr );
                    }
                    else if ( type == "float" )
                    {
                        mapData.properties[propName] = std::stof( propValueStr );
                    }
                    else if ( type == "bool" )
                    {
                        mapData.properties[propName] = ( std::string( propValueStr ) == "true" );
                    }
                    else
                    {
                        mapData.properties[propName] = std::string( propValueStr );
                    }
                }
            }
        }

        SHNK_CORE_INFO( "Map Loaded From File: {}", filePath );
        SHNK_CORE_INFO( "Map Name: {}", mapData.name );
        SHNK_CORE_INFO( "Parsed {} Map Properties.", mapData.properties.size() );
        SHNK_CORE_INFO( "Parsed {} Layers.", mapData.layers.size() );
        SHNK_CORE_INFO( "Parsed {} Tilesets.", mapData.tilesets.size() );
        SHNK_CORE_INFO( "Map Parsed: {}x{} tiles.", mapData.width, mapData.height );
        SHNK_CORE_INFO( "Tile Size: {}x{} pixels.", mapData.tileWidth, mapData.tileHeight );

        for ( XMLElement * e = mapElement->FirstChildElement(); e != nullptr; e = e->NextSiblingElement() )
        {
            std::string value = e->Value();
            if ( value == "layer" )
            {
                if ( auto layerOpt = ParseLayer( e ) )
                {
                    mapData.layers.push_back( *layerOpt );
                }
            }
            else if ( value == "tileset" )
            {
                mapData.tilesets.push_back( ParseTileset( e ) );
            }
        }

        mapData.m_VisibleLayerCount = 0;
        for ( const auto & layer : mapData.layers )
        {
            if ( layer.visible )
            {
                mapData.m_VisibleLayerCount++;
            }
        }

        SHNK_CORE_INFO( "Map Loaded From File: {}", filePath );
        SHNK_CORE_INFO( "Map Name: {}", mapData.name );
        SHNK_CORE_INFO( "Parsed {} Map Properties.", mapData.properties.size() );
        SHNK_CORE_INFO( "Parsed {} Layers.", mapData.layers.size() );
        SHNK_CORE_INFO( "Parsed {} Tilesets.", mapData.tilesets.size() );
        SHNK_CORE_INFO( "Map Parsed: {}x{} tiles.", mapData.width, mapData.height );
        SHNK_CORE_INFO( "Tile Size: {}x{} pixels.", mapData.tileWidth, mapData.tileHeight );

        return mapData;
    }

    MapData MapParser::ParseFromMemory( const void * data, size_t size, const char * sourcePath )
    {
        using namespace tinyxml2;

        MapData               mapData;
        tinyxml2::XMLDocument doc;

        if ( doc.Parse( reinterpret_cast<const char *>( data ), size ) != XML_SUCCESS )
        {
            SHNK_CORE_ERROR( "Error parsing TMX map data from memory: {}", sourcePath ? sourcePath : "<memory>" );
            return mapData;
        }

        m_FilePath = sourcePath ? sourcePath : "";

        mapData.name = std::filesystem::path( m_FilePath ).stem().string();

        XMLElement * mapElement = doc.RootElement();
        if ( !mapElement )
        {
            SHNK_CORE_ERROR( "Error: Could not find root <map> element in {}", m_FilePath );
            return mapData;
        }

        mapData.width      = mapElement->IntAttribute( "width" );
        mapData.height     = mapElement->IntAttribute( "height" );
        mapData.tileWidth  = mapElement->IntAttribute( "tilewidth" );
        mapData.tileHeight = mapElement->IntAttribute( "tileheight" );

        if ( XMLElement * propertiesElement = mapElement->FirstChildElement( "properties" ) )
        {
            for ( XMLElement * propertyElement = propertiesElement->FirstChildElement( "property" );
                  propertyElement != nullptr;
                  propertyElement = propertyElement->NextSiblingElement( "property" ) )
            {
                const char * propName     = propertyElement->Attribute( "name" );
                const char * propValueStr = propertyElement->Attribute( "value" );
                const char * propTypeStr  = propertyElement->Attribute( "type" );

                if ( propName && propValueStr )
                {
                    std::string type = propTypeStr ? propTypeStr : "string";

                    if ( type == "int" )
                    {
                        mapData.properties[propName] = std::stoi( propValueStr );
                    }
                    else if ( type == "float" )
                    {
                        mapData.properties[propName] = std::stof( propValueStr );
                    }
                    else if ( type == "bool" )
                    {
                        mapData.properties[propName] = ( std::string( propValueStr ) == "true" );
                    }
                    else
                    {
                        mapData.properties[propName] = std::string( propValueStr );
                    }
                }
            }
        }

        for ( XMLElement * e = mapElement->FirstChildElement(); e != nullptr; e = e->NextSiblingElement() )
        {
            std::string value = e->Value();
            if ( value == "layer" )
            {
                if ( auto layerOpt = ParseLayer( e ) )
                {
                    mapData.layers.push_back( *layerOpt );
                }
            }
            else if ( value == "tileset" )
            {
                mapData.tilesets.push_back( ParseTileset( e ) );
            }
        }

        mapData.m_VisibleLayerCount = 0;
        for ( const auto & layer : mapData.layers )
        {
            if ( layer.visible )
            {
                mapData.m_VisibleLayerCount++;
            }
        }

        SHNK_CORE_INFO( "Map Loaded From Memory: {}", sourcePath );
        SHNK_CORE_INFO( "Map Name: {}", mapData.name );
        SHNK_CORE_INFO( "Parsed {} Map Properties.", mapData.properties.size() );
        SHNK_CORE_INFO( "Parsed {} Layers.", mapData.layers.size() );
        SHNK_CORE_INFO( "Parsed {} Tilesets.", mapData.tilesets.size() );
        SHNK_CORE_INFO( "Map Parsed: {}x{} tiles.", mapData.width, mapData.height );
        SHNK_CORE_INFO( "Tile Size: {}x{} pixels.", mapData.tileWidth, mapData.tileHeight );

        return mapData;
    }

    bool MapParser::SaveMap( const MapData & mapData )
    {
        using namespace tinyxml2;

        tinyxml2::XMLDocument doc;
        if ( doc.LoadFile( this->m_FilePath.c_str() ) != XML_SUCCESS )
        {
            SHNK_CORE_ERROR( "Failed to load TMX file for saving: {}", this->m_FilePath );
            return false;
        }

        XMLElement * mapElement = doc.RootElement();
        if ( !mapElement )
        {
            SHNK_CORE_ERROR( "Error: Could not find root <map> element in {} for saving.", this->m_FilePath );
            return false;
        }

        XMLElement * propertiesElement = mapElement->FirstChildElement( "properties" );
        if ( !propertiesElement )
        {
            propertiesElement = doc.NewElement( "properties" );
            mapElement->InsertFirstChild( propertiesElement );
        }

        propertiesElement->DeleteChildren();

        for ( const auto & [key, value] : mapData.properties )
        {
            XMLElement * propertyElement = doc.NewElement( "property" );
            propertyElement->SetAttribute( "name", key.c_str() );

            std::visit( [&]( auto && arg )
                        {
                            using T = std::decay_t<decltype( arg )>;
                            if constexpr ( std::is_same_v<T, std::string> )
                            {
                                propertyElement->SetAttribute( "value", arg.c_str() );
                                propertyElement->SetAttribute( "type", "string" );
                            }
                            else if constexpr ( std::is_same_v<T, int> )
                            {
                                propertyElement->SetAttribute( "value", std::to_string( arg ).c_str() );
                                propertyElement->SetAttribute( "type", "int" );
                            }
                            else if constexpr ( std::is_same_v<T, float> )
                            {
                                propertyElement->SetAttribute( "value", std::to_string( arg ).c_str() );
                                propertyElement->SetAttribute( "type", "float" );
                            }
                            else if constexpr ( std::is_same_v<T, bool> )
                            {
                                propertyElement->SetAttribute( "value", arg ? "true" : "false" );
                                propertyElement->SetAttribute( "type", "bool" );
                            }
                        },
                        value );

            propertiesElement->InsertEndChild( propertyElement );
        }

        std::set<std::string> memoryLayerNames;
        for ( const auto & layer : mapData.layers )
        {
            memoryLayerNames.insert( layer.name );
        }

        std::vector<XMLElement *> layersToDelete;
        for ( XMLElement * layerElement = mapElement->FirstChildElement( "layer" ); layerElement != nullptr; layerElement = layerElement->NextSiblingElement( "layer" ) )
        {
            const char * xmlLayerName = layerElement->Attribute( "name" );
            if ( xmlLayerName && memoryLayerNames.find( xmlLayerName ) == memoryLayerNames.end() )
            {
                layersToDelete.push_back( layerElement );
            }
        }

        for ( XMLElement * layer : layersToDelete )
        {
            SHNK_CORE_TRACE( "Removing deleted layer '{}' from TMX file.", layer->Attribute( "name" ) );
            mapElement->DeleteChild( layer );
        }

        for ( const auto & layerData : mapData.layers )
        {
            XMLElement * layerElement = nullptr;
            for ( XMLElement * el = mapElement->FirstChildElement( "layer" ); el != nullptr; el = el->NextSiblingElement( "layer" ) )
            {
                if ( const char * name = el->Attribute( "name" ) )
                {
                    if ( std::string( name ) == layerData.name )
                    {
                        layerElement = el;
                        break;
                    }
                }
            }

            std::stringstream csvStream;
            for ( int y = 0; y < layerData.height; ++y )
            {
                for ( int x = 0; x < layerData.width; ++x )
                {
                    csvStream << layerData.data[y * layerData.width + x];
                    if ( x < layerData.width - 1 || y < layerData.height - 1 )
                    {
                        csvStream << ",";
                    }
                }
                if ( y < layerData.height - 1 )
                {
                    csvStream << "\n";
                }
            }

            if ( layerElement )
            {
                XMLElement * dataElement = layerElement->FirstChildElement( "data" );
                if ( dataElement )
                {
                    dataElement->SetText( csvStream.str().c_str() );
                }
            }
            else
            {
                XMLElement * newLayerElement = doc.NewElement( "layer" );
                newLayerElement->SetAttribute( "name", layerData.name.c_str() );
                newLayerElement->SetAttribute( "width", layerData.width );
                newLayerElement->SetAttribute( "height", layerData.height );
                newLayerElement->SetAttribute( "visible", layerData.visible );

                XMLElement * newDataElement = doc.NewElement( "data" );
                newDataElement->SetAttribute( "encoding", "csv" );
                newDataElement->SetText( csvStream.str().c_str() );

                newLayerElement->InsertEndChild( newDataElement );
                mapElement->InsertEndChild( newLayerElement );
            }
        }

        if ( doc.SaveFile( this->m_FilePath.c_str() ) == XML_SUCCESS )
        {
            SHNK_CORE_INFO( "Map saved successfully to {}", this->m_FilePath );
            return true;
        }
        else
        {
            SHNK_CORE_ERROR( "Failed to save map to {}", this->m_FilePath );
            return false;
        }
    }

    Tileset MapParser::ParseTileset( tinyxml2::XMLElement * tilesetElement )
    {
        using namespace tinyxml2;
        Tileset tileset;
        tileset.firstGid = tilesetElement->IntAttribute( "firstgid" );

        const char * source = tilesetElement->Attribute( "source" );
        if ( source )
        {
            const auto tsxPath      = std::filesystem::path( source );
            const auto tsxAssetData = Shinkiro::Core::App->GetBundleManager().GetAssetData( "Maps/" + tsxPath.string() );

            if ( tsxAssetData.empty() )
            {
                SHNK_CORE_ERROR( "Failed to load external tileset asset: {}", tsxPath.string() );
                return tileset;
            }

            XMLDocument tsxDoc;
            if ( tsxDoc.Parse( reinterpret_cast<const char *>( tsxAssetData.data() ), tsxAssetData.size() ) != XML_SUCCESS )
            {
                SHNK_CORE_ERROR( "Failed to parse external tileset: {}", tsxPath.string() );
                return tileset;
            }

            XMLElement * externalTilesetElement = tsxDoc.RootElement();
            if ( externalTilesetElement )
            {
                tileset.tileWidth  = externalTilesetElement->IntAttribute( "tilewidth" );
                tileset.tileHeight = externalTilesetElement->IntAttribute( "tileheight" );
                tileset.columns    = externalTilesetElement->IntAttribute( "columns" );

                XMLElement * imageElement = externalTilesetElement->FirstChildElement( "image" );
                if ( imageElement )
                {
                    const char * imageSource = imageElement->Attribute( "source" );

                    std::filesystem::path imagePath = tsxPath.parent_path() / std::filesystem::path( imageSource );
                    tileset.imageSource             = imagePath.generic_string();
                    tileset.imageWidth              = imageElement->IntAttribute( "width" );
                    tileset.imageHeight             = imageElement->IntAttribute( "height" );
                }
                else
                {
                    SHNK_CORE_TRACE( "Warning: External tileset '{}' has no <image> element.", tsxPath.string() );
                }
            }
        }
        else
        {
            tileset.tileWidth  = tilesetElement->IntAttribute( "tilewidth" );
            tileset.tileHeight = tilesetElement->IntAttribute( "tileheight" );
            tileset.columns    = tilesetElement->IntAttribute( "columns" );

            XMLElement * imageElement = tilesetElement->FirstChildElement( "image" );
            if ( imageElement )
            {
                const char * imageSource = imageElement->Attribute( "source" );

                std::filesystem::path imagePath = std::filesystem::path( m_FilePath ).parent_path() / std::filesystem::path( imageSource );
                tileset.imageSource             = imagePath.generic_string();
                tileset.imageWidth              = imageElement->IntAttribute( "width" );
                tileset.imageHeight             = imageElement->IntAttribute( "height" );
            }
        }

        if ( tileset.columns == 0 && tileset.tileWidth > 0 && tileset.imageWidth > 0 )
        {
            tileset.columns = tileset.imageWidth / tileset.tileWidth;
        }

        if ( tileset.rows == 0 && tileset.tileHeight > 0 && tileset.imageHeight > 0 )
        {
            tileset.rows = tileset.imageHeight / tileset.tileHeight;
        }

        // SHNK_CORE_TRACE(
        //     "Tileset Parsed: firstGid={}, tileWidth={}, tileHeight={}, imageSource={}, imageWidth={}, imageHeight={}, columns={}, rows={}",
        //     tileset.firstGid,
        //     tileset.tileWidth,
        //     tileset.tileHeight,
        //     tileset.imageSource,
        //     tileset.imageWidth,
        //     tileset.imageHeight,
        //     tileset.columns,
        //     tileset.rows
        // );

        return tileset;
    }

    std::optional<TileLayer> MapParser::ParseLayer( tinyxml2::XMLElement * layerElement )
    {
        using namespace tinyxml2;
        TileLayer layer;

        layer.name    = layerElement->Attribute( "name" );
        layer.visible = layerElement->IntAttribute( "visible", 1 );

        if ( !layer.visible )
        {
            SHNK_CORE_TRACE( "Skipping parsing of invisible layer: {}", layer.name );
            return std::nullopt;
        }

        layer.width  = layerElement->IntAttribute( "width" );
        layer.height = layerElement->IntAttribute( "height" );

        // SHNK_CORE_INFO( "Parsing Layer: name={}, width={}, height={}, visible={}", layer.name, layer.width, layer.height, layer.visible );

        // Find the <data> element within the <layer>
        XMLElement * dataElement = layerElement->FirstChildElement( "data" );
        if ( !dataElement )
        {
            SHNK_CORE_ERROR( "Error: Layer '{}' has no <data> element.", layer.name );
            return layer;
        }

        // Check encoding - we currently only support CSV
        const char * encoding = dataElement->Attribute( "encoding" );
        if ( !encoding || std::string( encoding ) != "csv" )
        {
            SHNK_CORE_ERROR( "Error: Unsupported layer encoding '{}'. Only 'csv' is supported.", ( encoding ? encoding : "none" ) );
            return layer;
        }

        // Get the text content of the <data> element, which is a CSV string
        const char * csv_text = dataElement->GetText();
        if ( !csv_text )
        {
            SHNK_CORE_WARN( "Warning: Layer '{}' <data> element is empty.", layer.name );
            return layer;
        }

        // Parse the CSV string
        std::stringstream ss( csv_text );
        std::string       value;
        while ( std::getline( ss, value, ',' ) )
        {
            value.erase( std::remove_if( value.begin(), value.end(), ::isspace ), value.end() );
            if ( !value.empty() )
            {
                try
                {
                    layer.data.push_back( std::stoi( value ) );
                }
                catch ( const std::invalid_argument & )
                {
                    // We ignore invalid numbers, which can happen with trailing commas
                }
            }
        }

        return layer;
    }
}
