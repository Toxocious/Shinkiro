#pragma once

#ifndef SHINKIRO_ASSET_ASSETBUNDLE_H
#    define SHINKIRO_ASSET_ASSETBUNDLE_H

#    include <Asset/_Defs.h>

#    include <Asset/Asset.h>
#    include <Asset/FontAsset.h>
#    include <Asset/TextureAsset.h>

#    include <filesystem>
#    include <string>
#    include <vector>

namespace Shinkiro::Asset
{
    class ASSET_API AssetBundle
    {
    public:
        /**
         * @brief Constructs a new AssetBundle
         * @param name The name of the asset bundle
         * @param type The AssetType of the bundle
         */
        AssetBundle( const std::string & name, AssetType type );
        ~AssetBundle() = default;

    public:
        bool AddAssetPath( const std::filesystem::path & assetPath );

        bool PackAssets( const std::filesystem::path & outputPath );

    public:
        /**
         * @brief Adds an asset to the bundle
         * @param assetPath The file path to the asset
         * @return True if the asset was added successfully, false otherwise
         */
        template <AssetType Type>
        inline bool AddAsset( const std::filesystem::path & assetPath )
        {
            static_assert( false, "Unsupported asset type" );
            return false;
        }

        /**
         * @brief Specialization for adding Font assets
         * @param assetPath The file path to the font asset or directory containing font assets
         * @return True if the asset was added successfully, false otherwise
         */
        template <>
        inline bool AddAsset<AssetType::Font>( const std::filesystem::path & assetPath )
        {
            if ( !std::filesystem::exists( assetPath ) )
            {
                return false;
            }

            if ( std::filesystem::is_directory( assetPath ) )
            {
                for ( const auto & entry : std::filesystem::recursive_directory_iterator( assetPath ) )
                {
                    if ( entry.is_regular_file() && entry.path().extension().string() == ".ttf" )
                    {
                        m_FontAssets.emplace_back( entry.path().string() );
                        m_AssetPaths.push_back( entry.path() );
                    }
                }
                return true;
            }
            else if ( std::filesystem::is_regular_file( assetPath ) )
            {
                m_FontAssets.emplace_back( assetPath.string() );
                m_AssetPaths.push_back( assetPath );
                return true;
            }

            return false;
        }

        /**
         * @brief Specialization for adding Texture assets
         * @param assetPath The file path to the texture asset
         * @return True if the asset was added successfully, false otherwise
         */
        template <>
        inline bool AddAsset<AssetType::Texture>( const std::filesystem::path & assetPath )
        {
            if ( !std::filesystem::exists( assetPath ) )
            {
                return false;
            }

            m_TextureAssets.emplace_back( assetPath.string() );
            m_AssetPaths.push_back( assetPath );
            return true;
        }

    public:
        /**
         * @brief Gets the name of the asset bundle
         * @return The name of the asset bundle
         */
        const std::string & GetName() const
        {
            return m_Name;
        };

        /**
         * @brief Gets the AssetType of the asset bundle
         * @return The AssetType of the asset bundle
         */
        AssetType GetType() const
        {
            return m_Type;
        };

    protected:
        std::string m_Name;
        AssetType   m_Type;

        std::filesystem::path m_AssetDirectoryPath;

        //
        std::vector<std::filesystem::path> m_AssetPaths;

        //
        std::vector<FontAsset>    m_FontAssets;
        std::vector<TextureAsset> m_TextureAssets;
    };
}

#endif
