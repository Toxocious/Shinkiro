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

#include <filesystem>
#include <fstream>
#include <stack>

class ASSET_API AssetBundleHeader
{
public:
    AssetBundleHeader();

public:
    bool isValid() const
    {
        return m_Magic == 0x424E444C && m_Version > 0; // 'BNDL' in hex
    }

    uint32_t getMagic() const
    {
        return m_Magic;
    }

    uint32_t getVersion() const
    {
        return m_Version;
    }

    uint32_t getAssetCount() const
    {
        return m_AssetCount;
    }

    void setAssetCount( uint32_t count )
    {
        m_AssetCount = count;
    }

    uint32_t getHeaderSize() const
    {
        return m_HeaderSize;
    }

private:
    uint32_t m_Magic;      // Magic number to identify bundle format (e.g., 'BNDL')
    uint32_t m_Version;    // Bundle format version
    uint32_t m_AssetCount; // Number of assets in the bundle
    uint32_t m_HeaderSize; // Size of the header in bytes
};
