#Requires -Version 5.1

<#
.SYNOPSIS
    Version management script for Wisdom project
.DESCRIPTION
    Manages project version stored in VERSION file
.PARAMETER Command
    Action to perform: Get, Set, Major, Minor, or Patch
.PARAMETER Version
    Version to set (required for Set command)
.EXAMPLE
    .\version.ps1 Get
    .\version.ps1 Set 1.0.0
    .\version.ps1 Major
    .\version.ps1 Minor
    .\version.ps1 Patch
#>

param(
    [Parameter(Mandatory=$true, Position=0)]
    [ValidateSet('Get', 'Set', 'Major', 'Minor', 'Patch')]
    [string]$Command,
    
    [Parameter(Mandatory=$false, Position=1)]
    [string]$Version
)

$ErrorActionPreference = 'Stop'
$VersionFile = Join-Path $PSScriptRoot 'VERSION'

function Read-Version {
    if (-not (Test-Path $VersionFile)) {
        throw "VERSION file not found at: $VersionFile"
    }
    return (Get-Content $VersionFile -Raw).Trim()
}

function Write-Version {
    param([string]$NewVersion)
    Set-Content -Path $VersionFile -Value "$NewVersion`n" -NoNewline
}

function Test-VersionFormat {
    param([string]$Ver)
    if ($Ver -notmatch '^\d+\.\d+\.\d+$') {
        throw "Invalid version format: $Ver (expected: major.minor.patch)"
    }
}

function Get-VersionComponents {
    param([string]$Ver)
    Test-VersionFormat $Ver
    $parts = $Ver -split '\.'
    return @{
        Major = [int]$parts[0]
        Minor = [int]$parts[1]
        Patch = [int]$parts[2]
    }
}

function Invoke-VersionBump {
    param([string]$BumpType)
    
    $current = Read-Version
    $components = Get-VersionComponents $current
    
    switch ($BumpType) {
        'Major' {
            $components.Major++
            $components.Minor = 0
            $components.Patch = 0
        }
        'Minor' {
            $components.Minor++
            $components.Patch = 0
        }
        'Patch' {
            $components.Patch++
        }
    }
    
    $newVersion = "$($components.Major).$($components.Minor).$($components.Patch)"
    Write-Version $newVersion
    
    Write-Host "Version bumped: $current -> $newVersion" -ForegroundColor Green
    return $newVersion
}

# Main logic
try {
    switch ($Command) {
        'Get' {
            $currentVersion = Read-Version
            Write-Host $currentVersion
        }
        'Set' {
            if ([string]::IsNullOrWhiteSpace($Version)) {
                throw "'Set' command requires -Version parameter"
            }
            Test-VersionFormat $Version
            Write-Version $Version
            Write-Host "Version set to: $Version" -ForegroundColor Green
        }
        { $_ -in @('Major', 'Minor', 'Patch') } {
            $null = Invoke-VersionBump $_
        }
    }
}
catch {
    Write-Error "Error: $_"
    exit 1
}
