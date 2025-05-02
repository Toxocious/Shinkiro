<div align="center">
  <!-- <img src="./app/images/Assets/banner.png" title="Pokemon Absolute Logo" alt="Pokemon Absolute Logo" /> -->
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
- [License](#license)



# About The Project
## Shinkiro's Origins
Shinkiro was initially created as a template for C++ applications where you would have one (or more) executables that would link to and allow the usage of one or more shared libraries.

Initially I wanted to set up the architecture for a game engine, where I could have an Editor (executable) that uses multiple libraries in order to organize functionality across any executables that would be created in the future, as well as to keep code organized into "modules".

## Features
Shinkiro currently features.. almost nothing.

The build system is set-up to use premake5, and currently generates the necessary Visual Studio files and configurations to build an [Editor](./Projects/Editor/) executable as well as a few shared libraries.

All shared libraries are prefixed with "**Core-**", such as "Core-Audio" where all code for audio-related functionality would be.



# Getting Started
## Prerequisites
You'll need [Premake](https://premake.github.io/download) and [Visual Studio](https://visualstudio.microsoft.com/vs/community/).

## Installation
Clone the repository.

```bash
git clone --recursive https://github.com/Toxocious/Shinkiro.git
```

## Project Setup
Generate the Visual Studio files with premake; there's a script included with the repository to handle this for you.

**Windows**
```sh
./Compile.sh
```

## Cleaning Up
Included with this repository is a script to remove all generated files along with all build files; you can run the script like so.

```sh
./Clean.sh
```



# License
This project is licensed under MIT.

For more information about the license, check out the [LICENSE](LICENSE.md).
