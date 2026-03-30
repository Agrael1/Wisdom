$FormatTargets = @("src", "examples")
$Extensions = @(".h", ".hpp", ".ixx", ".m", ".mm", ".c", ".cpp")

function Format-Files {
    param (
        [Parameter(Mandatory = $true)]
        [string]$TargetPath
    )

    Get-ChildItem -Path $TargetPath -Recurse -File |
        Where-Object { ($Extensions -contains $_.Extension) -and ($_.Name -ne "xxhash.h") } |
        ForEach-Object {
            Write-Host "format $($_.FullName)"
            clang-format -i $_.FullName
        }

    Write-Host "~~~ $TargetPath Done ~~~"
}

foreach ($dir in $FormatTargets) {
    if (Test-Path -Path $dir -PathType Container) {
        Format-Files -TargetPath $dir
    }
}
