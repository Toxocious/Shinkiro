cd /home/jess/Dev/shinkiro/Libraries/imgui

# Create output directories
mkdir -p lib lib lib

# Source files
SOURCES="include/imgui.cpp include/imgui_draw.cpp include/imgui_widgets.cpp include/imgui_tables.cpp include/imgui_demo.cpp"

# Common flags
COMMON_FLAGS="-fPIC -shared -Iinclude -std=c++17"

echo "Building ImGui libraries..."

# Debug build
echo "  Building debug..."
if [ -f lib/libimgui_debug.so ]; then
    rm -f lib/libimgui_debug.so*
fi
g++ $COMMON_FLAGS -g -O0 -DDEBUG -o lib/libimgui_debug.so $SOURCES

# Release build
echo "  Building release..."
if [ -f lib/libimgui_release.so ]; then
    rm -f lib/libimgui_release.so*
fi
g++ $COMMON_FLAGS -O2 -DNDEBUG -o lib/libimgui_release.so $SOURCES

# Dist build
echo "  Building dist..."
if [ -f lib/libimgui_dist.so ]; then
    rm -f lib/libimgui_dist.so*
fi
g++ $COMMON_FLAGS -O3 -DNDEBUG -s -o lib/libimgui_dist.so $SOURCES

echo "ImGui libraries created:"
echo "  Debug: lib/libimgui_debug.so"
echo "  Release: lib/libimgui_release.so"
echo "  Dist: lib/libimgui_dist.so"