#include <Asset/Asset.h>
#include <Asset/FontAsset.h>
#include <Asset/TextureAsset.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int main()
{
    const std::string assetsDir = "Assets";
    const std::string outputDir = "PackedAssets";

    std::cout << "Packing assets from " << assetsDir << " to " << outputDir << "...\n";
    {
    }
    std::cout << "Asset packing complete!\n";

    return 0;
}
