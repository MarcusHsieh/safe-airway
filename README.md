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

This section explains how to create a portable Windows .exe for Safe Airway that can run on any Windows 10/11 machine without requiring Qt installation.

#### Important: DPI Scaling Fix
The application now includes automatic high DPI scaling support to ensure proper rendering on Windows displays with scaling (125%, 150%, 200%, etc.). The Windows executable will automatically match your system's display settings.

#### Quick Start (Automated Build)
```batch
REM Using automated build script (Easiest)
cd C:\_Projects\safe-airway
build-windows-release.bat
```

The script will automatically:
- Clean previous builds
- Configure for release mode
- Build the application
- Bundle all Qt dependencies
- Create a ready-to-distribute folder

**Result:** Portable ~25-40 MB folder that runs on any Windows 10/11 machine in `safe-airway-dist/`

#### Manual Build (Step-by-Step)

If you prefer to build manually or the script doesn't work:

**Step 1: Set Up Environment**
```batch
REM Set Qt directory (adjust path if different)
set QTDIR=C:\Qt\6.7.3\mingw_64
set PATH=%QTDIR%\bin;%PATH%

REM Navigate to project
cd C:\_Projects\safe-airway
```

**Step 2: Clean Previous Builds**
```batch
REM Remove old build files
rmdir /s /q release
rmdir /s /q debug
del Makefile*
```

**Step 3: Configure Release Build**
```batch
qmake safe-airway.pro CONFIG+=release
```

**Step 4: Build the Application**
```batch
mingw32-make clean
mingw32-make -j4
```

This will take 2-5 minutes. Your executable will be at: `release\safe-airway.exe`

**Step 5: Create Distribution Folder**
```batch
REM Create distribution directory
mkdir safe-airway-dist
cd safe-airway-dist

REM Copy the executable
copy ..\release\safe-airway.exe .
```

**Step 6: Bundle Qt Dependencies**
```batch
REM Still in safe-airway-dist directory
windeployqt.exe safe-airway.exe --release --no-translations
```

This automatically copies all required Qt DLLs (Qt6Core.dll, Qt6Widgets.dll, Qt6Gui.dll, etc.)

**Step 7: Add MinGW Runtime (If Needed)**
```batch
REM Copy MinGW runtime DLLs
copy "%QTDIR%\bin\libgcc_s_seh-1.dll" .
copy "%QTDIR%\bin\libstdc++-6.dll" .
copy "%QTDIR%\bin\libwinpthread-1.dll" .
```

**Step 8: Verify the Package**

Your `safe-airway-dist` folder should contain:
- safe-airway.exe (Main executable)
- Qt6Core.dll, Qt6Widgets.dll, Qt6Gui.dll, Qt6PrintSupport.dll
- libgcc_s_seh-1.dll, libstdc++-6.dll, libwinpthread-1.dll
- platforms/qwindows.dll
- styles/qwindowsvistastyle.dll

**Total size:** ~25-40 MB

#### Testing the Distributable

**Test on Build Machine:**
1. Navigate to `safe-airway-dist` folder
2. Double-click `safe-airway.exe`
3. Application should start without errors

**Test on Clean Machine (Important):**
1. Copy the entire `safe-airway-dist` folder to a USB drive
2. Test on a Windows machine that **does not have Qt installed**
3. Run `safe-airway.exe` from the USB drive

If it runs successfully, your distribution is complete!

#### Distribution Options

**Option 1: ZIP Archive (Quick & Simple)**
```batch
REM From project root
powershell Compress-Archive -Path safe-airway-dist -DestinationPath SafeAirway-v1.1.0-windows-x64.zip
```

**Option 2: Create an Installer (Professional)**

Use **Inno Setup** (https://jrsoftware.org/isinfo.php) to create a professional installer:

1. Download and install Inno Setup
2. Create a script (`safe-airway-installer.iss`):

```ini
[Setup]
AppName=Safe Airway
AppVersion=1.1.0
DefaultDirName={pf}\SafeAirway
DefaultGroupName=Safe Airway
OutputDir=.
OutputBaseFilename=SafeAirway-Setup-v1.1.0

[Files]
Source: "safe-airway-dist\*"; DestDir: "{app}"; Flags: recursesubdirs

[Icons]
Name: "{group}\Safe Airway"; Filename: "{app}\safe-airway.exe"
Name: "{commondesktop}\Safe Airway"; Filename: "{app}\safe-airway.exe"

[Run]
Filename: "{app}\safe-airway.exe"; Description: "Launch Safe Airway"; Flags: postinstall nowait skipifsilent
```

3. Compile the script in Inno Setup
4. Distribute the resulting `SafeAirway-Setup-v1.1.0.exe`

#### Distribution Verification Checklist

Before distributing, verify:
- [ ] Application starts without errors
- [ ] All forms load correctly
- [ ] Cases can be saved and loaded
- [ ] Emergency scenarios popup works
- [ ] Printing functionality works
- [ ] All buttons and inputs are functional
- [ ] Tested on a clean Windows machine
- [ ] Package size is reasonable (~25-40 MB)
- [ ] No missing DLL errors
- [ ] Application closes cleanly

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