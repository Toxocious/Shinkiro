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

#include <vector>

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
    TextureAsset( const std::string & name );
    ~TextureAsset() override = default;

    // Asset interface implementation
    void Serialize( std::vector<char> & data ) const override;
    void Deserialize( const std::vector<char> & data ) override;

    // Custom methods
    bool LoadFromFile( const std::string & filepath );

    uint32_t                     GetWidth() const;
    uint32_t                     GetHeight() const;
    uint32_t                     GetChannels() const;
    TextureFormat                GetFormat() const;
    const std::vector<uint8_t> & GetPixelData() const;

public:
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
