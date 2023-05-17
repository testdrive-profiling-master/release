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

@rem plug-in list https://raw.githubusercontent.com/notepad-plus-plus/nppPluginList/master/src/pl.x64.json

wget -O hexedit.zip https://github.com/chcg/NPP_HexEdit/releases/download/0.9.12/HexEditor_0.9.12_x64.zip
%TESTDRIVE_DIR%bin\unzip.exe hexedit.zip -d notepad/plugins
del hexedit.zip

wget -O xml_tools.zip https://github.com/morbac/xmltools/releases/download/3.1.1.13/XMLTools-3.1.1.13-x64.zip
%TESTDRIVE_DIR%bin\unzip.exe xml_tools.zip -d notepad/plugins
del xml_tools.zip

wget -O Explorer.zip https://github.com/oviradoi/npp-explorer-plugin/releases/download/v1.9.9/Explorer_x64.zip
%TESTDRIVE_DIR%bin\unzip.exe Explorer.zip -d notepad/plugins

:END
