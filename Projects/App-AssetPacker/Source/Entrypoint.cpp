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

void PackAssets()
{
    std::cout << "Packing assets" << std::endl;
    {
        std::filesystem::path exePath = Shinkiro::Asset::GetExecutablePath();
        std::filesystem::path exeDir  = Shinkiro::Asset::GetExecutableDirectory();

        const auto assetsDir = exeDir / "Assets";
        const auto outputDir = exeDir / "AssetBundles";

        std::filesystem::path bundlePath = outputDir / "assets.bundle";
        if ( !std::filesystem::exists( outputDir ) )
        {
            std::filesystem::create_directories( outputDir );
        }

        if ( std::filesystem::exists( assetsDir ) && std::filesystem::is_directory( assetsDir ) )
        {
            std::cout << "Creating bundle from assets directory..." << std::endl;
            if ( Shinkiro::Asset::AssetBundleManager::CreateBundle( assetsDir, bundlePath ) )
            {
                std::cout << "Bundle created successfully at: " << bundlePath << std::endl;
            }
            else
            {
                std::cerr << "Failed to create bundle" << std::endl;
            }
        }
    }
}

void ShowBundlePackContents( const std::string & bundleName )
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

                std::cout << "Assets in bundle:" << std::endl;
                for ( const auto & assetName : bundleManager.GetAssetList() )
                {
                    std::cout << " - " << assetName << std::endl;
                }
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
    Shinkiro::Asset::Log::Init( "AssetPacker.log" );

    std::chrono::time_point<std::chrono::high_resolution_clock> timer =
        std::chrono::high_resolution_clock::now();

    PackAssets();

    auto endTime = ( std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now() - timer ).count() * 0.001f * 0.001f * 0.001f ) * 1000.0f;

    std::cout << "Asset packing complete (" << endTime << " ms)\n\n\n";

    ShowBundlePackContents( "assets.bundle" );

    Shinkiro::Asset::Log::Shutdown();

    return 0;
}
