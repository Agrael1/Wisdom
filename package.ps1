#Requires -Version 5.1
<#
.SYNOPSIS
    Builds and packages Wisdom library for distribution.

.DESCRIPTION
    This script builds both Debug and Release configurations of Wisdom
    and generates distribution packages (NuGet, ZIP, or both).

.PARAMETER Format
    Package format(s) to generate: 'nuget', 'zip', or 'all'. Default: 'all'

.PARAMETER Clean
    Clean build directories before building. Default: $false

.PARAMETER SkipBuild
    Skip the build step (use existing build artifacts). Default: $false

.PARAMETER OutputDir
    Directory for output packages. Default: './artifacts'

.PARAMETER Configuration
    Build configuration: 'both', 'debug', or 'release'. Default: 'both'

.EXAMPLE
    .\package.ps1
    Builds and creates all package formats.

.EXAMPLE
    .\package.ps1 -Format nuget -Clean
    Clean build and create NuGet package only.

.EXAMPLE
    .\package.ps1 -Format zip -SkipBuild -OutputDir "./release"
    Create ZIP from existing build, output to ./release folder.
#>

[CmdletBinding()]
param(
    [ValidateSet('nuget', 'zip', 'all')]
    [string]$Format = 'all',

    [switch]$Clean,

    [switch]$SkipBuild,

    [string]$OutputDir = './artifacts',

    [ValidateSet('both', 'debug', 'release')]
    [string]$Configuration = 'both'
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

# Determine package formats to generate
$generateNuGet = $Format -in @('nuget', 'all')
$generateZip = $Format -in @('zip', 'all')

# Build configurations to process
$buildDebug = $Configuration -in @('both', 'debug')
$buildRelease = $Configuration -in @('both', 'release')

function Initialize-VSEnvironment {
    Write-Host "Initializing Visual Studio environment..." -ForegroundColor Cyan

    $vsWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (-not (Test-Path $vsWhere)) {
        throw "Visual Studio not found. Please install Visual Studio with C++ workload."
    }

    $vsPath = & $vsWhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if (-not $vsPath) {
        throw "Visual Studio with C++ tools not found."
    }

    $vcvarsPath = Join-Path $vsPath "VC\Auxiliary\Build\vcvars64.bat"
    if (-not (Test-Path $vcvarsPath)) {
        throw "vcvars64.bat not found at: $vcvarsPath"
    }

    # Remove problematic paths (Strawberry Perl, etc.) BEFORE capturing VS environment
    $env:PATH = ($env:PATH -split ';' | Where-Object { $_ -notmatch 'Strawberry' }) -join ';'

    # Run vcvars64.bat and capture environment
    $envBlock = cmd /c "`"$vcvarsPath`" >nul 2>&1 && set"
    foreach ($line in $envBlock) {
        if ($line -match "^([^=]+)=(.*)$") {
            [Environment]::SetEnvironmentVariable($matches[1], $matches[2], "Process")
        }
    }

    # Verify link.exe is MSVC's linker
    $linkPath = (Get-Command link.exe -ErrorAction SilentlyContinue).Source
    Write-Host "  Using linker: $linkPath" -ForegroundColor Gray
}

function Invoke-CMake {
    param([string[]]$Arguments)

    & cmake @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "CMake command failed: cmake $($Arguments -join ' ')"
    }
}

function Build-Configuration {
    param(
        [string]$Preset,
        [string]$BuildDir,
        [string]$Config
    )

    Write-Host "  Configuring $Config..." -ForegroundColor Gray
    Invoke-CMake @('--preset', $Preset)

    Write-Host "  Building $Config..." -ForegroundColor Gray
    Invoke-CMake @('--build', $BuildDir, '--config', $Config)

    Write-Host "  Installing $Config..." -ForegroundColor Gray
    Invoke-CMake @('--install', $BuildDir, '--config', $Config)
}

function New-Package {
    param(
        [string]$Generator,
        [string]$OutputPath
    )

    $buildDir = "build/msvc-release"
    $cpackDir = Join-Path $buildDir "_CPack_Packages"

    # Clean CPack staging directory to prevent cross-contamination between formats
    if (Test-Path $cpackDir) {
        Write-Host "  Cleaning CPack staging directory..." -ForegroundColor Gray
        Remove-Item -Recurse -Force $cpackDir
    }

    # Also clean any existing packages in the build directory
    $existingPackages = Get-ChildItem -Path $buildDir -Include @('*.nupkg', '*.zip') -ErrorAction SilentlyContinue
    foreach ($pkg in $existingPackages) {
        Remove-Item $pkg.FullName -Force
    }

    # Select the appropriate config file based on generator
    # NuGet: excludes DXC (users get it from Microsoft.Direct3D.DXC package)
    # ZIP: includes everything for standalone usage
    $configFile = switch ($Generator) {
        'NuGet' { '../../cmake/install/multi-config-nuget.cmake' }
        'ZIP'   { '../../cmake/install/multi-config.cmake' }
    }

    Push-Location $buildDir
    try {
        & cpack -G $Generator --config $configFile
        if ($LASTEXITCODE -ne 0) {
            throw "$Generator package generation failed"
        }

        # Move generated packages to output directory
        $pattern = switch ($Generator) {
            'NuGet' { '*.nupkg' }
            'ZIP' { '*.zip' }
        }
        $packages = Get-ChildItem -Filter $pattern -ErrorAction SilentlyContinue
        foreach ($pkg in $packages) {
            $dest = Join-Path $OutputPath $pkg.Name
            Move-Item $pkg.FullName $dest -Force
            Write-Host "  Created: $dest" -ForegroundColor Green
        }
    }
    finally {
        Pop-Location
    }
}

# =============================================================================
# Main Script
# =============================================================================

$totalSteps = 0
if (-not $SkipBuild) {
    if ($buildDebug) { $totalSteps++ }
    if ($buildRelease) { $totalSteps++ }
}
if ($generateNuGet) { $totalSteps++ }
if ($generateZip) { $totalSteps++ }
$currentStep = 0

Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host " Wisdom Package Builder" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Format:        $Format"
Write-Host "  Configuration: $Configuration"
Write-Host "  Output:        $OutputDir"
Write-Host "  Clean:         $Clean"
Write-Host "  Skip Build:    $SkipBuild"
Write-Host "========================================`n" -ForegroundColor Cyan

# Initialize VS environment
Initialize-VSEnvironment

# Create output directory
if (-not (Test-Path $OutputDir)) {
    New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
}
$OutputDir = Resolve-Path $OutputDir

# Clean if requested
if ($Clean) {
    Write-Host "Cleaning build directories..." -ForegroundColor Yellow
    @('build/msvc-debug', 'build/msvc-release') | ForEach-Object {
        if (Test-Path $_) { Remove-Item -Recurse -Force $_ }
    }
}

# Build
if (-not $SkipBuild) {
    if ($buildDebug) {
        $currentStep++
        Write-Host "`n[$currentStep/$totalSteps] Building Debug configuration..." -ForegroundColor Yellow
        Build-Configuration -Preset 'win-msvc-debug-lib' -BuildDir 'build/msvc-debug' -Config 'Debug'
    }

    if ($buildRelease) {
        $currentStep++
        Write-Host "`n[$currentStep/$totalSteps] Building Release configuration..." -ForegroundColor Yellow
        Build-Configuration -Preset 'win-msvc-lib' -BuildDir 'build/msvc-release' -Config 'Release'
    }
}

# Generate packages
if ($generateNuGet) {
    $currentStep++
    Write-Host "`n[$currentStep/$totalSteps] Generating NuGet package..." -ForegroundColor Yellow
    Write-Host "  (DXC excluded - use Microsoft.Direct3D.DXC NuGet package)" -ForegroundColor Gray
    New-Package -Generator 'NuGet' -OutputPath $OutputDir
}

if ($generateZip) {
    $currentStep++
    Write-Host "`n[$currentStep/$totalSteps] Generating ZIP archive..." -ForegroundColor Yellow
    Write-Host "  (Includes DXC for standalone usage)" -ForegroundColor Gray
    New-Package -Generator 'ZIP' -OutputPath $OutputDir
}

# Summary
Write-Host "`n========================================" -ForegroundColor Green
Write-Host " Build Complete!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host "Packages available in: $OutputDir" -ForegroundColor Green
Get-ChildItem $OutputDir -ErrorAction SilentlyContinue | ForEach-Object {
    $size = "{0:N2} MB" -f ($_.Length / 1MB)
    Write-Host "  - $($_.Name) ($size)" -ForegroundColor Gray
}
Write-Host "========================================`n" -ForegroundColor Green
