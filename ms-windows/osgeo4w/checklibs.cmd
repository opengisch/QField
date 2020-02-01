@echo off
REM ***************************************************************************
REM    windeployqfield.cmd
REM    ---------------------
REM    begin                : January 2020
REM    copyright            : (C) 2020 by Matthias Kuhn
REM    email                : matthias@opengis.ch
REM ***************************************************************************
REM *                                                                         *
REM *   This program is free software; you can redistribute it and/or modify  *
REM *   it under the terms of the GNU General Public License as published by  *
REM *   the Free Software Foundation; either version 2 of the License, or     *
REM *   (at your option) any later version.                                   *
REM *                                                                         *
REM ***************************************************************************

setlocal enabledelayedexpansion

set QFIELD_RELEASE_PATH=%1
if "%QFIELD_RELEASE_PATH%"=="" goto usage

if not exist "%QFIELD_RELEASE_PATH%" mkdir %QFIELD_RELEASE_PATH%

set O4W_ROOT=%OSGEO4W_ROOT:\=/%
set LIB_DIR=%O4W_ROOT%

PATH "%O4W_ROOT%\bin;%O4W_ROOT%\apps\qgis-dev\bin;%O4W_ROOT%\apps\Qt5\bin;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;%O4W_ROOT%\apps\gdal-dev\bin;%O4W_ROOT%\apps\proj-dev\bin;%O4W_ROOT%\apps\Qt5\plugins\platforms;%PATH%"

%O4W_ROOT%\bin\Dependencies.exe -modules "%QFIELD_RELEASE_PATH%/qfield.exe | grep "\[NOT_FOUND\]" | grep -vf acceptable_missing_libs.txt > C:\missing_libs.txt
wc -l C:\missing_libs.txt > C:\missing_libs_report.txt
set /p MISSING_LIBS=<C:\missing_libs_report.txt

echo %MISSING_LIBS%
if NOT "%MISSING_LIBS%" == "0 C:\missing_libs_report.txt" goto error

goto end

:usage
echo usage: %0 deploy_folder
echo sample: %0 C:\my_qfield_folder
exit /b 1

:error
echo Check error %ERRORLEVEL%: %DATE% %TIME%
exit /b 1

:end
echo Check finished: %DATE% %TIME%
exit /b 0

endlocal
