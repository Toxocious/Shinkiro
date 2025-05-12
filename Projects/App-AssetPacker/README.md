<div align="center">
  <!-- <img src="." title="Shinkiro Logo" alt="Shinkiro Logo" /> -->
  <h1 align="center">Shinkiro</h1>
</div>



# Table of Contents
- [Table of Contents](#table-of-contents)
- [Shinkiro Asset Packer](#shinkiro-asset-packer)
- [License](#license)



# Shinkiro Asset Packer
This directory contains the source code for Shinkiro's asset packer.

When built, simply place your `Assets/` folder inside of the built asset packer directory and run the executable.

If the asset packing succeeds, you will find a new directory named `AssetBundles/` inside of the executable's directory which will contain a single asset bundle.

All files found within your `Assets/` folder - including within subdirectories - will be packed into a single asset bundle.

> Asset bundles are not currently obfuscated or encrypted, though it is being considered down the line.



# License
This file is licensed under MIT alongside the rest of the project.

For more information about the license, check out the [LICENSE](../../LICENSE.md).
