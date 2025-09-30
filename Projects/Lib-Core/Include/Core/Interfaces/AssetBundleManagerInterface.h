#pragma once

#ifndef SHINKIRO_ASSET_IASSETBUNDLEMANAGER_H
#    define SHINKIRO_ASSET_IASSETBUNDLEMANAGER_H

#    include <Core/_Defs.h>

#    include <cstdint>
#    include <filesystem>
#    include <map>
#    include <memory>
#    include <string>
#    include <vector>

namespace Shinkiro::Asset
{
    class CORE_API IAssetBundleManager
    {
    public:
        IAssetBundleManager();
        virtual ~IAssetBundleManager();

        // Lifecycle / loading
        virtual void SetExtractionPath( const std::filesystem::path & path ) = 0;
        virtual bool LoadBundleInfo()                                        = 0;
        virtual void LoadAssetsIntoMemory()                                  = 0;

        // Queries / access
        virtual std::vector<std::string>     GetAssetList()                                = 0;
        virtual const std::vector<uint8_t> & GetAssetData( const std::string & assetName ) = 0;

        // Extraction
        virtual std::vector<uint8_t>                         ExtractAssetToMemory( const std::string & assetName ) = 0;
        virtual std::filesystem::path                        ExtractAssetToFile( const std::string & assetName )   = 0;
        virtual std::map<std::string, std::filesystem::path> ExtractAllAssets()                                    = 0;

        // Metadata
        virtual std::filesystem::path GetBundlePath() const = 0;
        virtual std::string           GetBundleName() const = 0;
    };
}

#endif
