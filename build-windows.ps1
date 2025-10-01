# Safe Airway - Windows PowerShell Build Script
# Builds the Safe Airway application with all UI optimizations

param(
    [string]$BuildType = "Debug",
    [switch]$Clean,
    [switch]$Help
)

# Function to write colored output
function Write-Status {
    param([string]$Message)
    Write-Host "[INFO] $Message" -ForegroundColor Blue
}

function Write-Success {
    param([string]$Message)
    Write-Host "[SUCCESS] $Message" -ForegroundColor Green
}

function Write-Warning {
    param([string]$Message)
    Write-Host "[WARNING] $Message" -ForegroundColor Yellow
}

function Write-Error {
    param([string]$Message)
    Write-Host "[ERROR] $Message" -ForegroundColor Red
}

# Help function
function Show-Help {
    Write-Host @"

🏗️  Safe Airway Windows PowerShell Build Script
=============================================

USAGE:
    .\build-windows.ps1 [BuildType] [-Clean] [-Help]

PARAMETERS:
    BuildType    Build configuration (Debug, Release, RelWithDebInfo, MinSizeRel)
                Default: Debug
    
    -Clean       Force clean build (removes existing build directory)
    -Help        Show this help message

EXAMPLES:
    .\build-windows.ps1                    # Debug build
    .\build-windows.ps1 Release            # Release build  
    .\build-windows.ps1 Debug -Clean       # Clean debug build

REQUIREMENTS:
    - CMake (3.16 or later)
    - Qt6 development libraries
    - Visual Studio Build Tools or MinGW
    - Windows 10/11

"@ -ForegroundColor Cyan
}

if ($Help) {
    Show-Help
    exit 0
}

Write-Host ""
Write-Host "🏗️  Safe Airway Windows PowerShell Build Script" -ForegroundColor Cyan
Write-Host "=============================================="  -ForegroundColor Cyan

# Check if we're in the right directory
if (-not (Test-Path "CMakeLists.txt")) {
    Write-Error "CMakeLists.txt not found. Please run this script from the safe-airway project root."
    exit 1
}

# Validate build type
$ValidBuildTypes = @("Debug", "Release", "RelWithDebInfo", "MinSizeRel")
if ($BuildType -notin $ValidBuildTypes) {
    Write-Error "Invalid build type: $BuildType"
    Write-Host "Valid build types: $($ValidBuildTypes -join ', ')" -ForegroundColor Yellow
    exit 1
}

$BuildDir = "build-$($BuildType.ToLower())"

Write-Status "Build type: $BuildType"
Write-Status "Build directory: $BuildDir"

# Check for required dependencies
Write-Status "Checking dependencies..."

# Check CMake
try {
    $cmakeVersion = & cmake --version 2>$null
    if ($LASTEXITCODE -eq 0) {
        $version = ($cmakeVersion[0] -split ' ')[2]
        Write-Success "CMake found: version $version"
    }
} catch {
    Write-Error "CMake is required but not found in PATH."
    Write-Host "Please install CMake and add it to your PATH." -ForegroundColor Yellow
    Write-Host "Download from: https://cmake.org/download/" -ForegroundColor Yellow
    exit 1
}

# Detect build system
$cmakeGenerator = $null
$buildTool = "cmake"

# Check for Visual Studio
try {
    $null = & cl 2>$null
    if ($LASTEXITCODE -ne 0) { throw }
    $cmakeGenerator = "Visual Studio 17 2022"
    $buildTool = "msbuild"
    Write-Success "Detected Visual Studio compiler"
} catch {
    # Check for MinGW
    try {
        $null = & gcc --version 2>$null
        if ($LASTEXITCODE -eq 0) {
            $cmakeGenerator = "MinGW Makefiles" 
            $buildTool = "make"
            Write-Success "Detected MinGW compiler"
        }
    } catch {
        Write-Warning "No suitable compiler detected. Using default generator."
    }
}

# Clean previous build if requested or if exists and Clean flag is set
if ($Clean -and (Test-Path $BuildDir)) {
    Write-Status "Cleaning previous build directory..."
    Remove-Item -Recurse -Force $BuildDir
}

# Create build directory
if (-not (Test-Path $BuildDir)) {
    Write-Status "Creating build directory..."
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

# Configure with CMake
Write-Status "Configuring project with CMake..."

$cmakeArgs = @(
    "-S", ".",
    "-B", $BuildDir,
    "-DCMAKE_BUILD_TYPE=$BuildType"
)

if ($cmakeGenerator) {
    $cmakeArgs += @("-G", $cmakeGenerator)
}

try {
    & cmake @cmakeArgs
    if ($LASTEXITCODE -ne 0) { throw }
    Write-Success "CMake configuration completed"
} catch {
    Write-Error "CMake configuration failed"
    exit 1
}

# Build the project
Write-Status "Building Safe Airway..."

# Get number of cores for parallel build
$cores = (Get-CimInstance Win32_ComputerSystem).NumberOfLogicalProcessors

$buildArgs = @(
    "--build", $BuildDir,
    "--parallel", $cores
)

if ($buildTool -eq "msbuild") {
    $buildArgs += @("--config", $BuildType)
}

try {
    & cmake @buildArgs
    if ($LASTEXITCODE -ne 0) { throw }
    Write-Success "Build completed successfully!"
} catch {
    Write-Error "Build failed"
    exit 1
}

# Check if executable was created
$executable = Join-Path $BuildDir "safe-airway.exe"
if (-not (Test-Path $executable)) {
    # Try alternative location for Visual Studio builds
    $executable = Join-Path $BuildDir "$BuildType\safe-airway.exe"
}

if (Test-Path $executable) {
    Write-Success "Executable created: $executable"
    
    Write-Host ""
    Write-Host "🎉 Build Complete!" -ForegroundColor Green
    Write-Host "==================" -ForegroundColor Green
    Write-Host "Executable: $executable" -ForegroundColor White
    Write-Host ""
    Write-Host "UI Optimizations Included:" -ForegroundColor Cyan
    Write-Host "• Emergency panel converted to popup overlay (~40% space savings)" -ForegroundColor White
    Write-Host "• Date of birth field removed for more space" -ForegroundColor White
    Write-Host "• Table replaced with modern widget for better touch interaction" -ForegroundColor White
    Write-Host "• Complete Tracoe manufacturer support added" -ForegroundColor White
    Write-Host "• Extended Shiley product lines (6.0-6.5mm sizes)" -ForegroundColor White
    Write-Host "• Automatic outer diameter and suction catheter calculations" -ForegroundColor White
    Write-Host "• Optimized layout for small displays like Surface Pro 8" -ForegroundColor White
    Write-Host ""
    Write-Host "You can now run: " -NoNewline -ForegroundColor White
    Write-Host """$executable""" -ForegroundColor Yellow
    Write-Host ""
} else {
    Write-Error "Executable not found after build"
    Write-Host "Expected location: $BuildDir\safe-airway.exe" -ForegroundColor Yellow
    Write-Host "Alternative location: $BuildDir\$BuildType\safe-airway.exe" -ForegroundColor Yellow
    exit 1
}