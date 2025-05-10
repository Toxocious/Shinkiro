#include <Asset/_FilePath.h>

#include <Asset/Asset.h>
#include <Asset/AssetBundle.h>

#include <Asset/AssetBundleManager.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

void ShowAndUnpackBundlePackContents( const std::string & bundleName )
{
    std::cout << "Bundle name: " << bundleName << std::endl;
    std::cout << "Bundle contents:" << std::endl;

    {
        AssetBundleManager bundleManager( bundleName );
        const auto         bundlePath = bundleManager.GetBundlePath();
        if ( std::filesystem::exists( bundlePath ) )
        {
            if ( bundleManager.LoadBundleInfo() )
            {
                std::cout << "Bundle loaded successfully" << std::endl;

                // List all assets
                std::cout << "Assets in bundle:" << std::endl;
                for ( const auto & assetName : bundleManager.GetAssetList() )
                {
                    std::cout << " - " << assetName << std::endl;
                }

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
    std::chrono::time_point<std::chrono::high_resolution_clock> timer =
        std::chrono::high_resolution_clock::now();

    ShowAndUnpackBundlePackContents( "assets.bundle" );

    auto endTime = ( std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now() - timer ).count() * 0.001f * 0.001f * 0.001f ) * 1000.0f;

    std::cout << "Asset packing complete (" << endTime << " ms)\n\n\n";

    return 0;
}
