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

#include <Asset/Asset.h>
#include <Asset/FontAsset.h>
#include <Asset/TextureAsset.h>

#include <filesystem>
#include <fstream>
#include <stack>

class ASSET_API AssetBundle
{
public:
    AssetBundle( const std::string & name, AssetType type );
    ~AssetBundle() = default;

public:
    // template <AssetType Type>
    // bool AddAsset( const std::filesystem::path & assetPath );

    bool AddAssetPath( const std::filesystem::path & assetPath );

    bool PackAssets( const std::filesystem::path & outputPath );

public:
    template <AssetType Type>
    inline bool AddAsset( const std::filesystem::path & assetPath )
    {
        static_assert( false, "Unsupported asset type" );
        return false;
    }

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
    const std::string & GetName() const
    {
        return m_Name;
    };

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
