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
        FontAsset( const std::string & name );
        ~FontAsset() override = default;

        // Asset interface implementation
        void Serialize( std::vector<char> & data ) const override;
        void Deserialize( const std::vector<char> & data ) override;

        // Custom methods
        bool LoadFromFile( const std::string & filepath );

        const std::vector<uint8_t> & GetFontData() const;

    public:
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
