#pragma once

#include <Asset/AssetBundleHeader.h>

namespace Shinkiro::Asset
{
    AssetBundleHeader::AssetBundleHeader()
        : m_Magic( 0x424E444C ), m_Version( 1 ), m_AssetCount( 0 ), m_HeaderSize( sizeof( AssetBundleHeader ) )
    {
    }
}
