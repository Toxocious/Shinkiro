#include <Asset/_FilePath.h>

#include <Asset/Asset.h>
#include <Asset/AssetBundle.h>

#include <Asset/AssetBundleManager.h>

#include <Asset/FontAsset.h>
#include <Asset/TextureAsset.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

struct AssetPathInfo
{
    std::filesystem::path path;
    AssetType             type;
};

void OriginalPackAssets()
{
    std::cout << "Packing assets";
    {
        // Get current executable paths.
        std::filesystem::path exePath = GetExecutablePath();
        std::filesystem::path exeDir  = GetExecutableDirectory();

        std::cout << "Executable path: " << exePath << std::endl;
        std::cout << "Executable directory: " << exeDir << std::endl;

        // Get asset directory.
        const auto assetsDir = exeDir / "Assets";
        const auto outputDir = exeDir / "PackedAssets";

        // Prep asset directory paths.
        std::vector<AssetPathInfo> assetPaths {
            { assetsDir / "Fonts",    AssetType::Font    },
            { assetsDir / "Textures", AssetType::Texture }
        };

        // After bundles declaration:
        for ( const auto & assetInfo : assetPaths )
        {
            std::string bundleType;
            switch ( assetInfo.type )
            {
                case AssetType::Font:
                    bundleType = "Fonts";
                    break;
                case AssetType::Texture:
                    bundleType = "Textures";
                    break;
                default:
                    bundleType = "Unknown";
                    break;
            }
            std::cout << "\t [ ~ ] Packing " << bundleType << " assets..." << std::endl;

            AssetBundle assetBundle = AssetBundle( assetInfo.path.stem().string(), assetInfo.type );

            if ( !std::filesystem::exists( assetInfo.path ) )
            {
                std::cerr << "\t [ ~ ] Directory not found: " << assetInfo.path << std::endl;
                continue;
            }

            for ( const auto & entry : std::filesystem::directory_iterator( assetInfo.path ) )
            {
                switch ( assetInfo.type )
                {
                    case AssetType::Font:
                        assetBundle.AddAsset<AssetType::Font>( entry.path() );
                        break;

                    case AssetType::Texture:
                        assetBundle.AddAsset<AssetType::Texture>( entry.path() );
                        break;

                    default:
                        std::cerr << "\t [ ~ ] Unsupported asset type for: " << entry.path() << std::endl;
                        break;
                }
            }

            std::filesystem::path outputFile = outputDir / ( "Shinkiro" + bundleType + ".assetbundle" );
            if ( !assetBundle.PackAssets( outputFile ) )
            {
                std::cerr << "\t [ ~ ] Failed to pack " << bundleType << " assets!" << std::endl;
            }
            else
            {
                std::cout << "\t [ ~ ] Packed " << bundleType << " assets to: " << outputFile << std::endl;
            }
        }
    }
}

void NewPackAssets()
{
    std::cout << "Packing assets";
    {
        // Get current executable paths.
        std::filesystem::path exePath = GetExecutablePath();
        std::filesystem::path exeDir  = GetExecutableDirectory();

        std::cout << "Executable path: " << exePath << std::endl;
        std::cout << "Executable directory: " << exeDir << std::endl;

        // Get asset directory.
        const auto assetsDir = exeDir / "Assets";
        const auto outputDir = exeDir / "PackedAssets";

        // Example: Create a bundle from the "assets" directory in the executable directory
        std::filesystem::path bundlePath = exeDir / "assets.bundle";

        if ( std::filesystem::exists( assetsDir ) && std::filesystem::is_directory( assetsDir ) )
        {
            std::cout << "Creating bundle from assets directory..." << std::endl;
            if ( AssetBundleManager::CreateBundle( assetsDir, bundlePath ) )
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

void ShowBundlePackContents( std::filesystem::path & bundlePath )
{
    std::cout << "Bundle path: " << bundlePath << std::endl;
    std::cout << "Bundle contents:" << std::endl;

    {
        AssetBundleManager bundleManager( "assets.bundle" );
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
                // std::cout << "Extracting all assets..." << std::endl;
                // auto extractedPaths = bundleManager.ExtractAllAssets();
                // std::cout << "Extracted " << extractedPaths.size() << " assets" << std::endl;
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

int main()
{
    /**
     * Pack assets.
     */
    {
        // Initialize timer.
        std::chrono::time_point<std::chrono::high_resolution_clock> timer = std::chrono::high_resolution_clock::now();

        {
            NewPackAssets();
        }

        auto endTime = ( std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now() - timer ).count() * 0.001f * 0.001f * 0.001f ) * 1000.0f;

        std::cout << "Asset packing complete (" << endTime << " ms)\n\n\n";
    }

    /**
     * Display packed asset bundle contents.
     */
    {
        ShowBundlePackContents( GetExecutableDirectory() / "assets.bundle" );
    }

    MessageBoxA( NULL, "Asset packing complete!", "PackAssets", MB_OK | MB_ICONINFORMATION );

    return 0;
}
