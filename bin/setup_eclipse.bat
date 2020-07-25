@echo off

if exist "%TESTDRIVE_DIR%" goto TESTDRIVE_EXIST

@echo First run the TestDrive Profiling Master at once...
goto END

:TESTDRIVE_EXIST
cd %TESTDRIVE_DIR%bin

:JRE_CHECK
setlocal

for /f "tokens=3" %%g in ('java -version 2^>^&1 ^| findstr /i "64-Bit"') do (
    set JAVAVER=%%g
)

if "%JAVAVER%"=="" goto JRE_INSTALLATION

for /f "tokens=3" %%g in ('java -version 2^>^&1 ^| findstr /i "version"') do (
    set JAVAVER=%%g
)

if "%JAVAVER%"=="" (
	echo Java Runtime Environment is not found!
	goto JRE_INSTALLATION
) else (
	set JAVAVER=%JAVAVER:"=%
	goto JRE_INSTALLED
)

:JRE_INSTALLED
@rem @echo Currently, JRE(Java Runtime Environment) %JAVAVER% is installed...

for /f "delims=. tokens=1-3" %%v in ("%JAVAVER%") do (
    @rem @echo Major: %%v
    @rem @echo Minor: %%w
    @rem @echo Build: %%x
	@rem check major version
	if %%v GTR 1 goto JRE_INSTALLATION_END
	@rem check minor version
	if %%w GEQ 8 goto JRE_INSTALLATION_END
	@echo Your JRE is too old!
	goto JRE_INSTALLATION
)

:JRE_INSTALLATION
@echo Installing the newest JRE(Java Runtime Environment)...
@echo Newest downloading the JRE(Java Runtime Environment)...
ToolsAutoDownload.exe jre
if not exist "jre.exe" (
	@echo JRE can't not be downloaded. Please contact to me.
	goto JRE_INSTALLATION_IS_DONE
)

start /w jre.exe /s

:JRE_INSTALLATION_IS_DONE
del jre.exe

:JRE_INSTALLATION_END
endlocal


:ECLIPSE_CHECK
if not exist ".\eclipse\eclipse.exe" goto CHECK_DIRECTORY

goto CHECK_SVEDITOR

:CHECK_DIRECTORY
if exist eclipse (
@echo cleanup previous Eclipse binaries...
rmdir /S /Q eclipse
)

:DOWNLOAD_ECLIPSE
@echo Downloading Eclipse...
ToolsAutoDownload.exe eclipse
if exist "eclipse.zip" goto UNZIP_ECLIPSE

@echo Can't download Eclipse, please connect your PC to internet...
goto END

:UNZIP_ECLIPSE
%TESTDRIVE_DIR%bin\unzip.exe eclipse.zip
del eclipse.zip
@rem 주의 %는 %% 2개 연달아 써야 함.
@rem curl -L http://downloads.sourceforge.net/project/veditor/development%%20build/net.sourceforge.veditor_1.2.1.c.jar -o eclipse\plugins\net.sourceforge.veditor_1.2.1.c.jar

:CHECK_SVEDITOR
IF EXIST "eclipse\plugins\net.sf.sveditor*" goto END
@echo Installing SVEditor eclipse plugin...
eclipse\eclipsec.exe -clean -purgeHistory -application org.eclipse.equinox.p2.director -noSplash -repository http://sveditor.sourceforge.net/update/,http://download.eclipse.org/releases/photon/ -installIU net.sf.sveditor.feature.group

:END
