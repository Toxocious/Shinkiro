<div align="center">
  <!-- <img src="..." title="Shinkiro Logo" alt="Shinkiro Logo" /> -->
  <h1 align="center">Shinkiro's FBX Updater</h1>

  **Shinkiro's FBX Updater** is a tool that allows you to upgrade older version .fbx files into newer version .fbx files.

  <img alt="Github Issues" src="https://img.shields.io/github/issues/Toxocious/Shinkiro?style=for-the-badge&logo=appveyor" />
  <img alt="Github Forks" src="https://img.shields.io/github/forks/Toxocious/Shinkiro?style=for-the-badge&logo=appveyor" />
  <img alt="Github Stars" src="https://img.shields.io/github/stars/Toxocious/Shinkiro?style=for-the-badge&logo=appveyor" />
  <br />

  <img alt="GitHub contributors" src="https://img.shields.io/github/contributors/Toxocious/Shinkiro?style=for-the-badge">
    <a href="https://visitorbadge.io/status?path=https%3A%2F%2Fgithub.com%2FToxocious%Shinkiro">
    <img src="https://api.visitorbadge.io/api/visitors?path=https%3A%2F%2Fgithub.com%2FToxocious%Shinkiro&label=Views&countColor=%234a618f&labelStyle=upper" />
  </a>
  <br />

  <img alt="License" src="https://img.shields.io/github/license/Toxocious/Shinkiro?style=for-the-badge&logo=appveyor" />

  Come join our comfy community over on Discord!

  <a href="https://discord.gg/NRZ2zWfpwK" target="_blank">
    <img src="https://discord.com/api/guilds/1002005327555862620/widget.png?style=banner2" alt="Discord Banner" />
  </a>
</div>



# Table of Contents
- [Table of Contents](#table-of-contents)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Output Structure](#output-structure)
- [License](#license)
- [Legal Notice](#legal-notice)



# Features
- Multi-threaded downloads for optimal performance
- Automatic directory creation and organization
- Configurable asset categories
- CPU-aware thread management



# Requirements
- Node.js (v14 or higher recommended)
- NPM or Yarn package manager
- [FBX Converter](https://aps.autodesk.com/developer/overview/fbx-converter-archives) v2013.3



# Installation
1. Navigate to the Tools/Fbx_Version_Updater directory
2. Install dependencies:

```bash
npm install
```



# Usage
Run the tool with:

```typescript
npx ts-node Fbx_Version_Updater.ts
```

The script expects your models to be any subdirectory of [./Input](./Input).

> NOTICE: To run the script, we expect FBX Converter to be installed to `"C:\\Program Files\\Autodesk\\FBX\\FBX Converter\\2013.3\\bin\\FBXConverter.exe"`.



# Output Structure
Converted models are organized into the following directory structure:

```
./Output
```



# License
This tool is part of the Shinkiro project and is subject to the license terms specified in the main [LICENSE](../../LICENSE.md)



# Legal Notice
I do **not** claim to own any assets that are converted by the use of this tool.

All Pokémon sprites and assets are owned by The Pokémon Company International and Nintendo.

Please respect copyright laws when using downloaded and converted assets.
