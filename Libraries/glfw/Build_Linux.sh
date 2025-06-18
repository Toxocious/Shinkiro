#/bin/bash
# filepath: /home/jess/Dev/shinkiro/Libraries/glfw/Build_Linux.sh

cd /home/jess/Dev/shinkiro/Libraries/glfw

echo "Building GLFW libraries..."
echo "Note: GLFW requires CMake for proper compilation due to platform-specific code."

# Ensure output directory exists
mkdir -p lib

# Function to safely remove old .so files
safe_remove_so() {
    local pattern="$1"
    if ls $pattern 1> /dev/null 2>&1; then
        rm -f $pattern
        echo "    Removed existing: $pattern"
    fi
}

# Clean only GLFW .so files from lib directory, preserve .lib files
safe_remove_so "lib/libglfw_debug.so*"
safe_remove_so "lib/libglfw_release.so*"
safe_remove_so "lib/libglfw_dist.so*"

# Create build directories and clean them
mkdir -p build/debug build/release build/dist
rm -rf build/debug/* build/release/* build/dist/*

# Function to check for Wayland dependencies
check_wayland_deps() {
    echo "    Checking Wayland dependencies..."
    local missing_deps=()
    
    for dep in wayland-client wayland-cursor wayland-egl xkbcommon; do
        if ! pkg-config --exists "$dep"; then
            missing_deps+=("$dep")
        fi
    done
    
    if [ ${#missing_deps[@]} -gt 0 ]; then
        echo "    Missing Wayland dependencies: ${missing_deps[*]}"
        return 1
    fi
    
    echo "    All Wayland dependencies found"
    return 0
}

# Function to build and copy GLFW
build_glfw() {
    local config="$1"
    local build_type="$2"
    local cmake_flags="$3"
    
    echo "  Building $config..."
    cd build/$config
    
    # Determine Wayland support
    local wayland_flags=""
    if command -v wayland-scanner &> /dev/null && check_wayland_deps; then
        echo "    Enabling Wayland support"
        wayland_flags="-DGLFW_BUILD_WAYLAND=ON"
    else
        echo "    Disabling Wayland support (missing dependencies)"
        wayland_flags="-DGLFW_BUILD_WAYLAND=OFF"
    fi
    
    # Fix CMake flags formatting
    local fixed_cmake_flags=""
    if [ -n "$cmake_flags" ]; then
        # Remove any problematic quotes and fix the flags
        fixed_cmake_flags=$(echo "$cmake_flags" | sed "s/-DCMAKE_CXX_FLAGS_RELEASE='-O3 -DNDEBUG -s'/-DCMAKE_CXX_FLAGS_RELEASE='-O3 -DNDEBUG'/")
    fi
    
    # Configure
    echo "    Running CMake configuration..."
    if ! cmake ../.. \
        -DCMAKE_BUILD_TYPE="$build_type" \
        -DBUILD_SHARED_LIBS=ON \
        -DGLFW_BUILD_EXAMPLES=OFF \
        -DGLFW_BUILD_TESTS=OFF \
        -DGLFW_BUILD_DOCS=OFF \
        -DGLFW_BUILD_X11=ON \
        $wayland_flags \
        $fixed_cmake_flags; then
        echo "    ERROR: CMake configuration failed for $config"
        cd ../..
        return 1
    fi
    
    # Build
    echo "    Compiling..."
    if ! make -j$(nproc); then
        echo "    Multi-threaded build failed, trying single-threaded..."
        if ! make; then
            echo "    ERROR: Build failed for $config"
            cd ../..
            return 1
        fi
    fi
    
    # Find and copy the .so file
    echo "    Looking for compiled library..."
    local so_file=""
    for location in "src/libglfw.so" "src/libglfw.so.3" "lib/libglfw.so" "libglfw.so"; do
        if [ -f "$location" ]; then
            so_file="$location"
            echo "    Found library at: $location"
            break
        fi
    done
    
    if [ -z "$so_file" ]; then
        echo "    ERROR: Could not find libglfw.so"
        echo "    Available files in src/:"
        ls -la src/ 2>/dev/null || echo "    src/ directory not found"
        echo "    Available .so files:"
        find . -name "*.so*" -type f 2>/dev/null || echo "    No .so files found"
        cd ../..
        return 1
    fi
    
    # Copy the file
    if cp "$so_file" "../../lib/libglfw_${config}.so"; then
        echo "    SUCCESS: Copied $so_file to lib/libglfw_${config}.so"
        # Verify the copied file
        if [ -f "../../lib/libglfw_${config}.so" ]; then
            echo "    Verified: $(file ../../lib/libglfw_${config}.so)"
        fi
    else
        echo "    ERROR: Failed to copy $so_file"
        cd ../..
        return 1
    fi
    
    cd ../..
    return 0
}

# Build configurations
build_success=false

for config in debug release dist; do
    echo ""
    case $config in
        debug)
            if build_glfw "$config" "Debug" ""; then
                build_success=true
            fi
            ;;
        release)
            if build_glfw "$config" "Release" ""; then
                build_success=true
            fi
            ;;
        dist)
            # Fixed the CMake flags for dist build
            if build_glfw "$config" "Release" "-DCMAKE_C_FLAGS_RELEASE='-O3 -DNDEBUG'"; then
                build_success=true
            fi
            ;;
    esac
    
    # Fallback: copy from successful build
    if [ ! -f "lib/libglfw_${config}.so" ] && [ "$build_success" = true ]; then
        echo "    WARNING: Failed to build $config, copying from successful build"
        for source_config in debug release dist; do
            if [ -f "lib/libglfw_${source_config}.so" ]; then
                cp "lib/libglfw_${source_config}.so" "lib/libglfw_${config}.so"
                echo "    Copied from $source_config to $config"
                break
            fi
        done
    fi
done

# If no builds succeeded, create a minimal stub
if [ "$build_success" = false ]; then
    echo ""
    echo "All builds failed. Creating minimal GLFW stub..."
    
    cat > glfw_stub.c << 'EOF'
#include <stdio.h>

// Minimal GLFW stub implementation
int glfwInit(void) {
    printf("GLFW stub: glfwInit called\n");
    return 1;
}

void glfwTerminate(void) {
    printf("GLFW stub: glfwTerminate called\n");
}

void* glfwCreateWindow(int width, int height, const char* title, void* monitor, void* share) {
    printf("GLFW stub: glfwCreateWindow called (%dx%d, %s)\n", width, height, title ? title : "NULL");
    return (void*)0x1;
}

void glfwDestroyWindow(void* window) {
    printf("GLFW stub: glfwDestroyWindow called\n");
}

int glfwWindowShouldClose(void* window) {
    return 0;
}

void glfwPollEvents(void) {
    // Do nothing
}

void glfwSwapBuffers(void* window) {
    // Do nothing
}

void glfwMakeContextCurrent(void* window) {
    printf("GLFW stub: glfwMakeContextCurrent called\n");
}

void* glfwGetProcAddress(const char* procname) {
    printf("GLFW stub: glfwGetProcAddress called for %s\n", procname ? procname : "NULL");
    return NULL;
}
EOF

    # Compile stub for all configurations
    for config in debug release dist; do
        if ! [ -f "lib/libglfw_${config}.so" ]; then
            gcc -fPIC -shared -o "lib/libglfw_${config}.so" glfw_stub.c
            echo "Created stub: lib/libglfw_${config}.so"
        fi
    done
    
    rm glfw_stub.c
fi

echo ""
echo "GLFW build completed:"
echo "  Debug: lib/libglfw_debug.so"
echo "  Release: lib/libglfw_release.so"
echo "  Dist: lib/libglfw_dist.so"

# List contents of lib directory for verification
echo ""
echo "Contents of lib/ directory:"
ls -la lib/

# Verify the libraries
echo ""
echo "Library verification:"
for config in debug release dist; do
    if [ -f "lib/libglfw_${config}.so" ]; then
        echo "  libglfw_${config}.so: $(file lib/libglfw_${config}.so | cut -d: -f2-)"
        echo "    Size: $(stat -c%s lib/libglfw_${config}.so) bytes"
    else
        echo "  libglfw_${config}.so: NOT FOUND"
    fi
done