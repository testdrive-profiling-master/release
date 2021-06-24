@echo off

if exist "%TESTDRIVE_DIR%" goto TESTDRIVE_EXIST

@echo First run the TestDrive Profiling Master at once...
goto END

:TESTDRIVE_EXIST
cd %TESTDRIVE_DIR%bin

:MSYS2_REINSTALL_CHECK
IF NOT EXIST "msys64\mingw64\bin\ccache.exe" goto MSYS2_REINSTALL
goto MSYS2_CHECK
:MSYS2_REINSTALL
echo *** Please wait for "MinGW" reinstallation... ***
if exist msys64 (
rmdir /S /Q msys64
)

:MSYS2_CHECK
IF NOT EXIST "msys64\mingw64.exe" goto MSYS2_INSTALL
goto END

:MSYS2_INSTALL
echo Installing MSYS2 for TestDrive...

ToolsAutoDownload.exe mingw
IF EXIST "msys2.tar.xz" goto MSYS2_DECOMPRESS
echo *E: Can't download MSYS2
goto END

:MSYS2_DECOMPRESS
xz -d msys2.tar.xz
IF NOT EXIST "msys2.tar" goto END
tar xvf msys2.tar
del msys2.tar
IF NOT EXIST "msys64\mingw64.exe" goto END

:MINGW64_INSTALL
cd msys64
echo exit > temp.sh
usr\bin\bash --login /temp.sh

echo export LANG="en_US.ANSI"> temp.sh
echo pacman -Sy --noconfirm>> temp.sh
usr\bin\bash --login /temp.sh

echo export LANG="en_US.ANSI"> temp.sh
echo pacman -Sy --noconfirm>> temp.sh
echo pacman -Syu --noconfirm>> temp.sh
usr\bin\bash --login /temp.sh
usr\bin\bash --login /temp.sh

copy ..\msys_mingw_install .
usr\bin\bash --login /msys_mingw_install

echo export LANG="en_US.ANSI"> temp.sh
echo pacman -Sy --noconfirm>> temp.sh
echo pacman -Syu --noconfirm>> temp.sh
usr\bin\bash --login /temp.sh
usr\bin\bash --login /temp.sh


echo Apply msys64 fix

echo export LANG="en_US.ANSI"> temp.sh
echo mkpasswd -l -c ^> /etc/passwd >> temp.sh
echo mkgroup -l -c ^> /etc/group >> temp.sh
usr\bin\bash --login /temp.sh

usr\bin\sed "s/: files db/: files/" < etc\nsswitch.conf > nsswitch.conf
move /Y nsswitch.conf etc\

del /Q msys_mingw_install temp.sh

cd ..

:END
