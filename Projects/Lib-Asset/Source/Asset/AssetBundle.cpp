#include <Asset/AssetBundle.h>

namespace Shinkiro::Asset
{
    AssetBundle::AssetBundle( const std::string & name, AssetType type )
        : m_Name( name ), m_Type( type )
    {
    }

    bool AssetBundle::PackAssets( const std::filesystem::path & outputPath )
    {
        switch ( m_Type )
        {
            case AssetType::Font:
                if ( m_FontAssets.empty() )
                {
                    return false;
                }
                break;

            case AssetType::Texture:
                if ( m_TextureAssets.empty() )
                {
                    return false;
                }
                break;

            default:
                return false;
        }

        // Create output directory if it doesn't exist
        std::filesystem::create_directories( outputPath.parent_path() );

        // Open output file
        std::ofstream outputFile( outputPath, std::ios::binary );
        if ( !outputFile.is_open() )
        {
            return false;
        }

        switch ( m_Type )
        {
            case AssetType::Font:
                for ( const auto & asset : m_FontAssets )
                {
                    std::ifstream inputFile( asset.GetPath(), std::ios::binary );
                    if ( !inputFile.is_open() )
                    {
                        return false;
                    }

                    // Write file data
                    inputFile.seekg( 0, std::ios::end );
                    size_t fileSize = inputFile.tellg();
                    inputFile.seekg( 0, std::ios::beg );

                    std::vector<char> buffer( fileSize );
                    inputFile.read( buffer.data(), fileSize );
                    outputFile.write( buffer.data(), fileSize );
                }
                break;

            case AssetType::Texture:
                for ( const auto & asset : m_TextureAssets )
                {
                    std::ifstream inputFile( asset.GetPath(), std::ios::binary );
                    if ( !inputFile.is_open() )
                    {
                        return false;
                    }

                    // Write file data
                    inputFile.seekg( 0, std::ios::end );
                    size_t fileSize = inputFile.tellg();
                    inputFile.seekg( 0, std::ios::beg );

                    std::vector<char> buffer( fileSize );
                    inputFile.read( buffer.data(), fileSize );
                    outputFile.write( buffer.data(), fileSize );
                }
                break;
        }

        // Write asset paths to the output file
        // for ( const auto & assetPath : m_AssetPaths )
        // {
        //     std::ifstream inputFile( assetPath, std::ios::binary );
        //     if ( !inputFile.is_open() )
        //     {
        //         return false;
        //     }

        //     // Get file size
        //     inputFile.seekg( 0, std::ios::end );
        //     size_t fileSize = inputFile.tellg();
        //     inputFile.seekg( 0, std::ios::beg );

        //     // Read entire file
        //     std::vector<char> buffer( fileSize );
        //     inputFile.read( buffer.data(), fileSize );

        //     // Write to output file
        //     outputFile.write( buffer.data(), fileSize );
        // }

        return true;
    }

    bool AssetBundle::AddAssetPath( const std::filesystem::path & assetPath )
    {
        if ( !std::filesystem::exists( assetPath ) )
        {
            return false;
        }

        m_AssetPaths.push_back( assetPath );
        return true;
    }
}
