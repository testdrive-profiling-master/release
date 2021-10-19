@echo off
setlocal

if exist "%TESTDRIVE_DIR%" goto TESTDRIVE_EXIST

@echo First, you must run the 'TestDrive Profiling Master' at once...
goto END

:TESTDRIVE_EXIST
cd %TESTDRIVE_DIR%bin

if exist ".\msys64\usr\bin\pacman.exe" goto UPGRADE_MINGW

call mingw_setup.bat

goto END:

:UPGRADE_MINGW
@echo ***********************************
@echo *** MinGW update in progress... ***
@echo ***********************************
cd msys64

@rem delete locked flag if exist.
if exist ".\var\lib\pacman\db.lck" rm .\var\lib\pacman\db.lck

@rem delete old packages
IF EXIST var\cache\pacman\pkg\*.pkg.tar.xz del /Q var\cache\pacman\pkg\*.pkg.tar.xz

echo export LANG="en_US.ANSI"> temp.sh
echo pacman -Sy --noconfirm>> temp.sh
echo pacman -Syu --noconfirm>> temp.sh
echo pacman -Scc --noconfirm>> temp.sh
for /f "delims= " %%i in ('dir/o-d/tw usr\bin\pacman.exe ^| findstr pacman.exe') do set old_pacman_date=%%i
usr\bin\bash --login /temp.sh
for /f "delims= " %%i in ('dir/o-d/tw usr\bin\pacman.exe ^| findstr pacman.exe') do set new_pacman_date=%%i
if %old_pacman_date% == %new_pacman_date% goto DONE
@echo *********************************************
@echo *** MinGW 2nd phase update in progress... ***
@echo *********************************************
usr\bin\bash --login /temp.sh

@rem delete me~
usr\bin\sed -i "s;/ucrt64/;../;" ucrt64/share/ngspice/scripts/spinit

:DONE
cd ..
@echo Done!

:END
endlocal
