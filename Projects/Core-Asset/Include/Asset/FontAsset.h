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

#include <string>
#include <vector>

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

private:
    std::vector<uint8_t> m_FontData;
    std::string          m_FontFamily;
    std::string          m_FontStyle;
};
