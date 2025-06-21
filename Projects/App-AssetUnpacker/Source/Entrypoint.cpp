#include <Asset/_FilePath.h>

#include <Asset/Asset.h>
#include <Asset/AssetBundle.h>
#include <Asset/Log.h>

#include <Asset/AssetBundleManager.h>

#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

void ShowAndUnpackBundlePackContents( const std::string & bundleName )
{
    std::cout << "Bundle name: " << bundleName << std::endl;
    std::cout << "Bundle contents:" << std::endl;
    {
        Shinkiro::Asset::AssetBundleManager bundleManager( bundleName );
        const auto                          bundlePath = bundleManager.GetBundlePath();

        if ( std::filesystem::exists( bundlePath ) )
        {
            if ( bundleManager.LoadBundleInfo() )
            {
                std::cout << "Bundle loaded successfully" << std::endl;
                auto assetList = bundleManager.GetAssetList();
                std::cout << "Found " << assetList.size() << " assets in bundle" << std::endl;

                // Extract all assets
                std::cout << "\nExtracting all assets..." << std::endl;
                auto extractedPaths = bundleManager.ExtractAllAssets();
                std::cout << "\nExtracted " << extractedPaths.size() << " assets" << std::endl;
            }
            else
            {
                std::cerr << "Failed to load bundle information" << std::endl;
            }
        }
        else
        {
            std::cout << "Bundle file not found: " << bundlePath << std::endl;
        }
    }
}

int main( int argc, char * argv[] )
{
    Shinkiro::Asset::Log::Init( "AssetUnpacker.log" );

    std::chrono::time_point<std::chrono::high_resolution_clock> timer =
        std::chrono::high_resolution_clock::now();

    ShowAndUnpackBundlePackContents( "assets.bundle" );

    auto endTime = ( std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now() - timer ).count() * 0.001f * 0.001f * 0.001f ) * 1000.0f;

    std::cout << "Asset unpacking complete (" << endTime << " ms)\n\n\n";

    Shinkiro::Asset::Log::Shutdown();

    return 0;
}
