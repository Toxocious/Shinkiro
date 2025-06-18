#/bin/bash
# filepath: /home/jess/Dev/shinkiro/Libraries/glad/Build_Linux.sh

cd /home/jess/Dev/shinkiro/Libraries/glad

# Create output directory (don't remove existing content)
mkdir -p lib

# Source files
SOURCES="src/glad.c"

# Common flags
COMMON_FLAGS="-fPIC -shared -Iinclude"

echo "Building GLAD libraries..."

# Debug build - only remove existing .so files with this naming pattern (if they exist)
echo "  Building debug..."
if [ -f lib/libglad_debug.so ]; then
    rm -f lib/libglad_debug.so*
fi
gcc $COMMON_FLAGS -g -O0 -DDEBUG -o lib/libglad_debug.so $SOURCES

# Release build
echo "  Building release..."
if [ -f lib/libglad_release.so ]; then
    rm -f lib/libglad_release.so*
fi
gcc $COMMON_FLAGS -O2 -DNDEBUG -o lib/libglad_release.so $SOURCES

# Dist build
echo "  Building dist..."
if [ -f lib/libglad_dist.so ]; then
    rm -f lib/libglad_dist.so*
fi
gcc $COMMON_FLAGS -O3 -DNDEBUG -s -o lib/libglad_dist.so $SOURCES

echo "GLAD libraries created:"
echo "  Debug: lib/libglad_debug.so"
echo "  Release: lib/libglad_release.so"
echo "  Dist: lib/libglad_dist.so"