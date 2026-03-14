param(
    [string]$BuildDir = "build",
    [string]$AppName = "PaletteRain"
)

$QtDir = "C:\Qt\6.8.3\mingw_64"
$InstallerBin = "C:\Qt\Tools\QtInstallerFramework\4.9\bin"
$MingwDir = "C:\Qt\Tools\mingw1310_64\bin"

# Add tools to PATH
$env:PATH = "$QtDir\bin;$MingwDir;$env:PATH"

Write-Host "Qt framework :" $QtDir
Write-Host "Qt installer :" $InstallerBin
Write-Host "Compiler :" $MingwDir
Write-Host ""

# Configure
cmake -S . -B $BuildDir -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=$QtDir

# Build
cmake --build $BuildDir

# Create deploy folder
$deploy = "$BuildDir\deploy"
New-Item -ItemType Directory -Force -Path $deploy | Out-Null

# Copy exe
Copy-Item "$BuildDir\$AppName.exe" $deploy -Force

# Run windeployqt
& "$QtDir\bin\windeployqt.exe" --release --no-translations "$deploy\$AppName.exe"

Write-Host ""
Write-Host "Deploy folder ready:"
Write-Host "$deploy"
