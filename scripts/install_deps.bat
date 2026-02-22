@echo off
setlocal

set REPO_ROOT=%~dp0..
set VCPKG_DIR=%REPO_ROOT%\third_party\vcpkg

echo =^> Initialising vcpkg submodule...
git -C "%REPO_ROOT%" submodule update --init --recursive third_party/vcpkg

echo =^> Bootstrapping vcpkg...
call "%VCPKG_DIR%\bootstrap-vcpkg.bat" -disableMetrics

echo =^> Installing dependencies (vcpkg.json manifest)...
"%VCPKG_DIR%\vcpkg.exe" install ^
    --triplet x64-windows ^
    --x-manifest-root="%REPO_ROOT%" ^
    --x-install-root="%REPO_ROOT%\third_party\vcpkg_installed"

echo.
echo Done. Now configure with CMake:
echo   cmake -B build -S . -G "Visual Studio 17 2022" -A x64
echo   cmake --build build --config Debug

endlocal
