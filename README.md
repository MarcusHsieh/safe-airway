# Safe Airway Application

A digital form application for airway management at Nemours Children's Health, built with Qt 6 and C++17.

## Quick Start

### Prerequisites
- Qt 6.7.3 with MinGW 11.2 64-bit (or Qt 6.5.9 LTS)
- C++17 compatible compiler (GCC 9.0+ or MSVC 2017+ recommended)
- qmake (included with Qt)

**Important**: If you have an older compiler (GCC 8.x), you may encounter C++17 compatibility issues. Please upgrade to Qt 6.7.3 with MinGW 11.2 or use MSVC build tools.

## Build & Run Guides

Choose your development environment:

### For Production/Distribution
- **[Creating Windows Distributable (Portable .exe)](CREATE_WINDOWS_EXECUTABLE.md)** - Complete guide to creating standalone Windows executables

### For Development

#### Windows Development (Recommended)
**Quick Build from Windows Command Prompt:**
```batch
cd C:\_Projects\safe-airway
build-windows-release.bat
release\safe-airway.exe
```

**For detailed Windows build instructions**, see the [Windows Build Guide](#windows-command-prompt) below.

#### WSL Development (Build Windows executables from Linux)
**Quick Build from WSL:**
```bash
cd /mnt/c/_Projects/safe-airway
./build-and-run-windows.sh
```

This script builds Windows .exe files from WSL and launches them in Windows (no WSL GUI issues).

**Manual WSL workflow:**
```bash
# Build Windows release from WSL
cmd.exe /c build-windows-release.bat

# Run Windows executable from WSL
/mnt/c/_Projects/safe-airway/release/safe-airway.exe
```

#### Qt Creator (Any Platform)
1. Open `safe-airway.pro` in Qt Creator
2. Select kit: Desktop Qt 6.7.3 MinGW 11.2 64-bit
3. Configure Project → Build → Run

## Detailed Build Instructions

### Windows (Command Prompt)
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

### Windows Release Build (Distributable)
```batch
REM Using automated build script (Easiest)
cd C:\_Projects\safe-airway
build-windows-release.bat
```

The script will:
- Build optimized release version
- Bundle all Qt dependencies automatically
- Create `safe-airway-dist/` folder with everything needed

**Result:** Portable ~25-40 MB folder that runs on any Windows 10/11 machine.

For detailed distribution instructions, see **[CREATE_WINDOWS_EXECUTABLE.md](CREATE_WINDOWS_EXECUTABLE.md)**

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

#### Windows
- **qmake not found**: Ensure Qt is installed and `QTDIR` is set correctly
  ```batch
  set QTDIR=C:\Qt\6.7.3\mingw_64
  set PATH=%QTDIR%\bin;%PATH%
  ```
- **mingw32-make not found**: Add `%QTDIR%\bin` to your PATH or reinstall Qt with MinGW
- **C++17 filesystem errors (GCC 8.x)**: Upgrade to Qt 6.7.3 with MinGW 11.2 or newer
- **Build fails**: Try cleaning first:
  ```batch
  qmake safe-airway.pro
  mingw32-make clean
  mingw32-make -j4
  ```

#### WSL
- **cmd.exe not found**: Ensure you're running from WSL with Windows interop enabled
- **Cannot access C:\ drive**: Mount point should be `/mnt/c/` in WSL
- **Build script fails**: Run from project root: `./build-and-run-windows.sh`

### Runtime Issues
- **Application won't start**: Check that all Qt DLLs are in PATH or use `build-windows-release.bat` for distributable
- **DPI scaling issues**: The application disables Windows DPI scaling for consistent sizing
- **Missing DLLs**: Use the automated build script which bundles all dependencies

### Development Tips
- **Fast iteration**: Use `./build-and-run-windows.sh` from WSL for quick rebuild+test cycles
- **Parallel builds**: Use `-j4` or `-j8` with mingw32-make (adjust based on CPU cores)
- **Qt Creator**: Best for development with incremental builds and debugging
- **Clean builds**: Delete `release/` and `debug/` folders when switching build types

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

After changing values, rebuild the application with `build-windows-release.bat` or `./build-and-run-windows.sh`.

## Version
Current version: 1.1.0-alpha

## License
© 2024 Nemours Children's Health