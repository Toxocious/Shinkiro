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

#include <zstd/zstd.h>

/**
 * @file AssetBundleManager.cpp
 * @brief Implementation of the AssetBundleManager class for managing asset bundles.
 */
namespace Shinkiro::Asset
{
    /**
     * @brief Constructor for AssetBundleManager.
     * @param BundleName The name of the asset bundle to manage.
     */
    AssetBundleManager::AssetBundleManager( const std::string & BundleName )
        : bundleName( BundleName )
    {
        // Use AssetBundles subdirectory for bundle path
        bundlePath = GetExecutableDirectory() / "AssetBundles" / bundleName;

        // Keep extraction path in root executable directory
        extractionPath = GetExecutableDirectory() / "UnpackedAssets";
    }

    /**
     * @brief Set the extraction path for unpacked assets.
     * @param path The directory where assets will be extracted.
     */
    void AssetBundleManager::SetExtractionPath( const std::filesystem::path & path )
    {
        extractionPath = path;
    }

    /**
     * @brief Load the asset bundle header and asset entries from the bundle file.
     */
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
            std::string msg = "Invalid bundle format: " + bundlePath.string() + " (magic: 0x" + std::to_string( header.getMagic() ) + ")" +
                              " (version: " + std::to_string( header.getVersion() ) + ")";
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

            uint64_t offset, compressedSize, uncompressedSize;
            file.read( reinterpret_cast<char *>( &offset ), sizeof( offset ) );
            file.read( reinterpret_cast<char *>( &compressedSize ), sizeof( compressedSize ) );
            file.read( reinterpret_cast<char *>( &uncompressedSize ), sizeof( uncompressedSize ) );

