#include <Asset/_Common.h>

#include <Asset/_FilePath.h>

#include <Asset/AssetBundleHeader.h>
#include <Asset/Log.h>

#include <Asset/AssetBundleManager.h>

#include <Core/Util/Log.h>

namespace Shinkiro::Asset
{
    AssetBundleManager::AssetBundleManager( const std::string & BundleName )
        : m_BundleName( BundleName )
    {
        // Use AssetBundles subdirectory for bundle path
        m_BundlePath = GetExecutableDirectory() / "AssetBundles" / m_BundleName;

        // Keep extraction path in root executable directory
        m_ExtractionPath = GetExecutableDirectory() / "UnpackedAssets";
    }

    void AssetBundleManager::SetExtractionPath( const std::filesystem::path & path )
    {
        m_ExtractionPath = path;
    }

    bool AssetBundleManager::LoadBundleInfo()
    {
        std::ifstream file( m_BundlePath, std::ios::binary );
        if ( !file.is_open() )
        {
            std::string msg = "Failed to open bundle: " + m_BundlePath.string();
            Log::Write( msg );
            std::cerr << msg << std::endl;
            return false;
        }

        AssetBundleHeader header;
        file.read( reinterpret_cast<char *>( &header ), sizeof( header ) );
        if ( !header.IsValid() )
        {
            std::string msg = "Invalid bundle format: " + m_BundlePath.string() + " (magic: 0x" + std::to_string( header.GetMagic() ) + ")" +
                              " (version: " + std::to_string( header.GetVersion() ) + ")";
            Log::Write( msg );
            std::cerr << msg << std::endl;

            SHNK_CORE_ERROR( "Invalid bundle format: {} (magic: 0x{:X}) (version: {})", m_BundlePath.string(), header.GetMagic(), header.GetVersion() );

            return false;
        }

        std::stringstream ss;
        ss << "--- Bundle Information ---" << std::endl;
        ss << "  Name: " << m_BundleName << std::endl;
        ss << "  Magic: 0x" << std::hex << header.GetMagic() << std::dec << std::endl;
        ss << "  Version: " << header.GetVersion() << std::endl;
        ss << "  Asset Count: " << header.GetAssetCount() << std::endl;
        ss << "  Header Size: " << header.GetHeaderSize() << " bytes" << std::endl;
        ss << "--------------------------";
        Log::Write( ss.str() );
        Log::Write( "" );

        header.DisplayBundleInfo();
        m_Assets.clear();

        for ( uint32_t i = 0; i < header.GetAssetCount(); ++i )
        {
            uint32_t nameLength;
            file.read( reinterpret_cast<char *>( &nameLength ), sizeof( nameLength ) );

            std::string name( nameLength, '\0' );
            file.read( &name[0], nameLength );

            uint64_t offset, compressedSize, uncompressedSize;
            file.read( reinterpret_cast<char *>( &offset ), sizeof( offset ) );
            file.read( reinterpret_cast<char *>( &compressedSize ), sizeof( compressedSize ) );
            file.read( reinterpret_cast<char *>( &uncompressedSize ), sizeof( uncompressedSize ) );

            m_Assets.emplace_back( name, offset, compressedSize, uncompressedSize );
        }

        return true;
    }

