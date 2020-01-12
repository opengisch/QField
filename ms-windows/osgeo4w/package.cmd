@echo off
REM ***************************************************************************
REM    package.cmd
REM    ---------------------
REM    begin                : January 2011
REM    copyright            : (C) 2011 by Juergen E. Fischer
REM    email                : jef at norbit dot de
REM ***************************************************************************
REM *                                                                         *
REM *   This program is free software; you can redistribute it and/or modify  *
REM *   it under the terms of the GNU General Public License as published by  *
REM *   the Free Software Foundation; either version 2 of the License, or     *
REM *   (at your option) any later version.                                   *
REM *                                                                         *
REM ***************************************************************************

setlocal enabledelayedexpansion

set VERSION=%1
set PACKAGE=%2
set PACKAGENAME=%3
set ARCH=%4
set SHA=%5
set SITE=%6
if "%VERSION%"=="" goto usage
if "%PACKAGE%"=="" goto usage
if "%PACKAGENAME%"=="" goto usage
if "%ARCH%"=="" goto usage
if not "%SHA%"=="" set SHA=-%SHA%
if "%SITE%"=="" set SITE=qfield.org
if "%TARGET%"=="" set TARGET=Nightly
if "%BUILDNAME%"=="" set BUILDNAME=%PACKAGENAME%-%VERSION%%SHA%-%TARGET%-VC14-%ARCH%

set BUILDDIR=%CD%\build-%PACKAGENAME%-%ARCH%
if not exist "%BUILDDIR%" mkdir %BUILDDIR%
if not exist "%BUILDDIR%" (echo could not create build directory %BUILDDIR% & goto error)

PATH

call msvc-env.bat %ARCH%
rem call gdal-dev-env.bat TODO changed mku cleanup

PATH

set O4W_ROOT=%OSGEO4W_ROOT:\=/%
set LIB_DIR=%O4W_ROOT%

for %%i in ("%GRASS_PREFIX%") do set GRASS7_VERSION=%%~nxi
set GRASS_VERSIONS=%GRASS7_VERSION%

set TAR=tar.exe
if exist "c:\cygwin\bin\tar.exe" set TAR=c:\cygwin\bin\tar.exe
if exist "c:\cygwin64\bin\tar.exe" set TAR=c:\cygwin64\bin\tar.exe

set BUILDCONF=RelWithDebInfo

cd ..\..
set SRCDIR=%CD%

if "%BUILDDIR:~1,1%"==":" %BUILDDIR:~0,2%
cd %BUILDDIR%

set PKGDIR=%OSGEO4W_ROOT%\apps\%PACKAGENAME%

if exist repackage goto package

if not exist build.log goto build

REM
REM try renaming the logfile to see if it's locked
REM

if exist build.tmp del build.tmp
if exist build.tmp (echo could not remove build.tmp & goto error)

ren build.log build.tmp
if exist build.log goto locked
if not exist build.tmp goto locked

ren build.tmp build.log
if exist build.tmp goto locked
if not exist build.log goto locked

goto build

:locked
echo Logfile locked
if exist build.tmp del build.tmp
goto error

:build
echo BEGIN: %DATE% %TIME%

set >buildenv.log

if exist qgsversion.h del qgsversion.h

if exist CMakeCache.txt if exist skipcmake goto skipcmake

touch %SRCDIR%\CMakeLists.txt

echo CMAKE: %DATE% %TIME%

if "%CMAKEGEN%"=="" set CMAKEGEN=Ninja
if "%OSGEO4W_CXXFLAGS%"=="" set OSGEO4W_CXXFLAGS=/MD /Z7 /MP /Od /D NDEBUG

for %%i in (%PYTHONHOME%) do set PYVER=%%~ni

cmake -G "%CMAKEGEN%" ^
	-D CMAKE_CXX_COMPILER="%CXX:\=/%" ^
	-D CMAKE_C_COMPILER="%CC:\=/%" ^
	-D CMAKE_LINKER="%CMAKE_COMPILER_PATH:\=/%/link.exe" ^
	-D CMAKE_CXX_FLAGS_RELWITHDEBINFO="%OSGEO4W_CXXFLAGS%" ^
	-D CMAKE_PDB_OUTPUT_DIRECTORY_RELWITHDEBINFO="%BUILDDIR%\apps\%PACKAGENAME%\pdb" ^
	-D PEDANTIC=TRUE ^
	-D CMAKE_BUILD_TYPE=%BUILDCONF% ^
	-D GDAL_LIBRARY=%O4W_ROOT%/apps/gdal-dev/lib/gdal_i.lib ^
	-D QT_LIBRARY_DIR=%O4W_ROOT%/lib ^
	-D QT_HEADERS_DIR=%O4W_ROOT%/apps/qt5/include ^
	-D CMAKE_INSTALL_PREFIX=%O4W_ROOT%/apps/%PACKAGENAME% ^
	-D QGIS_CORE_LIBRARY=%OSGEO4W_ROOT%\apps\qgis-dev\lib\qgis_core.lib ^
	-D QGIS_INCLUDE_DIR=%OSGEO4W_ROOT%\apps\qgis-dev\include ^
	%CMAKE_OPT% ^
	%SRCDIR:\=/%

if errorlevel 1 (echo cmake failed & goto error)

if "%CONFIGONLY%"=="1" (echo Exiting after configuring build directory: %CD% & goto end)

:skipcmake
if exist ..\noclean (echo skip clean & goto skipclean)
echo CLEAN: %DATE% %TIME%
cmake --build %BUILDDIR% --target clean --config %BUILDCONF%
if errorlevel 1 (echo clean failed & goto error)

:skipclean
echo ALL_BUILD: %DATE% %TIME%
cmake --build %BUILDDIR% --target qfield --config %BUILDCONF%

goto end

:usage
echo usage: %0 version package packagename arch [sha [site]]
echo sample: %0 1.3.0 38 qfield-dev x86_64 339dbf1 qfield.org
exit /b 1

:error
echo BUILD ERROR %ERRORLEVEL%: %DATE% %TIME%
if exist %PACKAGENAME%-%VERSION%-%PACKAGE%.tar.bz2 del %PACKAGENAME%-%VERSION%-%PACKAGE%.tar.bz2
exit /b 1

:end
echo FINISHED: %DATE% %TIME%

endlocal
