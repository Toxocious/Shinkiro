#/bin/bash

# Generate necessary project files using Premake5
case "$(uname -s)" in
    Linux*)
        echo "[Linux]"
        echo "Compiling for Linux"
        # Add Linux-specific commands or actions here
        premake5 --cc=clang --file=Build.lua gmake2
        ;;

    CYGWIN*|MINGW*|MSYS*)
        echo "[Windows]"
        echo "Generating Visual Studio solution(s)"
        premake5 --file=Build.lua vs2022
        ;;

    Darwin*)
        echo "[MacOS]"
        echo "Compiling for MacOS is not currently supported"
        exit
        ;;

    *)
        echo "[Unknown]"
        echo "Unknown operating system; quitting"
        exit
        ;;
esac
