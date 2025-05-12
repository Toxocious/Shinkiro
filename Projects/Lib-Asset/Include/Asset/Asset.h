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

#include <string>
#include <vector>

enum class AssetType
{
    Font,
    Audio,
    Model,
    Texture
};

class ASSET_API Asset
{
public:
    Asset( const std::string & name, AssetType type );
    virtual ~Asset();

    const std::string & GetName() const;
    AssetType           GetType() const;

    virtual void Serialize( std::vector<char> & data ) const   = 0;
    virtual void Deserialize( const std::vector<char> & data ) = 0;

protected:
    std::string m_Name;
    AssetType   m_Type;
};
