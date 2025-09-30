#include <Asset/_Common.h>

#include <Asset/AssetBundleManager.h>
#include <Asset/AssetBundleManagerFactory.h>

namespace Shinkiro::Asset
{
    std::unique_ptr<IAssetBundleManager> CreateAssetBundleManager( const std::string & bundleName )
    {
        return std::make_unique<AssetBundleManager>( bundleName );
    }
}
