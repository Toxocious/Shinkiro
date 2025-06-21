#include <Asset/_FilePath.h>

#include <Asset/AssetBundleHeader.h>
#include <Asset/Log.h>

#include <Asset/AssetBundleManager.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace Shinkiro::Asset
{
    AssetBundleManager::AssetBundleManager( const std::string & BundleName )
        : bundleName( BundleName )
    {
        // Use AssetBundles subdirectory for bundle path
        bundlePath = GetExecutableDirectory() / "AssetBundles" / bundleName;

        // Keep extraction path in root executable directory
        extractionPath = GetExecutableDirectory() / "UnpackedAssets";
    }

    void AssetBundleManager::SetExtractionPath( const std::filesystem::path & path )
    {
        extractionPath = path;
    }

    bool AssetBundleManager::LoadBundleInfo()
    {
        std::ifstream file( bundlePath, std::ios::binary );
        if ( !file.is_open() )
        {
            std::string msg = "Failed to open bundle: " + bundlePath.string();
            Log::Write( msg );
            std::cerr << msg << std::endl;
            return false;
        }

        AssetBundleHeader header;
        file.read( reinterpret_cast<char *>( &header ), sizeof( header ) );
        if ( !header.isValid() )
        {
            std::string msg = "Invalid bundle format: " + bundlePath.string();
            Log::Write( msg );
            std::cerr << msg << std::endl;

            return false;
        }

        std::stringstream ss;
        ss << "--- Bundle Information ---" << std::endl;
        ss << "  Name: " << bundleName << std::endl;
        ss << "  Magic: 0x" << std::hex << header.getMagic() << std::dec << std::endl;
        ss << "  Version: " << header.getVersion() << std::endl;
        ss << "  Asset Count: " << header.getAssetCount() << std::endl;
        ss << "  Header Size: " << header.getHeaderSize() << " bytes" << std::endl;
        ss << "--------------------------";
        Log::Write( ss.str() );
        Log::Write( "" );

        header.DisplayBundleInfo();
        assets.clear();

        for ( uint32_t i = 0; i < header.getAssetCount(); ++i )
        {
            uint32_t nameLength;
            file.read( reinterpret_cast<char *>( &nameLength ), sizeof( nameLength ) );

            std::string name( nameLength, '\0' );
            file.read( &name[0], nameLength );

            uint64_t offset, size;
            file.read( reinterpret_cast<char *>( &offset ), sizeof( offset ) );
            file.read( reinterpret_cast<char *>( &size ), sizeof( size ) );

            assets.emplace_back( name, offset, size );
        }

        return true;
    }

    std::vector<std::string> AssetBundleManager::GetAssetList()
    {
        std::vector<std::string> names;
        for ( const auto & asset : assets )
        {
            names.push_back( asset.name );
        }

        return names;
    }

    std::vector<uint8_t> AssetBundleManager::ExtractAssetToMemory( const std::string & assetName )
    {
        auto it = std::find_if(
            assets.begin(),
            assets.end(),
            [&]( const AssetEntry & entry )
            {
                return entry.name == assetName;
            }
        );

        if ( it == assets.end() )
        {
            throw std::runtime_error( "Asset not found: " + assetName );
        }

        std::ifstream file( bundlePath, std::ios::binary );
        if ( !file.is_open() )
        {
            throw std::runtime_error( "Failed to open bundle: " + bundlePath.string() );
        }

        file.seekg( it->offset );
        std::vector<uint8_t> data( it->size );
        file.read( reinterpret_cast<char *>( data.data() ), it->size );

        return data;
    }

    std::filesystem::path AssetBundleManager::ExtractAssetToFile( const std::string & assetName )
    {
        std::vector<uint8_t>  data    = ExtractAssetToMemory( assetName );
        std::filesystem::path outPath = extractionPath / assetName;

        // Create subdirectories if needed
        std::filesystem::path parent = outPath.parent_path();
        if ( !parent.empty() && !std::filesystem::exists( parent ) )
        {
            std::filesystem::create_directories( parent );
        }

        std::ofstream outFile( outPath, std::ios::binary );
        if ( !outFile.is_open() )
        {
            throw std::runtime_error( "Failed to create output file: " + outPath.string() );
        }

        outFile.write( reinterpret_cast<const char *>( data.data() ), data.size() );

        return outPath;
    }

    // Extract all assets
    std::map<std::string, std::filesystem::path> AssetBundleManager::ExtractAllAssets()
    {
        Log::Write( "Unpacking All Assets" );
        Log::Write( "" );

        Log::Write( "~ Found " + std::to_string( assets.size() ) + " assets in the bundle" );
        Log::Write( "" );

        std::map<std::string, std::filesystem::path> extractedPaths;
        for ( const auto & asset : assets )
        {
            try
            {
                std::filesystem::path path = ExtractAssetToFile( asset.name );
                extractedPaths[asset.name] = path;
                Log::Write( "Unpacked asset: " + asset.name );
            }
            catch ( const std::exception & e )
            {
                std::string msg = "Failed to extract asset " + asset.name + ": " + e.what();
                Log::Write( msg );
                std::cerr << msg << std::endl;
            }
        }
        return extractedPaths;
    }

    // Create a bundle from assets in a directory
    bool AssetBundleManager::CreateBundle( const std::filesystem::path & inputDir, const std::filesystem::path & outputPath )
    {
        Log::Write( "Creating asset bundle: " + outputPath.string() );
        Log::Write( "" );

        // Verifer that the input directory to be bundled actually exists.
        if ( !std::filesystem::exists( inputDir ) || !std::filesystem::is_directory( inputDir ) )
        {
            std::string msg = "Input directory does not exist: " + inputDir.string();
            Log::Write( msg );
            std::cerr << msg << std::endl;

            return false;
        }

        // Add all files within the input directory and its subdirectories to a vector.
        std::vector<std::filesystem::path> files;
        for ( const auto & entry : std::filesystem::recursive_directory_iterator( inputDir ) )
        {
            if ( std::filesystem::is_regular_file( entry ) )
            {
                files.push_back( entry.path() );
            }
        }

        // Create the output bundle file.
        std::ofstream bundle( outputPath, std::ios::binary );
        if ( !bundle.is_open() )
        {
            std::string msg = "\t\tFailed to create bundle file: " + outputPath.string();
            Log::Write( msg );
            std::cerr << msg << std::endl;

            return false;
        }

        // Iniitalize the bundle header, set its asset count, and write the header to the bundle output.
        AssetBundleHeader header;
        header.setAssetCount( static_cast<uint32_t>( files.size() ) );
        bundle.write( reinterpret_cast<const char *>( &header ), sizeof( header ) );

        // Calculate where asset data will start after all entries
        uint64_t dataStart = sizeof( header );
        for ( const auto & file : files )
        {
            std::string relativePath = file.lexically_relative( inputDir ).string();
            std::replace( relativePath.begin(), relativePath.end(), '\\', '/' );
            dataStart += sizeof( uint32_t ) +    // name length
                         relativePath.length() + // name
                         sizeof( uint64_t ) +    // offset
                         sizeof( uint64_t );     // size
        }

        // Write entries with correct offsets
        uint64_t currentOffset = dataStart;
        for ( const auto & file : files )
        {
            // Write entry name
            std::string relativePath = file.lexically_relative( inputDir ).string();
            std::replace( relativePath.begin(), relativePath.end(), '\\', '/' );
            Log::Write( "Packed Asset: " + relativePath );
            uint32_t nameLength = static_cast<uint32_t>( relativePath.length() );
            bundle.write( reinterpret_cast<const char *>( &nameLength ), sizeof( nameLength ) );
            bundle.write( relativePath.c_str(), nameLength );

            // Write offset and size
            uint64_t fileSize = std::filesystem::file_size( file );
            bundle.write( reinterpret_cast<const char *>( &currentOffset ), sizeof( currentOffset ) );
            bundle.write( reinterpret_cast<const char *>( &fileSize ), sizeof( fileSize ) );

            // Update offset for next file
            currentOffset += fileSize;
        }

        // Write actual file data
        for ( const auto & file : files )
        {
            std::ifstream input( file, std::ios::binary );
            if ( !input.is_open() )
            {
                std::string msg = "\t\tFailed to open input file: " + file.string();
                Log::Write( msg );
                std::cerr << msg << std::endl;

                return false;
            }
            bundle << input.rdbuf();
        }
        return true;
    }
}
