#pragma once

#ifndef SHINKIRO_ASSET_ASSETBUNDLEMANAGER_H
#    define SHINKIRO_ASSET_ASSETBUNDLEMANAGER_H

#    include <Asset/_Defs.h>

#    include <Core/Interfaces/AssetBundleManagerInterface.h>

#    include <filesystem>
#    include <map>
#    include <string>
#    include <unordered_map>
#    include <vector>

namespace Shinkiro::Asset
{
    struct AssetEntry
    {
        std::string name;             // Asset filename
        uint64_t    offset;           // Offset in the bundle file
        uint64_t    compressedSize;   // Size of the compressed asset in bytes
        uint64_t    uncompressedSize; // Size of the original asset in bytes

        /**
         * @brief Constructs a new AssetEntry
         * @param assetName The name of the asset
         * @param assetOffset The offset of the asset in the bundle
         * @param assetCompressedSize The size of the compressed asset
         * @param assetUncompressedSize The size of the uncompressed asset
         */
        AssetEntry( const std::string & assetName, uint64_t assetOffset, uint64_t assetCompressedSize, uint64_t assetUncompressedSize )
            : name( assetName ), offset( assetOffset ), compressedSize( assetCompressedSize ), uncompressedSize( assetUncompressedSize )
        {
        }
    };

    class ASSET_API AssetBundleManager : public IAssetBundleManager
    {
    public:
        /**
         * @brief Constructor for AssetBundleManager.
         * @param BundleName The name of the asset bundle to manage.
         */
        AssetBundleManager( const std::string & BundleName );

    public:
        /**
         * @brief Set the extraction path for unpacked assets.
         * @param path The directory where assets will be extracted.
         */
        void SetExtractionPath( const std::filesystem::path & path ) override;

        /**
         * @brief Load the asset bundle header and asset entries from the bundle file.
         * @return True if the bundle was loaded successfully, false otherwise.
         */
        bool LoadBundleInfo() override;

        /**
         * @brief Load all assets from the asset bundle into memory.
         * @throws std::runtime_error if any asset is not found or the bundle cannot be opened.
         */
        void LoadAssetsIntoMemory() override;

        /**
         * @brief Get a list of all asset names in the bundle.
         * @return A vector of asset names.
         */
        std::vector<std::string> GetAssetList() override;

        /**
         * @brief Get the data for a specific asset by name.
         * @param assetName The name of the asset to retrieve.
         * @return A reference to the vector containing the asset data in bytes.
         */
        const std::vector<uint8_t> & GetAssetData( const std::string & assetName ) override;

        /**
         * @brief Extract a specific asset to memory.
         * @param assetName The name of the asset to extract.
         * @return A vector containing the asset data.
         * @throws std::runtime_error if the asset is not found or the bundle cannot be opened.
         */
        std::vector<uint8_t> ExtractAssetToMemory( const std::string & assetName ) override;

        /**
         * @brief Extract a specific asset to a file.
         * @param assetName The name of the asset to extract.
         * @return The path to the extracted file.
         * @throws std::runtime_error if the asset is not found or the output file cannot be created.
         */
        std::filesystem::path ExtractAssetToFile( const std::string & assetName ) override;

        /**
         * @brief Extract all assets in the bundle to files.
         * @return A map of asset names to their extracted file paths.
         */
        std::map<std::string, std::filesystem::path> ExtractAllAssets() override;

        /**
         * @brief Create an asset bundle from a directory of files.
         * @param inputDir The directory containing files to bundle.
         * @param outputPath The path where the bundle will be created.
         * @return True if the bundle was created successfully, false otherwise.
         */
        static bool CreateBundle( const std::filesystem::path & inputDir, const std::filesystem::path & outputPath );

    public:
        /**
         * @brief Get the path to the asset bundle file.
         * @return The file path to the asset bundle.
         */
        std::filesystem::path GetBundlePath() const override
        {
            return m_BundlePath;
        }

        /**
         * @brief Get the name of the asset bundle.
         * @return The name of the asset bundle.
         */
        std::string GetBundleName() const override
        {
            return m_BundleName;
        }

    private:
        std::filesystem::path   m_BundlePath;
        std::string             m_BundleName;
        std::vector<AssetEntry> m_Assets;
        std::filesystem::path   m_ExtractionPath;

        std::unordered_map<std::string, std::vector<uint8_t>> m_AssetCache;
    };
}

#endif
