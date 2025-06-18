cd /home/jess/Dev/shinkiro/Libraries/glm

# Create output directories
mkdir -p lib lib lib

# GLM is header-only, but we can create a dummy .so with version info
echo "Building GLM libraries (header-only library with version info)..."

# Create a simple source file for GLM
cat > glm_version.cpp << 'EOF'
extern "C" {
    const char* glm_get_version() {
        return "GLM_VERSION_0_9_9_8";
    }
}
EOF

# Common flags
COMMON_FLAGS="-fPIC -shared -Iinclude -std=c++17"

# Debug build
echo "  Building debug..."
if [ -f lib/libglm_debug.so ]; then
    rm -f lib/libglm_debug.so*
fi
g++ $COMMON_FLAGS -g -O0 -DDEBUG -o lib/libglm_debug.so glm_version.cpp

# Release build
echo "  Building release..."
if [ -f lib/libglm_release.so ]; then
    rm -f lib/libglm_release.so*
fi
g++ $COMMON_FLAGS -O2 -DNDEBUG -o lib/libglm_release.so glm_version.cpp

# Dist build
echo "  Building dist..."
if [ -f lib/libglm_dist.so ]; then
    rm -f lib/libglm_dist.so*
fi
g++ $COMMON_FLAGS -O3 -DNDEBUG -s -o lib/libglm_dist.so glm_version.cpp

# Clean up
rm glm_version.cpp

echo "GLM libraries created:"
echo "  Debug: lib/libglm_debug.so"
echo "  Release: lib/libglm_release.so"
echo "  Dist: lib/libglm_dist.so"
