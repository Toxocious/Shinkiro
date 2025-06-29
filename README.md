<div align="center">
  <!-- <img src="..." title="Shinkiro Logo" alt="Shinkiro Logo" /> -->
  <h1 align="center">Shinkiro</h1>

  **Shinkiro** is a C++ project that currently compiles into a single executable alongside multiple shared libraries.

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
- [About The Project](#about-the-project)
  - [Shinkiro's Origins](#shinkiros-origins)
  - [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Project Setup](#project-setup)
  - [Cleaning Up](#cleaning-up)
- [Project Modules](#project-modules)
  - [Executables](#executables)
    - [Asset Packer](#asset-packer)
    - [Asset Unpacker](#asset-unpacker)
    - [Editor](#editor)
  - [Libraries](#libraries)
    - [Lib-Asset](#lib-asset)
    - [Lib-Audio](#lib-audio)
    - [Lib-Core](#lib-core)
    - [Lib-Input](#lib-input)
    - [Lib-Renderer](#lib-renderer)
  - [Tools](#tools)
    - [Asset Downloader](#asset-downloader)
- [License](#license)



# About The Project
## Shinkiro's Origins
Shinkiro was initially created to serve as a template for C++ applications where you would have one (or more) executables that would link to and allow the usage of one or more shared libraries.

Initially I wanted to set up the architecture for a game engine, where I could have an Editor (executable) that uses multiple libraries in order to organize functionality across any executables that would be created in the future, as well as to keep code organized into "modules".

## Features
Shinkiro currently features.. almost nothing.

The build system is set-up to use premake5, and currently generates the necessary Visual Studio files and configurations to build an [Editor](./Projects/Editor/) executable, an asset packer executable, an asset unpacker executable, and a variety of small core shared libraries.

All shared libraries are prefixed with **`Lib-*`**, such as "Lib-Audio" where all code for audio-related functionality would be.

All executable applications are prefixed with **`*App-*`**.

Any external tools that are not directly used by any part of an application or its modules should remain inside of the `./Tools` directory.



# Getting Started
## Prerequisites
You'll need [Premake](https://premake.github.io/download) and [Visual Studio](https://visualstudio.microsoft.com/vs/community/).

> Support for non-Windows operating systems does not currently exist.

## Installation
Clone the repository.

```bash
git clone --recursive https://github.com/Toxocious/Shinkiro.git
```

## Project Setup
Generate the Visual Studio files with premake; there's a script included with the repository to handle this for you.

```sh
./Compile.sh
```

## Cleaning Up
Included with this repository is a script to remove all generated files along with all build files; you can run the script like so.

```sh
./Clean.sh
```


# Project Modules

## Executables
### Asset Packer
Shinkiro features a basic but usable asset packer, serving the purpose of bundling assets for any Shinkiro application to use during runtime.
> Read more about the asset packer [here](./Projects/App-AssetPacker/README.md)

### Asset Unpacker
Shinkiro features an asset unpacker, allowing previously packed asset bundles to be unpacked back into its respective collective of files.
> Read more about the asset unpacker [here](./Projects/App-AssetUnpacker/README.md)

### Editor
Shinkiro's editor is not currently fit for use as it is not functional.
> Read more about the editor [here](./Projects/App-Editor/README.md)

## Libraries
### Lib-Asset
Shinkiro includes a shared library that specifically handles assets - packing and unpacking, as well as loading an asset into memory.
> Read more about the asset library [here](./Projects/Lib-Asset/README.md)

### Lib-Audio
Shinkiro's Lib-Audio library is not currently functional.
> Read more about the audio library [here](./Projects/Lib-Audio/README.md)

### Lib-Core
Shinkiro's Lib-Core library contains more generalized features for any Shinkiro application.
> Read more about the core library [here](./Projects/Lib-Core/README.md)

### Lib-Input
Shinkiro's Lib-Input library is not currently functional.
> Read more about the input library [here](./Projects/Lib-Input/README.md)

### Lib-Renderer
Shinkiro's Lib-Renderer library is not currently functional.
> Read more about the renderer library [here](./Projects/Lib-Renderer/README.md)

## Tools
### Asset Downloader
Shinkiro includes a custom tool that fetches and downloads assets from Pokemon Showdown, namely Pokemon sprites and icons, item icons, and Pokemon cries.
> Read more about the asset downloader [here](./Tools/Sprite_Downloader/README.md)



# License
This project is licensed under MIT.

For more information about the license, check out the [LICENSE](LICENSE.md).
