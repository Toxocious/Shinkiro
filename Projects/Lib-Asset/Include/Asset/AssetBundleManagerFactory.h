#pragma once

#ifndef SHINKIRO_ASSET_ASSETBUNDLEMANAGERFACTORY_H
#    define SHINKIRO_ASSET_ASSETBUNDLEMANAGERFACTORY_H

#    include <Asset/_Defs.h>

#    include <Core/Interfaces/AssetBundleManagerInterface.h>

#    include <memory>
#    include <string>

namespace Shinkiro::Asset
{
    ASSET_API std::unique_ptr<IAssetBundleManager> CreateAssetBundleManager( const std::string & bundleName );
}

#endif
