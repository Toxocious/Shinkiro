#pragma once

#include <Asset/AssetBundleHeader.h>

namespace Shinkiro::Asset
{
    AssetBundleHeader::AssetBundleHeader()
        : m_Magic( 0x53484E4B ), m_Version( 2 ), m_AssetCount( 0 ), m_HeaderSize( sizeof( AssetBundleHeader ) )
    {
    }
}
