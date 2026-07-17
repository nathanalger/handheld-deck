$ErrorActionPreference = "Stop"

$Root = Split-Path $PSScriptRoot -Parent
$Vcpkg = Join-Path $Root "vcpkg"

if (!(Test-Path $Vcpkg))
{
    git clone https://github.com/microsoft/vcpkg.git $Vcpkg
}

Push-Location $Vcpkg

if (!(Test-Path ".\vcpkg.exe"))
{
    .\bootstrap-vcpkg.bat
}

.\vcpkg.exe install sdl3:x64-windows

Pop-Location