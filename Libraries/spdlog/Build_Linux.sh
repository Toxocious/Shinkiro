cd /home/jess/Dev/shinkiro/Libraries/spdlog

# Source files
SOURCES="src/spdlog.cpp src/stdout_sinks.cpp src/color_sinks.cpp src/file_sinks.cpp"

# Common flags
COMMON_FLAGS="-fPIC -shared -std=c++11 -Iinclude -DSPDLOG_COMPILED_LIB"

# Debug build
echo "  Building debug..."
if [ -f lib/libspdlog_debug.so ]; then
    rm -f lib/libspdlog_debug.so*
fi
g++ $COMMON_FLAGS -g -O0 -DDEBUG -o lib/libspdlog_debug.so $SOURCES

# Release build
echo "  Building release..."
if [ -f lib/libspdlog_release.so ]; then
    rm -f lib/libspdlog_release.so*
fi
g++ $COMMON_FLAGS -O2 -DNDEBUG -o lib/libspdlog_release.so $SOURCES

# Dist build
echo "  Building dist..."
if [ -f lib/libspdlog_dist.so ]; then
    rm -f lib/libspdlog_dist.so*
fi
g++ $COMMON_FLAGS -O3 -DNDEBUG -s -o lib/libspdlog_dist.so $SOURCES

echo "spdlog libraries created:"
echo "  Debug: lib/libspdlog_debug.so"
echo "  Release: lib/libspdlog_release.so"
echo "  Dist: lib/libspdlog_dist.so"
