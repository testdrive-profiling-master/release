@echo off
cd /D %TESTDRIVE_DIR%

echo "Installing new fonts..."
copy /Y bin\fonts\CascadiaMono.ttf %systemroot%\Fonts
copy /Y bin\fonts\CascadiaCode.ttf %systemroot%\Fonts
