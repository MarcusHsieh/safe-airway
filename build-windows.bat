@echo off
REM Safe Airway - Windows Build Script
REM Builds the Safe Airway application with all UI optimizations

setlocal enabledelayedexpansion

echo.
echo 🏗️  Safe Airway Windows Build Script
echo ===================================

REM Check if we're in the right directory
if not exist "CMakeLists.txt" (
    echo [ERROR] CMakeLists.txt not found. Please run this script from the safe-airway project root.
    pause
    exit /b 1
)

REM Build type (default to Debug, can be overridden with first argument)
set BUILD_TYPE=%1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Debug

REM Convert build type to lowercase for directory name
set BUILD_DIR=build-%BUILD_TYPE%
call :lowercase BUILD_DIR

echo [INFO] Build type: %BUILD_TYPE%
echo [INFO] Build directory: %BUILD_DIR%

REM Check for required dependencies
echo [INFO] Checking dependencies...

where cmake >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [ERROR] CMake is required but not found in PATH.
    echo Please install CMake and add it to your PATH.
    pause
    exit /b 1
)

REM Try to detect build system
where cl >nul 2>nul
if %ERRORLEVEL% equ 0 (
    set CMAKE_GENERATOR="Visual Studio 17 2022"
    set BUILD_TOOL=msbuild
    echo [INFO] Detected Visual Studio compiler
) else (
    where gcc >nul 2>nul
    if %ERRORLEVEL% equ 0 (
        set CMAKE_GENERATOR="MinGW Makefiles"
        set BUILD_TOOL=make
        echo [INFO] Detected MinGW compiler
    ) else (
        echo [WARNING] No suitable compiler detected. Using default generator.
        set CMAKE_GENERATOR=
        set BUILD_TOOL=cmake
    )
)

REM Clean previous build if exists
if exist "%BUILD_DIR%" (
    echo [INFO] Cleaning previous build directory...
    rmdir /s /q "%BUILD_DIR%"
)

REM Create build directory
echo [INFO] Creating build directory...
mkdir "%BUILD_DIR%"

REM Configure with CMake
echo [INFO] Configuring project with CMake...
if defined CMAKE_GENERATOR (
    cmake -S . -B "%BUILD_DIR%" -G %CMAKE_GENERATOR% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
) else (
    cmake -S . -B "%BUILD_DIR%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
)

if %ERRORLEVEL% neq 0 (
    echo [ERROR] CMake configuration failed
    pause
    exit /b 1
)

echo [SUCCESS] CMake configuration completed

REM Build the project
echo [INFO] Building Safe Airway...

REM Determine number of cores for parallel build
for /f "tokens=2 delims==" %%i in ('wmic cpu get NumberOfLogicalProcessors /value ^| find "="') do set CORES=%%i

if "%BUILD_TOOL%"=="msbuild" (
    cmake --build "%BUILD_DIR%" --config %BUILD_TYPE% --parallel %CORES%
) else (
    cmake --build "%BUILD_DIR%" --parallel %CORES%
)

if %ERRORLEVEL% neq 0 (
    echo [ERROR] Build failed
    pause
    exit /b 1
)

echo [SUCCESS] Build completed successfully!

REM Check if executable was created
set EXECUTABLE=%BUILD_DIR%\safe-airway.exe
if not exist "%EXECUTABLE%" (
    REM Try alternative location for Visual Studio builds
    set EXECUTABLE=%BUILD_DIR%\%BUILD_TYPE%\safe-airway.exe
)

if exist "%EXECUTABLE%" (
    echo [SUCCESS] Executable created: %EXECUTABLE%
    
    echo.
    echo 🎉 Build Complete!
    echo ==================
    echo Executable: %EXECUTABLE%
    echo.
    echo UI Optimizations Included:
    echo • Emergency panel converted to popup overlay (~40%% space savings^)
    echo • Date of birth field removed for more space
    echo • Table replaced with modern widget for better touch interaction
    echo • Complete Tracoe manufacturer support added
    echo • Extended Shiley product lines (6.0-6.5mm sizes^)
    echo • Automatic outer diameter and suction catheter calculations
    echo • Optimized layout for small displays like Surface Pro 8
    echo.
    echo You can now run: "%EXECUTABLE%"
    echo.
) else (
    echo [ERROR] Executable not found after build
    echo Expected location: %BUILD_DIR%\safe-airway.exe
    echo Alternative location: %BUILD_DIR%\%BUILD_TYPE%\safe-airway.exe
    pause
    exit /b 1
)

pause
goto :eof

REM Function to convert string to lowercase
:lowercase
for %%i in (a b c d e f g h i j k l m n o p q r s t u v w x y z) do (
    call set %1=!%1:%%i=%%i!
)
goto :eof