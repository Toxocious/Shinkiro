#include <Asset/Asset.h>

Asset::Asset( const std::string & name, AssetType type )
    : m_Name( name ), m_Type( type )
{
}

Asset::~Asset()
{
}

const std::string & Asset::GetName() const
{
    return m_Name;
}

AssetType Asset::GetType() const
{
    return m_Type;
}
