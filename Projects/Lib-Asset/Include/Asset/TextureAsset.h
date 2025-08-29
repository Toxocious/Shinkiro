#pragma once

#ifndef SHINKIRO_ASSET_TEXTUREASSET_H
#    define SHINKIRO_ASSET_TEXTUREASSET_H

#    include <Asset/_Defs.h>

#    include <Asset/Asset.h>

#    include <string>
#    include <vector>

namespace Shinkiro::Asset
{
    enum class TextureFormat
    {
        RGB8,
        RGBA8,
        BGR8,
        BGRA8
    };

    class ASSET_API TextureAsset : public Asset
    {
    public:
        /**
         * @brief Constructs a new TextureAsset
         * @param name The name of the texture asset
         */
        TextureAsset( const std::string & name );
        ~TextureAsset() override = default;

    public:
        /**
         * @brief Serializes the texture asset to a byte vector
         * @param data The vector to store the serialized data
         */
        void Serialize( std::vector<char> & data ) const override;

        /**
         * @brief Deserializes the texture asset from a byte vector
         * @param data The vector containing the serialized data
         * @throws A runtime error if the texture asset data size is invalid
         */
        void Deserialize( const std::vector<char> & data ) override;

        /**
         * @brief Load texture data from a file
         * @param filepath The path to the texture file
         * @return True if the texture was loaded successfully, false otherwise
         */
        bool LoadFromFile( const std::string & filepath );

    public:
        /**
         * @brief Get the width of the texture in pixels
         * @return The width of the texture
         */
        uint32_t GetWidth() const
        {
            return m_Width;
        }

        /**
         * @brief Get the height of the texture in pixels
         * @return The height of the texture
         */
        uint32_t GetHeight() const
        {
            return m_Height;
        }

        /**
         * @brief Get the number of color channels in the texture
         * @return The number of channels (e.g., 3 for RGB, 4
         */
        uint32_t GetChannels() const
        {
            return m_Channels;
        }

        /**
         * @brief Get the texture format
         * @return The TextureFormat enum value
         */
        TextureFormat GetFormat() const
        {
            return m_Format;
        }

        /**
         * @brief Check if the texture has mipmaps
         * @return True if the texture has mipmaps, false otherwise
         */
        const std::vector<uint8_t> & GetPixelData() const
        {
            return m_PixelData;
        }

        /**
         * @brief Get the path of the texture asset
         * @return The path of the texture as a string
         */
        const std::string & GetPath() const
        {
            return m_Path;
        }

    private:
        std::string m_Path;

        std::vector<uint8_t> m_PixelData;
        uint32_t             m_Width;
        uint32_t             m_Height;
        uint32_t             m_Channels;
        TextureFormat        m_Format;
        bool                 m_HasMipmaps;
    };
}

#endif
