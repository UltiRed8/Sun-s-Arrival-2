@echo off
setlocal
echo ======================================
echo  Unreal Project Full Rebuild Script
echo ======================================

echo Waiting for Unreal Editor to close...
:wait_for_editor
tasklist /FI "IMAGENAME eq UnrealEditor.exe" | find /I "UnrealEditor.exe" >nul
if %errorlevel%==0 (
    timeout /t 2 >nul
    goto wait_for_editor
)
echo Unreal Editor closed.

echo.
echo Deleting old build folders...
rmdir /s /q "D:/Unreal/Sun-s-Arrival2/Sun-s-Arrival-2\Binaries"
rmdir /s /q "D:/Unreal/Sun-s-Arrival2/Sun-s-Arrival-2\Intermediate"
del /q "D:/Unreal/Sun-s-Arrival2/Sun-s-Arrival-2\*.sln" >nul 2>&1

echo.
echo Regenerating Visual Studio project files...
set UBT_PATH="D:/UEInstall/UE_5.6/\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
if not exist %UBT_PATH% set UBT_PATH="D:/UEInstall/UE_5.6/\Engine\Binaries\DotNET\UnrealBuildTool.exe"
if not exist %UBT_PATH% (
    echo [ERROR] UnrealBuildTool.exe not found!
    pause
    exit /b 1
)
%UBT_PATH% -projectfiles -project="D:/Unreal/Sun-s-Arrival2/Sun-s-Arrival-2/Sun_s_Arrival.uproject" -game -engine -progress
if %errorlevel% neq 0 (
    echo [ERROR] Failed to generate project files.
    pause
    exit /b %errorlevel%
)

echo.
echo Compiling project...
%UBT_PATH% Sun_s_ArrivalEditor Win64 Development -project="D:/Unreal/Sun-s-Arrival2/Sun-s-Arrival-2/Sun_s_Arrival.uproject" -progress
if %errorlevel% neq 0 (
    echo [ERROR] Build failed.
    pause
    exit /b %errorlevel%
)

echo.
echo Launching Unreal Editor...
start "" "D:\UEInstall\UE_5.6\Engine\Binaries\Win64\UnrealEditor.exe" "D:/Unreal/Sun-s-Arrival2/Sun-s-Arrival-2/Sun_s_Arrival.uproject" -Build

echo.
echo All tasks completed successfully!
pause
endlocal
