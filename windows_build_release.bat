@echo off
::set VCPKG_DEFAULT_TRIPLET=x64-windows
::set VCPKG_INSTALLED_DIR=.\third_party\vcpkg\vcpkg\vcpkg_installed
::set VCPKG_TARGET_ARCHITECTURE=x64
::set VCPKG_MANIFEST_INSTALL=off
::set VCPKG_TARGET_TRIPLET=x64-windows

if not exist _generate ( 
	mkdir _generate 
)
cd _generate

cmake .. -G "Visual Studio 16"
cmake --build .
cd ..
pause
