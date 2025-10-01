# Safe Airway Application

A digital form application for airway management at Nemours Children's Health, built with Qt 6 and C++17.

## Quick Start

### Prerequisites
- Qt 6.7.3 with MinGW 11.2 64-bit (or Qt 6.5.9 LTS)
- C++17 compatible compiler (GCC 9.0+ or MSVC 2017+ recommended)
- CMake or qmake (included with Qt)

**Important**: If you have an older compiler (GCC 8.x), you may encounter C++17 compatibility issues. Please upgrade to Qt 6.7.3 with MinGW 11.2 or use MSVC build tools.

### Creating a Windows Distributable (Portable .exe)

To create a standalone Windows executable that can run without Qt installed:

**Using the automated build script (Easiest):**
```batch
# Open Windows Command Prompt (not WSL)
cd C:\_Projects\safe-airway
build-windows-release.bat
```

The script will:
- Build a release version
- Bundle all Qt dependencies automatically
- Create a `safe-airway-dist` folder with everything needed

**Result:** A portable ~25-40 MB folder that runs on any Windows 10/11 machine.

**For detailed instructions**, see `CREATE_WINDOWS_EXECUTABLE.md`

### Building

#### Linux (WSL/Ubuntu/Debian)
```bash
# Install Qt 6 and build tools
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev cmake build-essential

# Create build directory and configure
mkdir build
cd build
cmake ..

# Build the application
make -j4

# Run the application
./safe-airway
```

#### Windows (Command Prompt)
```batch
REM Prerequisites: Install Qt 6.7.3 with MinGW 11.2 64-bit from Qt Installer

REM Set up Qt environment (adjust path to your Qt installation)
set QTDIR=C:\Qt\6.7.3\mingw_64
set PATH=%QTDIR%\bin;%PATH%

REM Navigate to project directory
cd C:\_Projects\safe-airway

REM Build with qmake
qmake safe-airway.pro
mingw32-make -j4

REM Run the application
release\safe-airway.exe
```

#### Windows (CMake Alternative)
```batch
REM Set up Qt environment (adjust path to your Qt installation)
set QTDIR=C:\Qt\6.7.3\mingw_64
set PATH=%QTDIR%\bin;%PATH%

REM Clean any existing build directory to avoid path conflicts
rmdir /s /q build-win 2>nul

REM Create fresh build directory and configure
mkdir build-win
cd build-win
cmake .. -G "MinGW Makefiles"

REM Build the application
mingw32-make -j4

REM Run the application
safe-airway.exe
```

#### Using Qt Creator (Cross-platform)
1. Open `safe-airway.pro` in Qt Creator
2. Select the kit: Desktop Qt 6.7.3 MinGW 11.2 64-bit
3. Configure Project → Build → Run

## Windows Executable Distribution

For complete instructions on creating a portable Windows .exe, see **`CREATE_WINDOWS_EXECUTABLE.md`**

**Quick Summary:**
1. Run `build-windows-release.bat` from Windows Command Prompt
2. Distributable package created in `safe-airway-dist/` (~25-40 MB)
3. Test on machines without Qt installed
4. Distribute as ZIP or create installer with Inno Setup

### Features
- **Four Color-Coded Forms**: Tracheostomy (Teal), New Tracheostomy (Deep Purple), Difficult Airway (Amber), LTR (Blue)
- **Local JSON Storage**: Cases saved to `~/Documents/SafeAirway/case_saves/`
- **Emergency Scenarios**: Interactive emergency guidance with automatic ETT size synchronization (1300x800px popup)
- **Freeze Mode**: Lock interface during procedures while keeping emergency scenarios accessible
- **Responsive UI**: Percentage-based layout that adapts to different screen sizes
- **Enhanced Visibility**: 32px base font size for visibility from 5-10 feet (configurable in StyleManager)
- **HIPAA Compliant**: Patient last name and MRN hidden from display (preserved in backend)
- **Smart Notifications**: Bottom-centered popup notifications that allow button interaction
- **Case Management**: Save, load, and manage cases with recent files list
- **Manufacturer Persistence**: Tube manufacturer correctly saved and restored (uses data value, not display text)

