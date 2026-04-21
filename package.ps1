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

# Package-specific build roots
$nugetDebugBuildDir = 'build/msvc-debug-nuget'
$nugetReleaseBuildDir = 'build/msvc-release-nuget'
$zipDebugBuildDir = 'build/msvc-debug-zip'
$zipReleaseBuildDir = 'build/msvc-release-zip'

# Package-specific install roots
$nugetDebugInstallDir = 'install/msvc-debug-nuget'
$nugetReleaseInstallDir = 'install/msvc-release-nuget'
$zipDebugInstallDir = 'install/msvc-debug-zip'
$zipReleaseInstallDir = 'install/msvc-release-zip'

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

function Resolve-NuGetExecutable {
    $nugetCommand = Get-Command nuget.exe -ErrorAction SilentlyContinue
    if ($nugetCommand) {
        return $nugetCommand.Source
    }

    $candidatePaths = @(
        'build/msvc-release-nuget/NuGet/NuGet.exe',
        'build/msvc-debug-nuget/NuGet/NuGet.exe',
        'build/msvc-release-zip/NuGet/NuGet.exe',
        'build/msvc-debug-zip/NuGet/NuGet.exe',
        'build/msvc-release/NuGet/NuGet.exe',
        'build/msvc-debug/NuGet/NuGet.exe',
        'build/NuGet/NuGet.exe'
    )

    $vsWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vsWhere) {
        $vsPath = & $vsWhere -latest -products * -property installationPath 2>$null
        if ($vsPath) {
            $candidatePaths += Join-Path $vsPath 'Common7\IDE\CommonExtensions\Microsoft\NuGet\NuGet.exe'
        }
    }

    foreach ($candidate in $candidatePaths) {
        $fullPath = Resolve-Path $candidate -ErrorAction SilentlyContinue
        if ($fullPath) {
            return $fullPath.Path
        }
    }

    return $null
}

function Ensure-NuGetOnPath {
    $nugetExe = Resolve-NuGetExecutable
    if (-not $nugetExe) {
        throw "NuGet executable not found. Install NuGet CLI or build first so build/*/NuGet/NuGet.exe exists."
    }

    $nugetDir = Split-Path $nugetExe -Parent
    $pathEntries = $env:PATH -split ';'
    if ($pathEntries -notcontains $nugetDir) {
        $env:PATH = "$nugetDir;$env:PATH"
    }

    Write-Host "  Using NuGet: $nugetExe" -ForegroundColor Gray
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
        [string]$BuildDir,
        [string]$Config,
        [bool]$UseAgility,
        [string]$InstallDir
    )

    $agilityValue = if ($UseAgility) { 'ON' } else { 'OFF' }

    Write-Host "  Configuring $Config (WISDOM_USE_AGILITY_SDK=$agilityValue)..." -ForegroundColor Gray

    $configureArgs = @(
        '-S', '.',
        '-B', $BuildDir,
        '-G', 'Ninja',
        "-DCMAKE_BUILD_TYPE=$Config",
        "-DCMAKE_INSTALL_PREFIX=$InstallDir",
        '-DWISDOM_BUILD_EXAMPLES=OFF',
        '-DWISDOM_BUILD_TESTS=OFF',
        '-DCMAKE_UNITY_BUILD=ON',
        "-DWISDOM_USE_AGILITY_SDK=$agilityValue",
        '-DCPM_SOURCE_CACHE=build/_deps_cache'
    )

    if ($Config -eq 'Release') {
        $configureArgs += '-DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON'
    }

    Invoke-CMake $configureArgs

    Write-Host "  Building $Config..." -ForegroundColor Gray
    Invoke-CMake @('--build', $BuildDir)

    Write-Host "  Installing $Config..." -ForegroundColor Gray
    Invoke-CMake @('--install', $BuildDir)
}

function New-Package {
    param(
        [string]$Generator,
        [string]$OutputPath
    )

    $buildDir = switch ($Generator) {
        'NuGet' { $nugetReleaseBuildDir }
        'ZIP' { $zipReleaseBuildDir }
    }

    if (-not (Test-Path $buildDir)) {
        throw "$Generator build directory not found at '$buildDir'. Run without -SkipBuild or build required artifacts first."
    }

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
    # ZIP: includes DXC and Agility SDK for standalone usage
    $configFile = switch ($Generator) {
        'NuGet' { '../../cmake/install/multi-config-nuget.cmake' }
        'ZIP'   { '../../cmake/install/multi-config.cmake' }
    }

    if ($Generator -eq 'NuGet') {
        Ensure-NuGetOnPath
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
    if ($generateNuGet) {
        if ($buildDebug) { $totalSteps++ }
        if ($buildRelease) { $totalSteps++ }
    }
    if ($generateZip) {
        if ($buildDebug) { $totalSteps++ }
        if ($buildRelease) { $totalSteps++ }
    }
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
    @(
        $nugetDebugBuildDir,
        $nugetReleaseBuildDir,
        $zipDebugBuildDir,
        $zipReleaseBuildDir,
        'build/msvc-debug',
        'build/msvc-release'
    ) | ForEach-Object {
        if (Test-Path $_) { Remove-Item -Recurse -Force $_ }
    }
}

# Build
if (-not $SkipBuild) {
    if ($generateNuGet) {
        if ($buildDebug) {
            $currentStep++
            Write-Host "`n[$currentStep/$totalSteps] Building Debug configuration for NuGet (without Agility SDK)..." -ForegroundColor Yellow
            Build-Configuration -BuildDir $nugetDebugBuildDir -Config 'Debug' -UseAgility $false -InstallDir $nugetDebugInstallDir
        }

        if ($buildRelease) {
            $currentStep++
            Write-Host "`n[$currentStep/$totalSteps] Building Release configuration for NuGet (without Agility SDK)..." -ForegroundColor Yellow
            Build-Configuration -BuildDir $nugetReleaseBuildDir -Config 'Release' -UseAgility $false -InstallDir $nugetReleaseInstallDir
        }
    }

    if ($generateZip) {
        if ($buildDebug) {
            $currentStep++
            Write-Host "`n[$currentStep/$totalSteps] Building Debug configuration for ZIP (with Agility SDK)..." -ForegroundColor Yellow
            Build-Configuration -BuildDir $zipDebugBuildDir -Config 'Debug' -UseAgility $true -InstallDir $zipDebugInstallDir
        }

        if ($buildRelease) {
            $currentStep++
            Write-Host "`n[$currentStep/$totalSteps] Building Release configuration for ZIP (with Agility SDK)..." -ForegroundColor Yellow
            Build-Configuration -BuildDir $zipReleaseBuildDir -Config 'Release' -UseAgility $true -InstallDir $zipReleaseInstallDir
        }
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
