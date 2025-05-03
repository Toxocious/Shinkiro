#include <Asset/TextureAsset.h>

#include <fstream>
#include <stdexcept>

#include <stb_image/stb_image.h>

TextureAsset::TextureAsset( const std::string & name )
    : Asset( name, AssetType::Texture ), m_Width( 0 ), m_Height( 0 ), m_Channels( 0 ), m_Format( TextureFormat::RGBA8 ), m_HasMipmaps( false ), m_Path( name )
{
}

bool TextureAsset::LoadFromFile( const std::string & filepath )
{
    int       width, height, channels;
    uint8_t * data = stbi_load( filepath.c_str(), &width, &height, &channels, 0 );
    if ( !data )
    {
        return false;
    }

    m_Width    = width;
    m_Height   = height;
    m_Channels = channels;
    m_Format   = channels == 4 ? TextureFormat::RGBA8 : TextureFormat::RGB8;

    size_t dataSize = width * height * channels;
    m_PixelData.resize( dataSize );
    memcpy( m_PixelData.data(), data, dataSize );

    stbi_image_free( data );

    return true;
}

uint32_t TextureAsset::GetWidth() const
{
    return m_Width;
}

uint32_t TextureAsset::GetHeight() const
{
    return m_Height;
}

uint32_t TextureAsset::GetChannels() const
{
    return m_Channels;
}

TextureFormat TextureAsset::GetFormat() const
{
    return m_Format;
}

const std::vector<uint8_t> & TextureAsset::GetPixelData() const
{
    return m_PixelData;
}

void TextureAsset::Serialize( std::vector<char> & data ) const
{
    // Calculate total size needed
    size_t pixelDataSize = m_PixelData.size();
    size_t headerSize    = sizeof( uint32_t ) * 3 + sizeof( TextureFormat ) + sizeof( bool );

    // Reserve space
    data.resize( headerSize + pixelDataSize );
    char * ptr = data.data();

    // Write header
    memcpy( ptr, &m_Width, sizeof( uint32_t ) );
    ptr += sizeof( uint32_t );
    memcpy( ptr, &m_Height, sizeof( uint32_t ) );
    ptr += sizeof( uint32_t );
    memcpy( ptr, &m_Channels, sizeof( uint32_t ) );
    ptr += sizeof( uint32_t );
    memcpy( ptr, &m_Format, sizeof( TextureFormat ) );
    ptr += sizeof( TextureFormat );
    memcpy( ptr, &m_HasMipmaps, sizeof( bool ) );
    ptr += sizeof( bool );

    // Write pixel data
    memcpy( ptr, m_PixelData.data(), pixelDataSize );
}

void TextureAsset::Deserialize( const std::vector<char> & data )
{
    size_t headerSize = sizeof( uint32_t ) * 3 + sizeof( TextureFormat ) + sizeof( bool );
    if ( data.size() < headerSize )
    {
        throw std::runtime_error( "Invalid texture asset data" );
    }

    const char * ptr = data.data();

    // Read header
    memcpy( &m_Width, ptr, sizeof( uint32_t ) );
    ptr += sizeof( uint32_t );
    memcpy( &m_Height, ptr, sizeof( uint32_t ) );
    ptr += sizeof( uint32_t );
    memcpy( &m_Channels, ptr, sizeof( uint32_t ) );
    ptr += sizeof( uint32_t );
    memcpy( &m_Format, ptr, sizeof( TextureFormat ) );
    ptr += sizeof( TextureFormat );
    memcpy( &m_HasMipmaps, ptr, sizeof( bool ) );
    ptr += sizeof( bool );

    // Read pixel data
    size_t pixelDataSize = data.size() - headerSize;
    m_PixelData.resize( pixelDataSize );
    memcpy( m_PixelData.data(), ptr, pixelDataSize );
}
