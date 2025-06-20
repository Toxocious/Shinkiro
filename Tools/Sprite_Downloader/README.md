<div align="center">
  <!-- <img src="..." title="Shinkiro Logo" alt="Shinkiro Logo" /> -->
  <h1 align="center">Shinkiro's Sprite Downloader</h1>

  **Shinkiro's Sprite Downloader** is a tool dedicated to fetching and downloading sprite assets from [Pokémon Showdown](https://pokemonshowdown.com/).

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
- [Configuration](#configuration)
- [Output Structure](#output-structure)
- [License](#license)
- [Legal Notice](#legal-notice)



# Features
- Multi-threaded downloads for optimal performance
- Automatic directory creation and organization
- Configurable sprite categories
- CPU-aware thread management



# Requirements
- Node.js (v14 or higher recommended)
- NPM or Yarn package manager



# Installation
1. Navigate to the Tools/Sprite_Downloader directory
2. Install dependencies:

```bash
npm install
```



# Usage
Run the tool with:

```bash
npm start
```

or

```typescript
npx ts-node Sprite_Downloader.ts
```



# Configuration
The tool downloads sprites from configurable source directories. Edit the `sourceDirectories` array in Sprite_Downloader.ts to customize which sprite collections are downloaded.

```typescript
const sourceDirectories = [
    {
        url: 'https://play.pokemonshowdown.com/sprites/gen5/',
        outputDir: './Sprites/Normal',
    },
    {
        url: 'https://play.pokemonshowdown.com/sprites/types/',
        outputDir: './Sprites/Types',
    },
    // Add more directories as needed
];
```



# Output Structure
Downloaded sprites are organized into the following directory structure:

```
./Sprites/
  ├── Normal/       - Regular Pokémon sprites
  ├── Normal-Backs/ - Back-facing regular sprites
  ├── Shiny/        - Shiny Pokémon sprites
  ├── Shiny-Backs/  - Back-facing shiny sprites
  ├── Items/        - Item icons
  ├── Categories/   - Move category icons
  └── Types/        - Type icons
```


# License
This tool is part of the Shinkiro project and is subject to the license terms specified in the main [LICENSE](../../LICENSE.md)



# Legal Notice
I do **not** claim to own any assets that are downloaded via use of this tool.

All Pokémon sprites and assets are owned by The Pokémon Company International and Nintendo.

Please respect copyright laws when using downloaded assets.
