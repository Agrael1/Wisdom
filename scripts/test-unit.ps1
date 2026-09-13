[CmdletBinding()]
param()

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$workspaceRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$buildDir = Join-Path $workspaceRoot 'build/msvc-debug-tests'

function Initialize-VSEnvironment {
    Write-Host 'Initializing Visual Studio environment...' -ForegroundColor Cyan

    $vsWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (-not (Test-Path $vsWhere)) {
        throw 'Visual Studio not found. Please install Visual Studio with C++ workload.'
    }

    $vsPath = & $vsWhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if (-not $vsPath) {
        throw 'Visual Studio with C++ tools not found.'
    }

    $vcvarsPath = Join-Path $vsPath 'VC\Auxiliary\Build\vcvars64.bat'
    if (-not (Test-Path $vcvarsPath)) {
        throw "vcvars64.bat not found at: $vcvarsPath"
    }

    $env:PATH = ($env:PATH -split ';' | Where-Object { $_ -notmatch 'Strawberry' }) -join ';'

    $envBlock = cmd /c "`"$vcvarsPath`" >nul 2>&1 && set"
    foreach ($line in $envBlock) {
        if ($line -match '^([^=]+)=(.*)$') {
            [Environment]::SetEnvironmentVariable($matches[1], $matches[2], 'Process')
        }
    }
}

function Invoke-ExternalCommand {
    param(
        [Parameter(Mandatory = $true)]
        [string]$FilePath,

        [Parameter(Mandatory = $true)]
        [string[]]$Arguments,

        [Parameter(Mandatory = $true)]
        [string]$ActionName
    )

    if ($VerbosePreference -eq 'Continue') {
        & $FilePath @Arguments
        $exitCode = $LASTEXITCODE
        if ($exitCode -ne 0) {
            throw "$ActionName failed with exit code $exitCode"
        }

        return
    }

    $output = @(& $FilePath @Arguments *>&1)
    $exitCode = $LASTEXITCODE

    if ($exitCode -ne 0) {
        $messages = @(
            $output |
                ForEach-Object { $_.ToString() } |
                Where-Object { $_ -match '(?i)error|failed|exception|fatal' }
        )

        if ($messages.Count -gt 0) {
            $messages | Select-Object -Unique | ForEach-Object { Write-Host "    $_" -ForegroundColor Red }
        }
        else {
            $output |
                ForEach-Object { $_.ToString() } |
                Select-Object -Last 10 |
                ForEach-Object { Write-Host "    $_" -ForegroundColor Red }
        }

        throw "$ActionName failed with exit code $exitCode. Re-run with -Verbose for full logs."
    }
}

Write-Host 'Starting unit tests...' -ForegroundColor Yellow
Write-Host 'Use -Verbose to show full command output.' -ForegroundColor DarkGray

Initialize-VSEnvironment

Write-Host '[1/3] Configure unit test build' -ForegroundColor Cyan
Invoke-ExternalCommand -FilePath 'cmake' -Arguments @(
    '-S', $workspaceRoot,
    '-B', $buildDir,
    '-G', 'Ninja',
    '-DCMAKE_BUILD_TYPE=Debug',
    '-DWISDOM_BUILD_TESTS=ON',
    '-DWISDOM_BUILD_EXAMPLES=OFF'
) -ActionName 'CMake configure'
Write-Host '  OK' -ForegroundColor Green

Write-Host '[2/3] Build unit tests' -ForegroundColor Cyan
Invoke-ExternalCommand -FilePath 'cmake' -Arguments @('--build', $buildDir) -ActionName 'CMake build'
Write-Host '  OK' -ForegroundColor Green

Write-Host '[3/3] Run unit tests' -ForegroundColor Cyan
Invoke-ExternalCommand -FilePath 'ctest' -Arguments @('--test-dir', $buildDir, '--output-on-failure') -ActionName 'CTest run'
Write-Host '  OK' -ForegroundColor Green

Write-Host 'Unit tests completed successfully.' -ForegroundColor Green
