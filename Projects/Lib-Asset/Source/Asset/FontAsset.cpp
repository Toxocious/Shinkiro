#include <Asset/FontAsset.h>

#include <fstream>
#include <stdexcept>

FontAsset::FontAsset( const std::string & name )
    : Asset( name, AssetType::Font ), m_Path( name )
{
}

bool FontAsset::LoadFromFile( const std::string & filepath )
{
    std::ifstream file( filepath, std::ios::binary );
    if ( !file.is_open() )
    {
        return false;
    }

    // Get file size
    file.seekg( 0, std::ios::end );
    size_t fileSize = file.tellg();
    file.seekg( 0, std::ios::beg );

    // Read entire file
    m_FontData.resize( fileSize );
    file.read( reinterpret_cast<char *>( m_FontData.data() ), fileSize );

    return true;
}

const std::vector<uint8_t> & FontAsset::GetFontData() const
{
    return m_FontData;
}

void FontAsset::Serialize( std::vector<char> & data ) const
{
    // Calculate total size needed
    size_t dataSize   = m_FontData.size();
    size_t familySize = m_FontFamily.size();
    size_t styleSize  = m_FontStyle.size();

    // Reserve space
    data.resize( sizeof( size_t ) * 3 + dataSize + familySize + styleSize );
    char * ptr = data.data();

    // Write sizes
    memcpy( ptr, &dataSize, sizeof( size_t ) );
    ptr += sizeof( size_t );
    memcpy( ptr, &familySize, sizeof( size_t ) );
    ptr += sizeof( size_t );
    memcpy( ptr, &styleSize, sizeof( size_t ) );
    ptr += sizeof( size_t );

    // Write font data
    memcpy( ptr, m_FontData.data(), dataSize );
    ptr += dataSize;

    // Write strings
    memcpy( ptr, m_FontFamily.data(), familySize );
    ptr += familySize;
    memcpy( ptr, m_FontStyle.data(), styleSize );
}

void FontAsset::Deserialize( const std::vector<char> & data )
{
    if ( data.size() < sizeof( size_t ) * 3 )
    {
        throw std::runtime_error( "Invalid font asset data" );
    }

    const char * ptr = data.data();

    // Read sizes
    size_t dataSize, familySize, styleSize;
    memcpy( &dataSize, ptr, sizeof( size_t ) );
    ptr += sizeof( size_t );
    memcpy( &familySize, ptr, sizeof( size_t ) );
    ptr += sizeof( size_t );
    memcpy( &styleSize, ptr, sizeof( size_t ) );
    ptr += sizeof( size_t );

    // Verify total size
    if ( data.size() != sizeof( size_t ) * 3 + dataSize + familySize + styleSize )
    {
        throw std::runtime_error( "Invalid font asset data size" );
    }

    // Read font data
    m_FontData.resize( dataSize );
    memcpy( m_FontData.data(), ptr, dataSize );
    ptr += dataSize;

    // Read strings
    m_FontFamily.assign( ptr, familySize );
    ptr += familySize;
    m_FontStyle.assign( ptr, styleSize );
}
