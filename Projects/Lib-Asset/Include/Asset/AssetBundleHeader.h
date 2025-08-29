#pragma once

#ifndef SHINKIRO_ASSET_ASSETBUNDLEHEADER_H
#    define SHINKIRO_ASSET_ASSETBUNDLEHEADER_H

#    include <Asset/_Defs.h>

#    include <Log/Log.h>

namespace Shinkiro::Asset
{
    class ASSET_API AssetBundleHeader
    {
    public:
        AssetBundleHeader();

    public:
        /**
         * @brief Displays the bundle header information
         */
        void DisplayBundleInfo() const
        {
            SHNK_CORE_INFO( "Bundle Info:" );
            SHNK_CORE_INFO( "  Magic: 0x{0:X}", m_Magic );
            SHNK_CORE_INFO( "  Version: {0}", m_Version );
            SHNK_CORE_INFO( "  Asset Count: {0}", m_AssetCount );
            SHNK_CORE_INFO( "  Header Size: {0} bytes", m_HeaderSize );
        }

        bool IsValid() const
        {
            return m_Magic == 0x53484E4B && m_Version > 0;
        }

        uint32_t GetMagic() const
        {
            return m_Magic;
        }

        uint32_t GetVersion() const
        {
            return m_Version;
        }

        uint32_t GetAssetCount() const
        {
            return m_AssetCount;
        }

        void SetAssetCount( uint32_t count )
        {
            m_AssetCount = count;
        }

        uint32_t GetHeaderSize() const
        {
            return m_HeaderSize;
        }

    private:
        uint32_t m_Magic;      // Magic number to identify bundle format (e.g., 'SHNK')
        uint32_t m_Version;    // Bundle format version
        uint32_t m_AssetCount; // Number of assets in the bundle
        uint32_t m_HeaderSize; // Size of the header in bytes
    };
}

#endif
