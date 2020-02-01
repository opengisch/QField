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
set BUILDDIR=%2
set QMLDIR=%3
if "%QFIELD_RELEASE_PATH%"=="" goto usage

if not exist "%QFIELD_RELEASE_PATH%" mkdir %QFIELD_RELEASE_PATH%
if not exist "%BUILDDIR%" (echo invalid build directory specified %BUILDDIR% does not exist & goto error)
if not exist "%BUILDDIR%\output\bin\qfield.exe" (echo invalid build directory specified %BUILDDIR%\output\bin\qfield.exe does not exist & goto error)


set O4W_ROOT=%OSGEO4W_ROOT:\=/%
set LIB_DIR=%O4W_ROOT%

if "%ARCH%"=="x86" (
  set SPATIALINDEX_LIBRARY=%O4W_ROOT%/lib/spatialindex-32.lib
) else (
  set SPATIALINDEX_LIBRARY=%O4W_ROOT%/lib/spatialindex-64.lib
)

PATH "%O4W_ROOT%\bin;%O4W_ROOT%\apps\qgis-dev\bin;%O4W_ROOT%\apps\Qt5\bin;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;%O4W_ROOT%\apps\gdal-dev\bin;%O4W_ROOT%\apps\proj-dev\bin;%O4W_ROOT%\apps\Qt5\plugins\platforms;%PATH%"
SET QT_PLUGIN_PATH="%O4W_ROOT%\apps\qgis-dev\qtplugins;%O4W_ROOT%\apps\Qt5\plugins"
SET QT_QPA_PLATFORM_PLUGIN_PATH=%O4W_ROOT%\apps\Qt5\plugins\platforms
SET QML2_IMPORT_PATH=%O4W_ROOT%\apps\Qt5\qml
SET QML_IMPORT_PATH=%O4W_ROOT%\apps\Qt5\qml
SET QML_IMPORT_TRACE=

copy "%BUILDDIR%\output\bin\*.dll" "%QFIELD_RELEASE_PATH%"
copy "%BUILDDIR%\output\bin\qfield.exe" "%QFIELD_RELEASE_PATH%"
copy "%CD%\..\uwp\AppxManifest.xml" "%QFIELD_RELEASE_PATH%"

mkdir "%QFIELD_RELEASE_PATH%\images"
copy "%CD%\..\..\images\icons\qfield_icon_150.png" "%QFIELD_RELEASE_PATH%\images"
copy "%CD%\..\..\images\icons\qfield_icon_40.png" "%QFIELD_RELEASE_PATH%\images"

copy "%O4W_ROOT%\bin\zlib1.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\netcdf.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\geotiff.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\tiff.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\libtiff.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\sqlite3.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\freexl.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\iconv.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\libxml2.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\LIBMYSQL.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\hdf5.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\szip.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\libcurl.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\zlib1.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\openjp2.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\geos_c.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\geos.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\spatialite.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\spatialindex-64.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\expat.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\zip.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\lwgeom.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\ogdi.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\xerces-c_3_2.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\LIBPQ.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\jpeg.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\jpeg12.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\icuuc56.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\icuin56.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\icudt56.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\libpng16.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\liblzma.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\zstd.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\iconv-2.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\libssl-1_1-x64.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\libcrypto-1_1-x64.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\hdf5_hl.dll" "%QFIELD_RELEASE_PATH%"

copy "%O4W_ROOT%\apps\proj-dev\bin\proj_7_0.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\bin\proj_6_3.dll" "%QFIELD_RELEASE_PATH%"

copy "%O4W_ROOT%\apps\gdal-dev\bin\gdal301.dll" "%QFIELD_RELEASE_PATH%"

copy "%O4W_ROOT%\apps\Qt5\bin\qca-qt5.dll" "%QFIELD_RELEASE_PATH%"
copy "%O4W_ROOT%\apps\Qt5\bin\qt5keychain.dll" "%QFIELD_RELEASE_PATH%"

copy "%O4W_ROOT%\apps\qgis-dev\bin\qgis_core.dll" "%QFIELD_RELEASE_PATH%"

%O4W_ROOT%\apps\Qt5\bin\windeployqt.exe --concurrent --sensors --sql --webchannel --webkitwidgets --webkit --compiler-runtime --qmldir %QMLDIR% "%QFIELD_RELEASE_PATH%\qfield_core.dll"

%O4W_ROOT%\bin\Dependencies.exe -modules "%QFIELD_RELEASE_PATH%/qfield.exe | grep "\[NOT_FOUND\]" | grep -vf acceptable_missing_libs.txt

goto end

:usage
echo usage: %0 deploy_folder build_folder
echo sample: %0 C:\my_qfield_folder C:\qfield_source\ms-windows\osgeo4w\build-qfield\
exit /b 1

:error
echo Deploy error %ERRORLEVEL%: %DATE% %TIME%
exit /b 1

:end
echo Deploy finished: %DATE% %TIME%
exit /b 0

endlocal
