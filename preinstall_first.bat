@echo off
echo ------------------------------------------------------------------------
echo  Installing pre-installation packages for 'TestDrive Profiling Master'
echo ------------------------------------------------------------------------
echo.
echo You need to install Microsoft's Visual C++ 2015 Redistributable Packages.
echo.
:choise
set /P c=Are you sure you want to continue? [y/n]
if /I "%c%" EQU "Y" goto INSTALL
if /I "%c%" EQU "y" goto INSTALL
if /I "%c%" EQU "N" goto END
if /I "%c%" EQU "n" goto END
goto choise

:INSTALL
echo.
echo Installing pre-installation packages...
echo.
echo    Step #1/2. VS 2015 Redistributable Package x86
start /w .\preinstallation\vs2015\vc_redist.x86.exe /install /quiet /norestart
echo    Step #2/2. VS 2015 Redistributable Package x64
start /w .\preinstallation\vs2015\vc_redist.x64.exe /install /quiet /norestart

echo Done!
echo.
pause
:END
