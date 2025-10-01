# Safe Airway - Build Instructions

This document provides instructions for building the Safe Airway application with all UI optimizations.

## 🏗️ Build Scripts Available

Three build scripts are provided for different platforms and preferences:

### Linux/WSL: `build-linux.sh`
- **Bash script** for Linux, macOS, and WSL environments
- Automatically detects dependencies and build tools
- Supports parallel builds for faster compilation

### Windows: `build-windows.bat`
- **Batch file** for traditional Windows Command Prompt
- Compatible with Visual Studio and MinGW compilers
- Automatic compiler detection

### Windows: `build-windows.ps1`
- **PowerShell script** for modern Windows environments
- Enhanced error handling and colored output
- More flexible parameter handling

## 🚀 Quick Start

### Linux/WSL/macOS
```bash
# Simple debug build
./build-linux.sh

# Release build
./build-linux.sh Release
```

### Windows (Command Prompt)
```cmd
# Simple debug build
build-windows.bat

# Release build
build-windows.bat Release
```

### Windows (PowerShell)
```powershell
# Simple debug build
.\build-windows.ps1

# Release build
.\build-windows.ps1 Release

# Clean debug build
.\build-windows.ps1 Debug -Clean

# Show help
.\build-windows.ps1 -Help
```

## 📋 Prerequisites

### All Platforms
- **CMake 3.16 or later**
- **Qt6 development libraries** (Core, Widgets, PrintSupport)
- **C++17 compatible compiler**

### Linux/WSL
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake qt6-base-dev qt6-tools-dev

# Fedora/RHEL
sudo dnf install gcc-c++ cmake qt6-qtbase-devel qt6-qttools-devel

# Arch Linux
sudo pacman -S base-devel cmake qt6-base qt6-tools
```

### Windows
- **Visual Studio 2019/2022** with C++ support, OR
- **MinGW-w64** compiler
- **Qt6** installed (via Qt installer or vcpkg)

## 🎯 Build Outputs

After successful build, you'll find:
- **Linux**: `build-debug/safe-airway` or `build-release/safe-airway`
- **Windows**: `build-debug/safe-airway.exe` or `build-release/safe-airway.exe`

## ✨ UI Optimizations Included

All builds include these UI optimizations for better small-screen usability:

### 📱 Space Optimization
- **Emergency panel → popup overlay** (~40% vertical space savings)
- **Date of birth field removed** (freed up input area)
- **Optimized layout** for Surface Pro 8 and similar devices

### 🖱️ Touch-Friendly Interface
- **Table → modern widget** with larger touch targets
- **Better spacing** between interactive elements
- **Improved button sizes** for touch interaction

### 🧮 Enhanced Functionality
- **Complete Tracoe manufacturer support** with verified specifications
- **Extended Shiley product lines** (6.0-6.5mm sizes with TaperGuard variants)
- **Automatic calculations** for outer diameter and suction catheter sizing
- **Real-time validation** and error checking

### 🏥 Clinical Features
- **100% manufacturer coverage** (Bivona, Shiley, Tracoe)
- **Verified size algorithms** from clinical documentation
- **Professional medical interface** suitable for hospital environments

## 🐛 Troubleshooting

### Common Issues

**"CMake not found"**
- Install CMake and ensure it's in your system PATH
- Linux: `sudo apt install cmake`
- Windows: Download from https://cmake.org/download/

**"Qt6 not found"**
- Install Qt6 development packages
- Linux: `sudo apt install qt6-base-dev qt6-tools-dev`
- Windows: Use Qt installer or package manager

**"Compiler not found"**
- Linux: `sudo apt install build-essential`
- Windows: Install Visual Studio with C++ support

**Build fails with linking errors**
- Try a clean build: delete build directory and rebuild
- Ensure all dependencies are properly installed

### Build Script Options

**Linux script parameters:**
```bash
./build-linux.sh [Debug|Release|RelWithDebInfo|MinSizeRel]
```

**Windows PowerShell parameters:**
```powershell
.\build-windows.ps1 [-BuildType <type>] [-Clean] [-Help]
```

## 📊 Performance Tips

- **Parallel builds**: Scripts automatically use all CPU cores
- **Build types**:
  - `Debug`: Full debug info, no optimization (development)
  - `Release`: Optimized for performance (production)
  - `RelWithDebInfo`: Optimized with debug info (testing)
  - `MinSizeRel`: Optimized for small size

## 🔧 Manual Build (Advanced)

If you prefer manual control:

```bash
# Configure
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build-debug --parallel

# Run
./build-debug/safe-airway
```

## 📝 Notes

- Build scripts create separate directories for each build type
- Previous builds are automatically cleaned when using scripts
- All dependencies are checked before building
- Detailed error messages help troubleshoot issues

---

**Need help?** Check the console output for detailed error messages, or refer to the Safe Airway documentation.