#pragma once

#ifndef SHINKIRO_ASSET_ASSETBUNDLEHEADER_H
#    define SHINKIRO_ASSET_ASSETBUNDLEHEADER_H

#    include <Asset/_Defs.h>

#    include <filesystem>
#    include <fstream>
#    include <iostream>
#    include <stack>

namespace Shinkiro::Asset
{
    class ASSET_API AssetBundleHeader
    {
    public:
        AssetBundleHeader();

    public:
        void DisplayBundleInfo() const
        {
            std::cout << "Bundle Info:" << std::endl;
            std::cout << "  Magic: 0x" << std::hex << m_Magic << std::dec << std::endl;
            std::cout << "  Version: " << m_Version << std::endl;
            std::cout << "  Asset Count: " << m_AssetCount << std::endl;
            std::cout << "  Header Size: " << m_HeaderSize << " bytes" << std::endl;
        }

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
}

#endif
