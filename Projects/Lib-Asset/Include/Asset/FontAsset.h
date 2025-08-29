#pragma once

#ifndef SHINKIRO_ASSET_FONTASSET_H
#    define SHINKIRO_ASSET_FONTASSET_H

#    include <Asset/_Defs.h>

#    include <Asset/Asset.h>

#    include <string>
#    include <vector>

namespace Shinkiro::Asset
{
    class ASSET_API FontAsset : public Asset
    {
    public:
        /**
         * @brief Constructs a new FontAsset
         * @param name The name of the font asset
         */
        FontAsset( const std::string & name );
        ~FontAsset() override = default;

    public:
        /**
         * @brief Serializes the font asset to a byte vector
         * @param data The vector to store the serialized data
         */
        void Serialize( std::vector<char> & data ) const override;

        /**
         * @brief Deserializes the font asset from a byte vector
         * @param data The vector containing the serialized
         * @throws A runtime error if the font asset data size is invalid
         */
        void Deserialize( const std::vector<char> & data ) override;

    public:
        /**
         * @brief Load font data from a file
         * @param filepath The path to the font file
         * @return True if the font was loaded successfully, false otherwise
         */
        bool LoadFromFile( const std::string & filepath );

        /**
         * @brief Get the raw font data
         * @return A reference to the vector containing the font data in bytes
         */
        const std::vector<uint8_t> & GetFontData() const;

    public:
        /**
         * @brief Get the file path of the font asset
         * @return The file path of the font asset
         */
        const std::string & GetPath() const
        {
            return m_Path;
        }

    private:
        std::string m_Path;

        std::vector<uint8_t> m_FontData;
        std::string          m_FontFamily;
        std::string          m_FontStyle;
    };
}

#endif
