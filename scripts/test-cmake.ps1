param (
    [switch]$NoRun  # Optional flag to disable running the application at the end
)

# 0. Get the script's directory
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

# 1. Read the version
$VersionFilePath = Join-Path $ScriptDir "..\version\VERSION"
$WisdomVersion = (Get-Content $VersionFilePath).Trim()
Write-Host "Automated testing for Wisdom ZIP Package v$WisdomVersion" -ForegroundColor Cyan

# 2. Find the generated ZIP file
$ArtifactsDir = Join-Path $ScriptDir "..\artifacts"
$ZipFile = Get-ChildItem -Path $ArtifactsDir -Filter "wisdom-*$WisdomVersion*.zip" | Select-Object -First 1
if (-Not $ZipFile) {
    Write-Error "Could not find the generated .zip package in $ArtifactsDir"
    exit 1
}

# 3. Unzip the package
$ExtractDir = Join-Path $ScriptDir "..\tests\integration\cmake\extracted"
if (Test-Path $ExtractDir) { Remove-Item -Recurse -Force $ExtractDir }
Write-Host "Extracting $($ZipFile.Name) to $ExtractDir..."
Expand-Archive -Path $ZipFile.FullName -DestinationPath $ExtractDir

# CPack usually puts everything inside a subfolder inside the ZIP (e.g., wisdom-0.7.0-win64)
$ExtractedRoot = Get-ChildItem -Path $ExtractDir | Select-Object -First 1

# 4. Configure the test project using CMake
Write-Host "Configuring Test App via CMake..."
# We pass CMAKE_PREFIX_PATH so find_package() knows exactly where to look
$CmakeInputDir = Join-Path $ScriptDir "..\tests\integration\cmake"
$CmakeBuildDir = Join-Path $ScriptDir "..\tests\integration\cmake\build"
cmake -S $CmakeInputDir -B $CmakeBuildDir -DCMAKE_PREFIX_PATH="$($ExtractedRoot.FullName)"

# 5. Build the test project
Write-Host "Building Test App..."
cmake --build $CmakeBuildDir --config Release

if ($LASTEXITCODE -ne 0) {
    Write-Error "ZIP integration test failed to compile!"
    exit 1
}


if ($NoRun) {
    Write-Host "Skipping execution of Test App due to -NoRun flag..." -ForegroundColor Cyan
} else {
    # 6. Run the compiled executable
    # NOTE: Because it's a dynamic build, Windows needs to find wisdom-shared.dll. 
    # We temporarily add the extracted /bin folder to the environment PATH just for this run.
    $env:PATH = "$($ExtractedRoot.FullName)\bin;$env:PATH"
    
    Write-Host "Running Test App..."
    & (Join-Path $CmakeBuildDir "Release\TestApp.exe")
    Write-Host "Running Test App..."
    & (Join-Path $CmakeBuildDir "Release\TestAppShared.exe")
    Write-Host "Running Test App..."
    & (Join-Path $CmakeBuildDir "Release\TestAppHeaders.exe")
}


Write-Host "ZIP packaging completely validated!" -ForegroundColor Green