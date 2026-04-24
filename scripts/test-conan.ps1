# 0. Get the script's directory
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$TestDir = Join-Path $ScriptDir "..\tests\integration\conan"
$BaseDir = Join-Path $ScriptDir "\.."

# 1. Test Static
conan create $BaseDir --build=missing

# 2. Test Shared
conan create $BaseDir -o "wisdom/*:shared=True"