            assets.emplace_back( name, offset, compressedSize, uncompressedSize );
        }

        return true;
    }

    /**
     * @brief Load all assets from the asset bundle into memory.
     * @throws std::runtime_error if any asset is not found or the bundle cannot be opened.
     */
    void AssetBundleManager::LoadAssetsIntoMemory()
    {
        const auto bundlePath = GetBundlePath();

        if ( std::filesystem::exists( bundlePath ) )
        {
            if ( LoadBundleInfo() )
            {
                auto assetList = GetAssetList();
                if ( !assetList.empty() )
                {
                    for ( const auto & assetName : assetList )
                    {
                        try
                        {
                            // ExtractAssetToMemory will place the data in the cache
                            ExtractAssetToMemory( assetName );
                        }
                        catch ( const std::exception & e )
                        {
                            std::string msg = "Failed to load asset " + assetName + ": " + e.what();
                            Log::Write( msg );
                            std::cerr << msg << std::endl;

                            throw std::runtime_error( msg );
                        }
                    }
                }
            }
        }
        else
        {
            std::string msg = "Asset bundle does not exist: " + bundlePath.string();
            Log::Write( msg );
            std::cerr << msg << std::endl;

            throw std::runtime_error( msg );
        }
    }

    /**
     * @brief Get a list of all asset names in the bundle.
     * @return A vector of asset names.
     */
    std::vector<std::string> AssetBundleManager::GetAssetList()
    {
        std::vector<std::string> names;
        for ( const auto & asset : assets )
        {
            names.push_back( asset.name );
        }

        return names;
    }

    /**
     * @brief Get the data for a specific asset by name.
     * @param assetName The name of the asset to retrieve.
     * @return A reference to the vector containing the asset data.
     */
    const std::vector<uint8_t> & AssetBundleManager::GetAssetData( const std::string & assetName )
    {
        auto cacheIt = assetCache.find( assetName );
        if ( cacheIt != assetCache.end() )
        {
            return cacheIt->second;
        }

        // Data is not in cache, so extract it. It will be cached by ExtractAssetToMemory.
        ExtractAssetToMemory( assetName );

        // Return the newly cached data.
        return assetCache.at( assetName );
    }

    /**
     * @brief Extract a specific asset to memory.
     * @param assetName The name of the asset to extract.
     * @return A vector containing the asset data.
     * @throws std::runtime_error if the asset is not found or the bundle cannot be opened.
     */
    std::vector<uint8_t> AssetBundleManager::ExtractAssetToMemory( const std::string & assetName )
    {
        // If it's already cached, just return it.
        auto cacheIt = assetCache.find( assetName );
        if ( cacheIt != assetCache.end() )
        {
            return cacheIt->second;
        }

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
        std::vector<uint8_t> fileData( it->compressedSize );
        file.read( reinterpret_cast<char *>( fileData.data() ), it->compressedSize );

        // Check if the data is actually compressed
        if ( it->compressedSize < it->uncompressedSize )
        {
            std::vector<uint8_t> decompressedData( it->uncompressedSize );
            size_t const         decompressedSize = ZSTD_decompress(
                decompressedData.data(),
                decompressedData.size(),
                fileData.data(),
                fileData.size()
            );

            if ( ZSTD_isError( decompressedSize ) )
            {
                throw std::runtime_error(
                    "Failed to decompress asset " + assetName + ": " + ZSTD_getErrorName( decompressedSize )
                );
            }
            decompressedData.resize( decompressedSize );

            // Emplace the data into the cache and return it.
            auto [emplacedIt, success] = assetCache.emplace( assetName, std::move( decompressedData ) );
            return emplacedIt->second;
        }
        else
        {
            // Data is not compressed, just move it to the cache
            auto [emplacedIt, success] = assetCache.emplace( assetName, std::move( fileData ) );
            return emplacedIt->second;
        }
    }

    /**
     * @brief Extract a specific asset to a file.
     * @param assetName The name of the asset to extract.
     * @return The path to the extracted file.
     * @throws std::runtime_error if the asset is not found or the output file cannot be created.
     */
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

    /**
     * @brief Extract all assets in the bundle to files.
     * @return A map of asset names to their extracted file paths.
     */
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

    /**
     * @brief Create an asset bundle from a directory of files.
     * @param inputDir The directory containing files to bundle.
     * @param outputPath The path where the bundle will be created.
     * @return True if the bundle was created successfully, false otherwise.
     */
    bool AssetBundleManager::CreateBundle( const std::filesystem::path & inputDir, const std::filesystem::path & outputPath )
    {
        Log::Write( "Creating asset bundle: " + outputPath.string() );
        Log::Write( "" );
        if ( !std::filesystem::exists( inputDir ) || !std::filesystem::is_directory( inputDir ) )
        {
            std::string msg = "Input directory does not exist: " + inputDir.string();
            Log::Write( msg );
            std::cerr << msg << std::endl;
            return false;
        }

        std::vector<std::filesystem::path> files;
        for ( const auto & entry : std::filesystem::recursive_directory_iterator( inputDir ) )
        {
            if ( std::filesystem::is_regular_file( entry ) )
            {
                files.push_back( entry.path() );
            }
        }

        if ( files.empty() )
        {
            Log::Write( "No files found in input directory." );
            return false;
        }

        std::ofstream bundle( outputPath, std::ios::binary );
        if ( !bundle.is_open() )
        {
            std::string msg = "Failed to create bundle file: " + outputPath.string();
            Log::Write( msg );
            std::cerr << msg << std::endl;
            return false;
        }

        AssetBundleHeader header;
        header.setAssetCount( static_cast<uint32_t>( files.size() ) );

        // Leave space for the header
        bundle.seekp( sizeof( header ), std::ios::beg );

        std::vector<AssetEntry>           assetEntries;
        std::vector<std::vector<uint8_t>> compressedAssetData;
        uint64_t                          metadataSize = 0;

        for ( const auto & file : files )
        {
            std::string relativePath = file.lexically_relative( inputDir ).string();
            std::replace( relativePath.begin(), relativePath.end(), '\\', '/' );

            std::ifstream inputFile( file, std::ios::binary | std::ios::ate );
            size_t        uncompressedSize = inputFile.tellg();
            inputFile.seekg( 0, std::ios::beg );
            std::vector<uint8_t> srcBuffer( uncompressedSize );
            inputFile.read( reinterpret_cast<char *>( srcBuffer.data() ), uncompressedSize );

            size_t const         compressedBound = ZSTD_compressBound( uncompressedSize );
            std::vector<uint8_t> compressedBuffer( compressedBound );
            size_t const         compressedResult = ZSTD_compress( compressedBuffer.data(), compressedBound, srcBuffer.data(), uncompressedSize, ZSTD_CLEVEL_DEFAULT );

            if ( ZSTD_isError( compressedResult ) || compressedResult >= uncompressedSize )
            {
                // Store uncompressed if compression fails or makes it larger
                assetEntries.emplace_back( relativePath, 0, uncompressedSize, uncompressedSize );
                compressedAssetData.push_back( std::move( srcBuffer ) );
                metadataSize += sizeof( uint32_t ) + relativePath.length() + sizeof( uint64_t ) * 3;
            }
            else
            {
                // Store compressed
                compressedBuffer.resize( compressedResult );
                assetEntries.emplace_back( relativePath, 0, compressedResult, uncompressedSize );
                compressedAssetData.push_back( std::move( compressedBuffer ) );
                metadataSize += sizeof( uint32_t ) + relativePath.length() + sizeof( uint64_t ) * 3;
            }
        }

        uint64_t currentOffset = sizeof( header ) + metadataSize;
        for ( size_t i = 0; i < assetEntries.size(); ++i )
        {
            assetEntries[i].offset  = currentOffset;
            currentOffset          += assetEntries[i].compressedSize;
        }

        // Write metadata
        bundle.seekp( sizeof( header ), std::ios::beg );
        for ( const auto & entry : assetEntries )
        {
            uint32_t nameLength = static_cast<uint32_t>( entry.name.length() );
            bundle.write( reinterpret_cast<const char *>( &nameLength ), sizeof( nameLength ) );
            bundle.write( entry.name.c_str(), nameLength );
            bundle.write( reinterpret_cast<const char *>( &entry.offset ), sizeof( entry.offset ) );
            bundle.write( reinterpret_cast<const char *>( &entry.compressedSize ), sizeof( entry.compressedSize ) );
            bundle.write( reinterpret_cast<const char *>( &entry.uncompressedSize ), sizeof( entry.uncompressedSize ) );
        }

        // Write data
        for ( const auto & data : compressedAssetData )
        {
            bundle.write( reinterpret_cast<const char *>( data.data() ), data.size() );
        }

        // Write header at the beginning
        bundle.seekp( 0, std::ios::beg );
        bundle.write( reinterpret_cast<const char *>( &header ), sizeof( header ) );

        return true;
    }
}
