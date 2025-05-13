#include <Asset/Asset.h>

namespace Shinkiro::Asset
{
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
}
