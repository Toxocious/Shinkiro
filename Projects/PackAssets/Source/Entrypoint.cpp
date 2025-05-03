#include <Asset/_FilePath.h>

#include <Asset/Asset.h>
#include <Asset/AssetBundle.h>
#include <Asset/FontAsset.h>
#include <Asset/TextureAsset.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct AssetPathInfo
{
    fs::path  path;
    AssetType type;
};

int main()
{
    // Initialize timer.
    std::chrono::time_point<std::chrono::high_resolution_clock> timer = std::chrono::high_resolution_clock::now();

    std::cout << "Packing assets";
    {
        // Get current executable paths.
        fs::path exePath = GetExecutablePath();
        fs::path exeDir  = GetExecutableDirectory();

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

            if ( !fs::exists( assetInfo.path ) )
            {
                std::cerr << "\t [ ~ ] Directory not found: " << assetInfo.path << std::endl;
                continue;
            }

            for ( const auto & entry : fs::directory_iterator( assetInfo.path ) )
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

            fs::path outputFile = outputDir / ( "Shinkiro" + bundleType + ".assetbundle" );
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

    auto endTime = ( std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now() - timer ).count() * 0.001f * 0.001f * 0.001f ) * 1000.0f;

    std::cout << "Asset packing complete (" << endTime << " ms)\n";

    MessageBoxA( NULL, "Asset packing complete!", "PackAssets", MB_OK | MB_ICONINFORMATION );

    return 0;
}