    void AssetBundleManager::LoadAssetsIntoMemory()
    {
        const auto m_BundlePath = GetBundlePath();

        if ( std::filesystem::exists( m_BundlePath ) )
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
            std::string msg = "Asset bundle does not exist: " + m_BundlePath.string();
            Log::Write( msg );
            std::cerr << msg << std::endl;

            throw std::runtime_error( msg );
        }
    }

    std::vector<std::string> AssetBundleManager::GetAssetList()
    {
        std::vector<std::string> names;
        for ( const auto & asset : m_Assets )
        {
            names.push_back( asset.name );
        }

        return names;
    }

    const std::vector<uint8_t> & AssetBundleManager::GetAssetData( const std::string & assetName )
    {
        auto cacheIt = m_AssetCache.find( assetName );
        if ( cacheIt != m_AssetCache.end() )
        {
            return cacheIt->second;
        }

        ExtractAssetToMemory( assetName );

        return m_AssetCache.at( assetName );
    }

    std::vector<uint8_t> AssetBundleManager::ExtractAssetToMemory( const std::string & assetName )
    {
        auto cacheIt = m_AssetCache.find( assetName );
        if ( cacheIt != m_AssetCache.end() )
        {
            SHNK_CORE_TRACE( "Asset '{}' found in cache.", assetName );

            return cacheIt->second;
        }

        // If the asset metadata hasn't been loaded yet, load it now.
        if ( m_Assets.empty() )
        {
            SHNK_CORE_TRACE( "Asset metadata not loaded. Loading bundle info for {}.", m_BundlePath.string() );
            if ( !LoadBundleInfo() )
            {
                SHNK_CORE_ERROR( "Failed to load bundle info for: {}", m_BundlePath.string() );

                throw std::runtime_error( "Failed to load bundle info for: " + m_BundlePath.string() );
            }
        }

        auto it = std::find_if(
            m_Assets.begin(),
            m_Assets.end(),
            [&]( const AssetEntry & entry )
            {
                return entry.name == assetName;
            }
        );

        if ( it == m_Assets.end() )
        {
            SHNK_CORE_ERROR( "Asset not found in bundle: {}", assetName );

            throw std::runtime_error( "Asset not found: " + assetName );
        }

        std::ifstream file( m_BundlePath, std::ios::binary );
        if ( !file.is_open() )
        {
            SHNK_CORE_ERROR( "Failed to open bundle: {}", m_BundlePath.string() );

            throw std::runtime_error( "Failed to open bundle: " + m_BundlePath.string() );
        }

        auto start = std::chrono::high_resolution_clock::now();

        std::vector<uint8_t> finalData;

        {
            file.seekg( it->offset );
            std::vector<uint8_t> fileData( it->compressedSize );
            file.read( reinterpret_cast<char *>( fileData.data() ), it->compressedSize );

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
                    SHNK_CORE_ERROR( "Failed to decompress asset {}: {}", assetName, ZSTD_getErrorName( decompressedSize ) );

                    throw std::runtime_error(
                        "Failed to decompress asset " + assetName + ": " +
                        ZSTD_getErrorName( decompressedSize )
                    );
                }

                decompressedData.resize( decompressedSize );
                finalData = std::move( decompressedData );
            }
            else
            {
                finalData = std::move( fileData );
            }
        }

        SHNK_CORE_TRACE( "Loaded '{0}' in {1} ms ({2} bytes)", assetName, std::chrono::duration<float, std::milli>( std::chrono::high_resolution_clock::now() - start ).count(), finalData.size() );

        auto [emplacedIt, success] = m_AssetCache.emplace( assetName, std::move( finalData ) );
        return emplacedIt->second;
    }

    std::filesystem::path AssetBundleManager::ExtractAssetToFile( const std::string & assetName )
    {
        std::vector<uint8_t>  data    = ExtractAssetToMemory( assetName );
        std::filesystem::path outPath = m_ExtractionPath / assetName;

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

    std::map<std::string, std::filesystem::path> AssetBundleManager::ExtractAllAssets()
    {
        Log::Write( "Unpacking All Assets" );
        Log::Write( "" );

        Log::Write( "~ Found " + std::to_string( m_Assets.size() ) + " assets in the bundle" );
        Log::Write( "" );

        std::map<std::string, std::filesystem::path> extractedPaths;

        for ( const auto & asset : m_Assets )
        {
            try
            {
                std::filesystem::path path = ExtractAssetToFile( asset.name );
                extractedPaths[asset.name] = path;
                Log::Write( "Unpacked asset: " + asset.name );

                SHNK_CORE_INFO( "Unpacked asset: {}", asset.name );
            }
            catch ( const std::exception & e )
            {
                std::string msg = "Failed to extract asset " + asset.name + ": " + e.what();
                Log::Write( msg );
                std::cerr << msg << std::endl;

                SHNK_CORE_ERROR( "Failed to extract asset {}: {}", asset.name, e.what() );
            }
        }

        return extractedPaths;
    }

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
        header.SetAssetCount( static_cast<uint32_t>( files.size() ) );

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
