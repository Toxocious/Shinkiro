#pragma once

#ifndef SHINKIRO_ASSET_ASSET_H
#    define SHINKIRO_ASSET_ASSET_H

#    include <Asset/_Defs.h>

#    include <filesystem>
#    include <string>
#    include <vector>

namespace Shinkiro::Asset
{
    enum class ASSET_API AssetType
    {
        Font,
        Audio,
        Model,
        Texture
    };

    class ASSET_API Asset
    {
    public:
        /**
         * @brief Constructs a new Asset
         * @param name The name of the asset
         * @param type The type of asset
         */
        Asset( const std::string & name, AssetType type );
        virtual ~Asset();

    public:
        /**
         * @brief Gets the name of the current asset
         * @return The name of the asset
         */
        const std::string & GetName() const;

        /**
         * @brief Gets the AssetType of the current asset
         * @return The AssetType of the asset
         */
        AssetType GetType() const;

        /**
         * @brief Serializes an asset given byte data
         * @param data The bytes of an asset
         */
        virtual void Serialize( std::vector<char> & data ) const   = 0;

        /**
         * @brief Deserializes an asset given byte data
         * @param data The bytes of an asset
         */
        virtual void Deserialize( const std::vector<char> & data ) = 0;

    protected:
        std::string m_Name;
        AssetType   m_Type;
    };
}

#endif
