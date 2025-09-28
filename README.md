# Safe Airway Application

A digital form application for airway management at Nemours Children's Health, built with Qt 6 and C++17.

## Quick Start

### Prerequisites
- Qt 6.7.3 with MinGW 11.2 64-bit (or Qt 6.5.9 LTS)
- C++17 compatible compiler (GCC 9.0+ or MSVC 2017+ recommended)
- CMake or qmake (included with Qt)

**Important**: If you have an older compiler (GCC 8.x), you may encounter C++17 compatibility issues. Please upgrade to Qt 6.7.3 with MinGW 11.2 or use MSVC build tools.

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

## Creating a Distributable Windows Executable

To create a standalone Windows executable that can be distributed to other computers without Qt installed:

### Step 1: Build Release Version
```batch
REM Set up Qt environment
set QTDIR=C:\Qt\6.7.3\mingw_64
set PATH=%QTDIR%\bin;%PATH%

REM Navigate to project directory
cd C:\_Projects\safe-airway

REM Clean previous builds
rmdir /s /q release 2>nul
rmdir /s /q debug 2>nul

REM Build release version
qmake safe-airway.pro CONFIG+=release
mingw32-make clean
mingw32-make -j4
```

### Step 2: Create Distribution Directory
```batch
REM Create distribution folder
mkdir safe-airway-dist
cd safe-airway-dist

REM Copy the executable
copy ..\release\safe-airway.exe .
```

### Step 3: Bundle Qt Dependencies
Use Qt's deployment tool to automatically include all required DLLs:

```batch
REM Run windeployqt to bundle Qt libraries
windeployqt.exe safe-airway.exe

REM This will automatically copy:
REM - Qt6Core.dll, Qt6Widgets.dll, Qt6Gui.dll, Qt6PrintSupport.dll
REM - platforms\qwindows.dll (platform plugin)
REM - styles\qwindowsvistastyle.dll (Windows theme)
REM - MinGW runtime libraries
```

### Step 4: Add Additional Runtime Dependencies
```batch
REM Copy MinGW runtime libraries (if not included by windeployqt)
copy "%QTDIR%\bin\libgcc_s_seh-1.dll" . 2>nul
copy "%QTDIR%\bin\libstdc++-6.dll" . 2>nul
copy "%QTDIR%\bin\libwinpthread-1.dll" . 2>nul

REM Copy Visual C++ Redistributable (if using MSVC build)
REM These are usually found in C:\Program Files (x86)\Microsoft Visual Studio\...
REM Or download from Microsoft and install on target machines
```

### Step 5: Test the Distribution
```batch
REM Test on a clean machine or VM without Qt installed
REM The safe-airway-dist folder should now be self-contained

REM Optional: Create installer using NSIS, Inno Setup, or WiX
```

### Alternative: CMake Release Build
```batch
REM Using CMake for release build
rmdir /s /q build-release 2>nul
mkdir build-release
cd build-release

cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
mingw32-make -j4

REM Copy executable to distribution folder
mkdir ..\safe-airway-dist
copy safe-airway.exe ..\safe-airway-dist\

REM Run windeployqt from distribution folder
cd ..\safe-airway-dist
windeployqt.exe safe-airway.exe
```

### Final Distribution Structure
Your distributable folder should look like this:
```
safe-airway-dist/
├── safe-airway.exe          (Main executable)
├── Qt6Core.dll              (Qt core library)
├── Qt6Widgets.dll           (Qt widgets library)
├── Qt6Gui.dll               (Qt GUI library)
├── Qt6PrintSupport.dll      (Qt printing library)
├── libgcc_s_seh-1.dll       (MinGW runtime)
├── libstdc++-6.dll          (MinGW C++ library)
├── libwinpthread-1.dll      (MinGW threading)
├── platforms/
│   └── qwindows.dll         (Windows platform plugin)
└── styles/
    └── qwindowsvistastyle.dll (Windows visual style)
```

### Distribution Notes
- **Size**: Expect ~25-40MB for the complete package
- **Compatibility**: Works on Windows 10/11 (64-bit)
- **Testing**: Always test on a clean machine without Qt installed
- **Installer**: Consider using NSIS or Inno Setup for professional distribution
- **Code Signing**: For professional deployment, sign the executable with a code signing certificate

### Features
- **Four Color-Coded Forms**: Tracheostomy (Teal), New Tracheostomy (Pink), Difficult Airway (Yellow), LTR (Blue)
- **Local JSON Storage**: Cases saved to `~/Documents/SafeAirway/case_saves/`
- **Emergency Scenarios**: Interactive emergency guidance with automatic ETT size synchronization
- **Freeze Mode**: Lock interface during procedures while keeping emergency scenarios accessible
- **Responsive UI**: Percentage-based layout that adapts to different screen sizes
- **Enhanced Visibility**: Enlarged fonts and elements for visibility from 5-10 feet
- **HIPAA Compliant**: Patient last name and MRN hidden from display (preserved in backend)
- **Smart Notifications**: Temporary popup notifications that don't interfere with workflow
- **Case Management**: Save, load, and manage cases with recent files list

### Recent Improvements (v1.0.0-alpha)
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

## Version
Current version: 1.0.0-alpha

## License
© 2024 Nemours Children's Health