### Recent Improvements (v1.1.0-alpha)
- ✅ **Centralized Font Configuration**: All font sizes configurable in one location (`src/utils/StyleManager.cpp`)
- ✅ **Enhanced Emergency Panel**: Larger popup (1300x800px) with wider buttons to prevent text cutoff
- ✅ **Improved Notifications**: Bottom-centered notifications that allow button clicks while visible
- ✅ **Fixed Save Logic**: Resolved manufacturer field reset issue and unwanted save prompts
- ✅ **Smart Change Detection**: Loading flag prevents false "unsaved changes" during data loading
- ✅ **Font Size Standardization**: All UI elements now use 32px base font for consistent visibility
- ✅ **Emergency Instructions**: Increased to 32px for better readability from distance

### Previous Improvements (v1.0.0-alpha)
- ✅ **Responsive Layout**: Converted to percentage-based sizing for cross-screen compatibility
- ✅ **Enhanced Visibility**: Increased font sizes (32pt base, up to 40pt for critical elements)
- ✅ **HIPAA Compliance**: Hidden sensitive patient information from display
- ✅ **Space Optimization**: Compact header with logo beside title
- ✅ **Modern Notifications**: Popup notification system replaces permanent status bar
- ✅ **Decision Box Enhancement**: Enlarged primary rescue mode options
- ✅ **Epic Integration Ready**: Backend data structure preserved for future C++ integration

### Application Structure
- `src/core/` - Application services (CaseManager, Application)
- `src/models/` - Data models (Case, EmergencyScenario)
- `src/utils/` - Utilities (ConfigManager, StyleManager)
- `src/views/` - Main windows and form views
- `src/widgets/` - Reusable UI components

### Data Storage
Cases are automatically saved to:
```
~/Documents/SafeAirway/case_saves/
├── tracheostomy/
├── new_tracheostomy/
├── difficult_airway/
└── ltr/
```

Each case is saved as a JSON file with timestamp and case ID.

## Troubleshooting

### Common Build Issues

#### Linux/WSL
- **Qt 6 not found**: Install Qt 6 development packages
  ```bash
  sudo apt install qt6-base-dev qt6-tools-dev cmake build-essential
  ```
- **CMake version too old**: Update CMake to 3.16 or newer
  ```bash
  sudo apt install cmake
  ```

#### Windows
- **qmake not found**: Ensure Qt is installed and `QTDIR` is set correctly
- **mingw32-make not found**: Add `%QTDIR%\bin` to your PATH
- **C++17 filesystem errors (GCC 8.x)**: Upgrade to Qt 6.7.3 with MinGW 11.2 or newer:
  ```batch
  REM Download and install Qt 6.7.3 with MinGW 11.2 64-bit
  REM Then set the correct path:
  set QTDIR=C:\Qt\6.7.3\mingw_64
  set PATH=%QTDIR%\bin;%PATH%
  ```
- **CMake cache path error**: Clean build directory when switching between WSL and Windows:
  ```batch
  rmdir /s /q build-win
  mkdir build-win
  cd build-win
  cmake .. -G "MinGW Makefiles"
  ```
- **Build fails**: Try cleaning first:
  ```batch
  mingw32-make clean
  mingw32-make -j4
  ```

### Runtime Issues
- **Application won't start**: Check that all Qt DLLs are in PATH or same directory as executable
- **Display issues in WSL**: Install X11 server or use Windows build instead
- **Font rendering issues**: Install additional font packages for your system

### Performance Tips
- Use `-j4` or `-j8` for parallel builds (adjust based on CPU cores)
- For development, use Qt Creator for faster incremental builds
- Clean build directory when switching between debug/release builds

## Documentation
See `PROJECT_DOCUMENTATION.md` for comprehensive technical documentation.
See `workflow-diagram.md` for application workflow details.

## Customizing Font Sizes

All font sizes can be easily adjusted in one central location:

**File:** `src/utils/StyleManager.cpp` (lines 7-42)

```cpp
// Edit these values to change font sizes globally
static const int BASE_FONT_SIZE = 32;                    // Base font for most elements
static const int HEADER_FONT_SIZE = 36;                  // Main page headers
static const int EMERGENCY_INSTRUCTIONS_FONT_SIZE = 32;  // Emergency instructions
static const int FORM_INPUT_FONT_SIZE = 32;              // Form input fields
static const int BUTTON_FONT_SIZE = 32;                  // Button text
static const int NOTIFICATION_FONT_SIZE = 32;            // Notification messages
// ... and more
```

After changing values, rebuild the application with `./build-linux.sh` (Linux) or rebuild in Windows.

## Version
Current version: 1.1.0-alpha

## License
© 2024 Nemours Children's Health