#ifdef PLATFORM_WINDOWS
#    ifdef CORE_ASSET_EXPORTS
#        define ASSET_API __declspec( dllexport )
#    else
#        define ASSET_API __declspec( dllimport )
#    endif
#else
#    define ASSET_API
#endif

#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>

struct AssetEntry
{
    std::string name;   // Asset filename
    uint64_t    offset; // Offset in the bundle file
    uint64_t    size;   // Size of the asset in bytes

    AssetEntry( const std::string & n, uint64_t o, uint64_t s )
        : name( n ), offset( o ), size( s )
    {
    }
};

class ASSET_API AssetBundleManager
{
public:
    AssetBundleManager( const std::string & BundleName );

public:
    void SetExtractionPath( const std::filesystem::path & path );
    bool LoadBundleInfo();

    std::vector<std::string> GetAssetList();
    std::vector<uint8_t>     ExtractAssetToMemory( const std::string & assetName );

    std::filesystem::path                        ExtractAssetToFile( const std::string & assetName );
    std::map<std::string, std::filesystem::path> ExtractAllAssets();

    static bool CreateBundle( const fs::path & inputDir, const fs::path & outputPath );

public:
    std::filesystem::path GetBundlePath() const
    {
        return bundlePath;
    }

    std::string GetBundleName() const
    {
        return bundleName;
    }

private:
    std::filesystem::path   bundlePath;
    std::string             bundleName;
    std::vector<AssetEntry> assets;
    std::filesystem::path   extractionPath;
};
