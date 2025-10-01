#!/bin/bash

# Safe Airway - Linux Build Script
# Builds the Safe Airway application with all UI optimizations

set -e  # Exit on any error

# Check for help flag
if [[ "$1" == "--help" || "$1" == "-h" ]]; then
    cat << 'EOF'

>>  Safe Airway Linux Build Script
==================================

USAGE:
    ./build-linux.sh [BuildType] [--help]

PARAMETERS:
    BuildType    Build configuration (Debug, Release, RelWithDebInfo, MinSizeRel)
                Default: Debug
    
    --help, -h   Show this help message

EXAMPLES:
    ./build-linux.sh                      # Debug build
    ./build-linux.sh Release              # Release build  
    ./build-linux.sh RelWithDebInfo       # Release with debug info

REQUIREMENTS:
    - CMake (3.16 or later)
    - Qt6 development libraries
    - GCC/Clang compiler
    - make or ninja build system

EOF
    exit 0
fi

echo "🏗️  Safe Airway Linux Build Script"
echo "=================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    print_error "CMakeLists.txt not found. Please run this script from the safe-airway project root."
    exit 1
fi

# Check for required dependencies
print_status "Checking dependencies..."

if ! command -v cmake &> /dev/null; then
    print_error "CMake is required but not installed. Please install CMake first."
    exit 1
fi

if ! command -v make &> /dev/null; then
    print_error "Make is required but not installed. Please install build-essential."
    exit 1
fi

# Check for Qt6
if ! pkg-config --exists Qt6Core Qt6Widgets; then
    print_warning "Qt6 development packages may not be installed."
    print_warning "Install with: sudo apt install qt6-base-dev qt6-tools-dev"
fi

# Build type (default to Debug, can be overridden)
BUILD_TYPE=${1:-Debug}
BUILD_DIR="build-${BUILD_TYPE,,}"  # Convert to lowercase

print_status "Build type: $BUILD_TYPE"
print_status "Build directory: $BUILD_DIR"

# Clean previous build if exists
if [ -d "$BUILD_DIR" ]; then
    print_status "Cleaning previous build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory
print_status "Creating build directory..."
mkdir -p "$BUILD_DIR"

# Configure with CMake
print_status "Configuring project with CMake..."
if cmake -S . -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON; then
    print_success "CMake configuration completed"
else
    print_error "CMake configuration failed"
    exit 1
fi

# Build the project
print_status "Building Safe Airway..."
if cmake --build "$BUILD_DIR" --parallel $(nproc); then
    print_success "Build completed successfully!"
else
    print_error "Build failed"
    exit 1
fi

# Check if executable was created
EXECUTABLE="$BUILD_DIR/safe-airway"
if [ -f "$EXECUTABLE" ]; then
    print_success "Executable created: $EXECUTABLE"
    
    # Make it executable
    chmod +x "$EXECUTABLE"
    
    echo ""
    echo ">> Build Complete!"
    echo "=================="
    echo "Executable: $EXECUTABLE"
    echo "Run with: ./$EXECUTABLE"
    echo ""
else
    print_error "Executable not found after build"
    exit 1
fi