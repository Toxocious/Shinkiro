<div align="center">
  <!-- <img src="..." title="Shinkiro Logo" alt="Shinkiro Logo" /> -->
  <h1 align="center">Shinkiro's Audio Converter</h1>

  **Shinkiro's Audio Converter** is a tool dedicated to converting audio files from one format to another.

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
- [Usage](#usage)
- [Configuration](#configuration)
- [Output Structure](#output-structure)
- [License](#license)



# Features
- Multi-threaded conversion calls for optimal performance
- Automatic directory creation and organization
- CPU-aware thread management



# Requirements
- [FFmpeg](https://ffmpeg.org/) installed and in your system's PATH



# Usage
The script converts audio files from one format to another using FFmpeg, supporting multi-threaded conversion.

Basic usage:
```
./Convert.sh <input_extension> <output_extension> [max_concurrent_threads] [--dry-run]
```

Example:
```bash
./Convert.sh ogg wav 8 --dry-run
```

- ``<input_extension>`` — file format to convert from (e.g., ogg)
- ``<output_extension>`` — file format to convert to (e.g., wav)
- ``[max_concurrent_threads]`` — optional number of parallel conversion jobs (defaults to half CPU cores)
- ``[--dry-run]`` — optional flag to simulate conversions without running ffmpeg

The script automatically reads input files from the `./Input` folder relative to the script's location and outputs converted files to the `./Output` folder.



# Configuration
Supported audio formats:
``mp3, wav, ogg, flac, aac, m4a, opus, alac, wma, aiff``

- Thread count is auto-detected but can be overridden.
- Logs are saved in the `./Logs` directory with timestamps.
- Dry run mode outputs commands without performing conversions.



# Output Structure
Downloaded assets are organized into the following directory structure:

```
./Input/  - Audio files to convert
./Output/ - Converted audio files
./Logs/   - Conversion logs
```


# License
This tool is part of the Shinkiro project and is subject to the license terms specified in the main [LICENSE](../../LICENSE.md)
