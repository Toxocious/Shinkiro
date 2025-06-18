#/bin/bash
# filepath: /home/jess/Dev/shinkiro/Compile.sh

set -e  # Exit on any error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Logging functions
log_info() {
    echo -e "${BLUE}[INFO]${NC} $(date '+%Y-%m-%d %H:%M:%S') - $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $(date '+%Y-%m-%d %H:%M:%S') - $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $(date '+%Y-%m-%d %H:%M:%S') - $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $(date '+%Y-%m-%d %H:%M:%S') - $1" >&2
}

# Create logs directory
LOGS_DIR="Logs"
mkdir -p "$LOGS_DIR"

MAIN_LOG="$LOGS_DIR/Compile_$(date +%Y%m%d_%H%M%S).log"

# Redirect all output to main log file as well
exec > >(tee -a "$MAIN_LOG")
exec 2> >(tee -a "$MAIN_LOG" >&2)

# Check for required tools
check_dependencies() {
    log_info "Checking dependencies..."
    
    local missing_deps=()
    
    if ! command -v premake5 &> /dev/null; then
        missing_deps+=("premake5")
    fi
    
    if ! command -v make &> /dev/null; then
        missing_deps+=("make")
    fi
    
    if ! command -v clang &> /dev/null && ! command -v gcc &> /dev/null; then
        missing_deps+=("clang or gcc")
    fi
    
    if [ ${#missing_deps[@]} -ne 0 ]; then
        log_error "Missing required dependencies: ${missing_deps[*]}"
        log_error "Please install the missing dependencies and try again."
        exit 1
    fi
    
    log_success "All dependencies found"
}

# Build a single library with error handling
build_library() {
    local lib_name="$1"
    local lib_path="Libraries/$lib_name"
    local build_script="$lib_path/Build_Linux.sh"
    local lib_log="$lib_path/build_$(date +%Y%m%d_%H%M%S).log"
    
    if [ ! -f "$build_script" ]; then
        log_warning "Build script not found for $lib_name: $build_script"
        return 1
    fi
    
    log_info "Building $lib_name..."
    
    # Make script executable
    chmod +x "$build_script"
    
    # Run the build script with timeout and capture output
    if timeout 300 "$build_script" > "$lib_log" 2>&1; then
        log_success "Built $lib_name successfully"
        
        # Check if .so files were created
        local so_count=$(find "$lib_path/lib" -name "*.so" 2>/dev/null | wc -l)
        if [ "$so_count" -gt 0 ]; then
            log_info "$lib_name: Found $so_count .so files"
        else
            log_warning "$lib_name: No .so files found in lib directory"
        fi
        
        return 0
    else
        local exit_code=$?
        log_error "Failed to build $lib_name (exit code: $exit_code)"
        log_error "Build log: $lib_log"
        
        # Show last few lines of the error log
        if [ -f "$lib_log" ]; then
            log_error "Last 10 lines of build log:"
            tail -10 "$lib_log" | sed 's/^/  /'
        fi
        
        return 1
    fi
}

# Build all libraries
build_libraries() {
    log_info "Starting library builds..."
    
    local libraries=("assimp" "glad" "glfw" "glm" "imgui" "spdlog")
    local failed_builds=()
    local successful_builds=()
    
    for lib in "${libraries[@]}"; do
        if build_library "$lib"; then
            successful_builds+=("$lib")
        else
            failed_builds+=("$lib")
        fi
        echo  # Add spacing between library builds
    done
    
    # Summary
    log_info "Library build summary:"
    log_success "Successfully built: ${successful_builds[*]:-none}"
    
    if [ ${#failed_builds[@]} -gt 0 ]; then
        log_error "Failed to build: ${failed_builds[*]}"
        log_error "Check individual library logs for details"
        return 1
    else
        log_success "All libraries built successfully!"
        return 0
    fi
}

# Clean old build artifacts
clean_builds() {
    if [[ " $* " =~ " --clean " ]]; then
        log_info "Cleaning previous build artifacts..."
        
        # Clean generated files
        rm -rf Generated/ Build/ Makefile
        
        # Clean library builds
        find Libraries/ -name "build" -type d -exec rm -rf {} + 2>/dev/null || true
        find Libraries/ -name "*.log" -delete 2>/dev/null || true
        
        # Clean Visual Studio files
        find . -name "*.sln" -delete 2>/dev/null || true
        find . -name "*.vcxproj" -delete 2>/dev/null || true
        find . -name "*.vcxproj.filters" -delete 2>/dev/null || true
        find . -name "*.vcxproj.user" -delete 2>/dev/null || true
        
        # Clean Makefiles
        find . -name "*.make" -delete 2>/dev/null || true
        find . -name "Makefile" -delete 2>/dev/null || true
        
        log_success "Build artifacts cleaned"
        return 0  # Return 0 to indicate clean was performed
    fi
    return 1  # Return 1 to indicate no clean was performed
}

# Generate project files
generate_makefiles() {
    log_info "Generating Makefiles with Premake5..."
    
    if premake5 --cc=clang --file=Build.lua gmake2 2>&1 | tee -a "$MAIN_LOG"; then
        log_success "Makefiles generated successfully"
        return 0
    else
        log_error "Failed to generate Makefiles"
        return 1
    fi
}

# Build main project
build_project() {
    log_info "Building main project..."
    
    local configs=("debug" "release" "dist")
    
    for config in "${configs[@]}"; do
        log_info "Building $config configuration..."
        
        if make config="$config" -j$(nproc) 2>&1 | tee -a "$MAIN_LOG"; then
            log_success "Built $config configuration successfully"
        else
            log_error "Failed to build $config configuration"
            return 1
        fi
    done
    
    log_success "All configurations built successfully!"
}

# Main execution
main() {
    # Handle command line arguments
    if [ "$1" = "--help" ] || [ "$1" = "-h" ]; then
        echo "Usage: $0 [--clean] [--libs-only] [--project-only]"
        echo "  --clean       Clean previous build artifacts (does not build)"
        echo "  --libs-only   Build only libraries"
        echo "  --project-only Build only main project (skip libraries)"
        echo "  --help, -h    Show this help message"
        exit 0
    fi
    
    # Check if we're only cleaning
    if clean_builds "$@"; then
        log_success "Clean operation completed successfully!"
        log_info "Main build log saved to: $MAIN_LOG"
        exit 0
    fi

    log_info "Starting build process..."
    log_info "Main build log: $MAIN_LOG"
    
    check_dependencies
    
    case "$(uname -s)" in
        Linux*)
            log_info "Detected Linux platform"
            
            if [[ ! " $* " =~ " --project-only " ]]; then
                if ! build_libraries; then
                    log_error "Library builds failed. Stopping."
                    exit 1
                fi
            fi
            
            if [[ ! " $* " =~ " --libs-only " ]]; then
                if ! generate_makefiles; then
                    log_error "Makefile generation failed. Stopping."
                    exit 1
                fi
                
                if ! build_project; then
                    log_error "Project build failed."
                    exit 1
                fi
            fi
            ;;
            
        CYGWIN*|MINGW*|MSYS*)
            log_info "Detected Windows platform"
            log_info "Generating Visual Studio solution..."
            
            if premake5 --file=Build.lua vs2022; then
                log_success "Visual Studio solution generated successfully"
                log_info "Open the generated .sln file in Visual Studio to build"
            else
                log_error "Failed to generate Visual Studio solution"
                exit 1
            fi
            ;;
            
        Darwin*)
            log_warning "macOS platform detected"
            log_error "Compiling for macOS is not currently supported"
            exit 1
            ;;
            
        *)
            log_error "Unknown operating system: $(uname -s)"
            log_error "Supported platforms: Linux, Windows (Cygwin/MinGW/MSYS)"
            exit 1
            ;;
    esac
    
    log_success "Build process completed successfully!"
    log_info "Main build log saved to: $MAIN_LOG"
}

# Trap to handle script interruption
trap 'log_error "Build process interrupted"; exit 130' INT TERM

# Run main function with all arguments
main "$@"