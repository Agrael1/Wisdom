param (
    [switch]$NoRun  # Optional flag to disable running the application at the end
)

# 0. Get the script's directory
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

# 1. Read the version dynamically from your repository's VERSION file
$VersionFilePath = Join-Path $ScriptDir "..\version\VERSION"
if (-Not (Test-Path $VersionFilePath)) {
    Write-Error "Could not find VERSION file at $VersionFilePath"
    exit 1
}

$WisdomVersion = (Get-Content $VersionFilePath).Trim()
Write-Host "Automated testing for Wisdom NuGet Package v$WisdomVersion" -ForegroundColor Cyan

# 1.5 Clear the output directory before building
$GlobalCachePath = "$env:USERPROFILE\.nuget\packages\wisdom\$WisdomVersion"
if (Test-Path $GlobalCachePath) {
    Write-Host "Purging outdated v$WisdomVersion from global NuGet cache..." -ForegroundColor Yellow
    Remove-Item -Path $GlobalCachePath -Recurse -Force
}

# 2. Restore the NuGet package from your local feed, passing the version variable
Write-Host "Restoring NuGet packages..."
$VcxprojPath = Join-Path $ScriptDir "..\tests\integration\nuget\test.vcxproj"
msbuild $VcxprojPath -t:restore -p:RestorePackagesConfig=true /p:WisdomPackageVersion=$WisdomVersion

$Linkages = @("dynamic", "static", "headers")

foreach ($Linkage in $Linkages) {
    Write-Host "`n--- Testing Linkage: $Linkage ---" -ForegroundColor Cyan

    # 3. Build the project using MSBuild
    Write-Host "Building Test App ($Linkage)..."
    msbuild $VcxprojPath /p:Configuration=Release /p:Platform=x64 /p:WisdomPackageVersion=$WisdomVersion /p:WisdomLinkage=$Linkage

    # 4. Verify the build succeeded
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Integration test failed to compile for $Linkage linkage!"
        exit 1
    }

    # 5. Verify your .targets file successfully copied the DLL (only for shared linkage)
    if ($Linkage -eq "shared") {
        $DllPath = Join-Path $ScriptDir "..\tests\integration\nuget\x64\Release\wisdom-shared.dll"
        if (-Not (Test-Path $DllPath)) {
            Write-Error "DLL was not copied to the output directory! Check your .targets DeploymentContent."
            exit 1
        }
    }

    # 6. Run the compiled executable
    if ($NoRun) {
        Write-Host "Skipping execution of Test App due to -NoRun flag..." -ForegroundColor Cyan
    } else {
        Write-Host "Running Test App ($Linkage)..."
        & (Join-Path $ScriptDir "..\tests\integration\nuget\x64\Release\test.exe")
    }
}

Write-Host "`nNuGet packaging completely validated for v$WisdomVersion!" -ForegroundColor Green