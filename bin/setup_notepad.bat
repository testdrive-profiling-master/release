@echo off

if exist "%TESTDRIVE_DIR%" goto TESTDRIVE_EXIST

@echo First run the TestDrive Profiling Master at once...
goto END

:TESTDRIVE_EXIST
cd %TESTDRIVE_DIR%bin

if not exist ".\notepad\unicode\notepad++.exe" goto CHECK_EXIST
@echo Deleting... too old notepad++
taskkill /F /IM notepad++.exe
rmdir /S /Q notepad

:CHECK_EXIST
if not exist ".\notepad\notepad++.exe" goto CHECK_DIRECTORY

@echo Already NotePad was installed.
goto END

:CHECK_DIRECTORY
if not exist ".\notepad\" goto DOWNLOAD_START

@echo cleanup previous NotePad binaries, please wait...
rmdir /S /Q notepad

:DOWNLOAD_START

@echo NotePad downloading...
ToolsAutoDownload.exe notepad++

if exist "notepad.zip" goto UNZIP_NOTEPAD

@echo Can't download NotePad, please connect your PC to internet...
goto END


:UNZIP_NOTEPAD

%TESTDRIVE_DIR%bin\unzip.exe notepad.zip -d notepad
del notepad.zip

@rem %의 경우 %%을 2번 사용해야 함을 기억해야 함.
curl -L https://sourceforge.net/projects/npp-plugins/files/Hex%20Editor/Hex%20Editor%20Plugin%20v0.9.5/HexEditor_0_9_5_UNI_dll.zip -o hexedit.zip

%TESTDRIVE_DIR%bin\unzip.exe hexedit.zip -d notepad/plugins
del hexedit.zip

:END
