[CmdletBinding()]
param()

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$scriptRoot = $PSScriptRoot

$steps = @(
    @{ Name = 'Build and package artifacts'; Script = Join-Path $scriptRoot 'package.ps1' },
    @{ Name = 'Build and run unit tests'; Script = Join-Path $scriptRoot 'test-unit.ps1' },
    @{ Name = 'Run ZIP (CMake) integration test'; Script = Join-Path $scriptRoot 'test-cmake.ps1' },
    @{ Name = 'Run NuGet integration test'; Script = Join-Path $scriptRoot 'test-nuget.ps1' }
    @{ Name = 'Run Conan integration test'; Script = Join-Path $scriptRoot 'test-conan.ps1' }
)

function Invoke-TestStep {
    param(
        [Parameter(Mandatory = $true)]
        [int]$Index,

        [Parameter(Mandatory = $true)]
        [int]$Total,

        [Parameter(Mandatory = $true)]
        [hashtable]$Step
    )

    Write-Host ("[{0}/{1}] {2}" -f $Index, $Total, $Step.Name) -ForegroundColor Cyan

    if ($VerbosePreference -eq 'Continue') {
        & $Step.Script
        if ($LASTEXITCODE -ne 0) {
            throw "Step failed with exit code ${LASTEXITCODE}: $($Step.Name)"
        }

        Write-Host '  OK' -ForegroundColor Green
        return
    }

    $output = @(& $Step.Script *>&1)
    $exitCode = $LASTEXITCODE

    if ($exitCode -ne 0) {
        $messages = @(
            $output |
                ForEach-Object { $_.ToString() } |
                Where-Object { $_ -match '(?i)error|failed|exception|fatal' }
        )

        Write-Host '  FAILED' -ForegroundColor Red
        if ($messages.Count -gt 0) {
            $messages | Select-Object -Unique | ForEach-Object { Write-Host "    $_" -ForegroundColor Red }
        }
        else {
            $output |
                ForEach-Object { $_.ToString() } |
                Select-Object -Last 10 |
                ForEach-Object { Write-Host "    $_" -ForegroundColor Red }
        }

        throw "Step failed with exit code ${exitCode}: $($Step.Name). Re-run with -Verbose for full logs."
    }

    Write-Host '  OK' -ForegroundColor Green
}

Write-Host "Starting local validation..." -ForegroundColor Yellow
Write-Host "Use -Verbose to show full command output." -ForegroundColor DarkGray

for ($i = 0; $i -lt $steps.Count; $i++) {
    Invoke-TestStep -Index ($i + 1) -Total $steps.Count -Step $steps[$i]
}

Write-Host "All steps completed successfully." -ForegroundColor Green
