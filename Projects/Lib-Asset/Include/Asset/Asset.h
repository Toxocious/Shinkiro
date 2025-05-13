#pragma once

#ifndef SHINKIRO_ASSET_ASSET_H
#    define SHINKIRO_ASSET_ASSET_H

#    include <Asset/_Defs.h>

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
}

#endif
