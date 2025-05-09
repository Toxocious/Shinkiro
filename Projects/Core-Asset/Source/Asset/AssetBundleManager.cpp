#include <Asset/_FilePath.h>

#include <Asset/AssetBundleHeader.h>

#include <Asset/AssetBundleManager.h>

AssetBundleManager::AssetBundleManager( const std::string & name )
    : bundleName( name )
{
    bundlePath     = GetExecutableDirectory() / bundleName;
    extractionPath = GetExecutableDirectory() / "extracted_assets";
}

void AssetBundleManager::SetExtractionPath( const fs::path & path )
{
    extractionPath = path;
}

bool AssetBundleManager::LoadBundleInfo()
{
    std::ifstream file( bundlePath, std::ios::binary );
    if ( !file.is_open() )
    {
        std::cerr << "Failed to open bundle: " << bundlePath << std::endl;
        return false;
    }

    AssetBundleHeader header;
    file.read( reinterpret_cast<char *>( &header ), sizeof( header ) );

    if ( !header.isValid() )
    {
        std::cerr << "Invalid bundle format" << std::endl;
        return false;
    }

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
    auto it = std::find_if( assets.begin(), assets.end(), [&]( const AssetEntry & entry )
                            {
                                return entry.name == assetName;
                            } );

    if ( it == assets.end() )
    {
        throw std::runtime_error( "Asset not found: " + assetName );
    }

    std::ifstream file( bundlePath, std::ios::binary );
    if ( !file.is_open() )
    {
        throw std::runtime_error( "Failed to open bundle file" );
    }

    std::vector<uint8_t> data( it->size );
    file.seekg( it->offset );
    file.read( reinterpret_cast<char *>( data.data() ), it->size );

    if ( file.fail() )
    {
        throw std::runtime_error( "Failed to read asset data" );
    }

    return data;
}

/**
 * Doesn't properly write the asset's data to the file.
 * It'll create the output directory and subdirectories, but pretty much all extracted files will be empty.
 */
std::filesystem::path AssetBundleManager::ExtractAssetToFile( const std::string & assetName )
{
    auto data = ExtractAssetToMemory( assetName );

    // Ensure extraction directory exists
    if ( !std::filesystem::exists( extractionPath ) )
    {
        std::filesystem::create_directories( extractionPath );
    }

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
    std::map<std::string, std::filesystem::path> extractedPaths;

    for ( const auto & asset : assets )
    {
        try
        {
            std::filesystem::path path = ExtractAssetToFile( asset.name );
            extractedPaths[asset.name] = path;
        }
        catch ( const std::exception & e )
        {
            std::cerr << "Failed to extract " << asset.name << ": " << e.what() << std::endl;
        }
    }

    return extractedPaths;
}

// Create a bundle from assets in a directory
bool AssetBundleManager::CreateBundle( const fs::path & inputDir, const fs::path & outputPath )
{
    if ( !fs::exists( inputDir ) || !fs::is_directory( inputDir ) )
    {
        std::cerr << "Input directory does not exist: " << inputDir << std::endl;
        return false;
    }

    std::vector<fs::path> files;
    for ( const auto & entry : fs::recursive_directory_iterator( inputDir ) )
    {
        if ( fs::is_regular_file( entry ) )
        {
            files.push_back( entry.path() );
        }
    }

    AssetBundleHeader header;
    header.setAssetCount( static_cast<uint32_t>( files.size() ) );

    std::ofstream bundle( outputPath, std::ios::binary );
    if ( !bundle.is_open() )
    {
        std::cerr << "Failed to create bundle file: " << outputPath << std::endl;
        return false;
    }

    // Write header placeholder - we'll update it later
    bundle.write( reinterpret_cast<const char *>( &header ), sizeof( header ) );

    // Calculate relative paths for asset names
    std::vector<AssetEntry> entries;
    uint64_t                currentOffset = sizeof( header );

    // First write the asset entries table
    for ( const auto & file : files )
    {
        std::string relativePath = file.lexically_relative( inputDir ).string();
        // Normalize path separators to forward slashes
        std::replace( relativePath.begin(), relativePath.end(), '\\', '/' );

        uint32_t nameLength = static_cast<uint32_t>( relativePath.length() );
        bundle.write( reinterpret_cast<const char *>( &nameLength ), sizeof( nameLength ) );
        bundle.write( relativePath.c_str(), nameLength );

        // Placeholder for offset and size - we'll update these later
        uint64_t placeholderOffset = 0;
        uint64_t placeholderSize   = 0;
        bundle.write( reinterpret_cast<const char *>( &placeholderOffset ), sizeof( placeholderOffset ) );
        bundle.write( reinterpret_cast<const char *>( &placeholderSize ), sizeof( placeholderSize ) );

        // Update currentOffset for next entry
        currentOffset += sizeof( nameLength ) + nameLength + sizeof( uint64_t ) * 2;
    }

    // Now write the actual asset data and update the entries
    uint64_t dataOffset = bundle.tellp();
    bundle.seekp( sizeof( header ) );

    for ( const auto & file : files )
    {
        std::string relativePath = file.lexically_relative( inputDir ).string();
        std::replace( relativePath.begin(), relativePath.end(), '\\', '/' );

        // Skip the name field and write offset/size
        uint32_t nameLength = static_cast<uint32_t>( relativePath.length() );
        bundle.seekp( sizeof( nameLength ) + nameLength, std::ios::cur );

        uint64_t assetOffset = dataOffset;
        uint64_t assetSize   = fs::file_size( file );
        bundle.write( reinterpret_cast<const char *>( &assetOffset ), sizeof( assetOffset ) );
        bundle.write( reinterpret_cast<const char *>( &assetSize ), sizeof( assetSize ) );

        // Move to the end to append the file data
        bundle.seekp( 0, std::ios::end );

        // Read and write the file data
        std::ifstream assetFile( file, std::ios::binary );
        if ( !assetFile.is_open() )
        {
            std::cerr << "Failed to open asset file: " << file << std::endl;
            return false;
        }

        bundle << assetFile.rdbuf();

        // Update dataOffset for the next asset
        dataOffset += assetSize;
    }

    return true;
}
