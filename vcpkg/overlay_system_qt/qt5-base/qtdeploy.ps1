# This script is based on the implementation of windeployqt for qt5.7.1
#
# Qt's plugin deployment strategy is that each main Qt Module has a hardcoded
# set of plugin subdirectories. Each of these subdirectories is deployed in
# full if that Module is referenced.
#
# This hardcoded list is found inside qttools\src\windeployqt\main.cpp. For
# updating, inspect the symbols qtModuleEntries and qtModuleForPlugin.

# Note: this function signature and behavior is depended upon by applocal.ps1
function deployPluginsIfQt([string]$targetBinaryDir, [string]$QtPluginsDir, [string]$targetBinaryName) {
    $baseDir = Split-Path $QtPluginsDir -parent
    $binDir = "$baseDir\bin"

    function deployPlugins([string]$pluginSubdirName) {
        if (Test-Path "$QtPluginsDir\$pluginSubdirName") {
            Write-Verbose "  Deploying plugins directory '$pluginSubdirName'"
            New-Item "$targetBinaryDir\plugins\$pluginSubdirName" -ItemType Directory -ErrorAction SilentlyContinue | Out-Null
            Get-ChildItem "$QtPluginsDir\$pluginSubdirName\*.dll" | % {
                deployBinary "$targetBinaryDir\plugins\$pluginSubdirName" "$QtPluginsDir\$pluginSubdirName" $_.Name
                resolve "$targetBinaryDir\plugins\$pluginSubdirName\$($_.Name)"
            }
            Get-ChildItem "$QtPluginsDir\$pluginSubdirName\*.so" | % {
                deployBinary "$targetBinaryDir\plugins\$pluginSubdirName" "$QtPluginsDir\$pluginSubdirName" $_.Name
                resolve "$targetBinaryDir\plugins\$pluginSubdirName\$($_.Name)"
            }
        } else {
            Write-Verbose "  Skipping plugins directory '$pluginSubdirName': doesn't exist"
        }
    }

    # We detect Qt modules in use via the DLLs themselves. See qtModuleEntries in Qt to find the mapping.
    if ($targetBinaryName -imatch ".*Qt5Cored?.(dll|so)") {
        if (!(Test-Path "$targetBinaryDir\qt.conf")) {
            "[Paths]" | Out-File -encoding ascii "$targetBinaryDir\qt.conf"
        }
    } elseif ($targetBinaryName -imatch ".*Qt5Guid?.(dll|so)") {
        Write-Verbose "  Deploying platforms"
        New-Item "$targetBinaryDir\plugins\platforms" -ItemType Directory -ErrorAction SilentlyContinue | Out-Null
        Get-ChildItem "$QtPluginsDir\platforms\qwindows*.dll" | % {
            deployBinary "$targetBinaryDir\plugins\platforms" "$QtPluginsDir\platforms" $_.Name
        }

        deployPlugins "accessible"
        deployPlugins "imageformats"
        deployPlugins "iconengines"
        deployPlugins "platforminputcontexts"
        deployPlugins "styles"
    } elseif ($targetBinaryName -imatch ".*Qt5Networkd?.(dll|so)") {
        deployPlugins "bearer"
        if (Test-Path "$binDir\libcrypto-1_1-x64.dll")
        {
            deployBinary "$targetBinaryDir" "$binDir" "libcrypto-1_1-x64.dll"
            deployBinary "$targetBinaryDir" "$binDir" "libssl-1_1-x64.dll"
        }
        if (Test-Path "$binDir\libcrypto-1_1.dll")
        {
            deployBinary "$targetBinaryDir" "$binDir" "libcrypto-1_1.dll"
            deployBinary "$targetBinaryDir" "$binDir" "libssl-1_1.dll"
        }
    } elseif ($targetBinaryName -imatch ".*Qt5Sqld?.(dll|so)") {
        deployPlugins "sqldrivers"
    } elseif ($targetBinaryName -imatch ".*Qt5Multimediad?.(dll|so)") {
        deployPlugins "audio"
        deployPlugins "mediaservice"
        deployPlugins "playlistformats"
    } elseif ($targetBinaryName -imatch ".*Qt5PrintSupportd?.(dll|so)") {
        deployPlugins "printsupport"
    } elseif ($targetBinaryName -imatch ".*Qt5Qmld?.(dll|so)") {
        if(!(Test-Path "$targetBinaryDir\qml"))
        {
            if (Test-Path "$binDir\..\qml") {
                cp -r "$binDir\..\qml" $targetBinaryDir
            } elseif (Test-Path "$binDir\..\..\qml") {
                cp -r "$binDir\..\..\qml" $targetBinaryDir
            } else {
                throw "FAILED"
            }
        }
    } elseif ($targetBinaryName -imatch ".*Qt5Quickd?.(dll|so)") {
        foreach ($a in @("Qt5QuickControls2", "Qt5QuickControls2d", "Qt5QuickShapes", "Qt5QuickShapesd", "Qt5QuickTemplates2", "Qt5QuickTemplates2d", "Qt5QmlWorkerScript", "Qt5QmlWorkerScriptd", "Qt5QuickParticles", "Qt5QuickParticlesd", "Qt5QuickWidgets", "Qt5QuickWidgetsd", "Qt5PositioningQuick", "Qt5PositioningQuickd", "Qt5Multimedia", "Qt5Multimediad", "Qt5MultimediaQuick", "Qt5MultimediaQuickd", "Qt5Charts", "Qt5Chartsd"))
        {
            if (Test-Path "$binDir\$a.(dll|so)")
            {
                deployBinary "$targetBinaryDir" "$binDir" "$a.(dll|so)"
            }
        }
        deployPlugins "scenegraph"
        deployPlugins "qmltooling"
    } elseif ($targetBinaryName -imatch ".*Qt5Declarative.*(dll|so)") {
        deployPlugins "qml1tooling"
    } elseif ($targetBinaryName -imatch ".*Qt5Positioning.*(dll|so)") {
        deployPlugins "position"
    } elseif ($targetBinaryName -imatch ".*Qt5Location.*(dll|so)") {
        deployPlugins "geoservices"
    } elseif ($targetBinaryName -imatch ".*Qt5Sensors.*(dll|so)") {
        deployPlugins "sensors"
        deployPlugins "sensorgestures"
    } elseif ($targetBinaryName -imatch ".*Qt5WebEngineCore.*(dll|so)") {
        deployPlugins "qtwebengine"
    } elseif ($targetBinaryName -imatch ".*Qt53DRenderer.*(dll|so)") {
        deployPlugins "sceneparsers"
    } elseif ($targetBinaryName -imatch ".*Qt5TextToSpeech.*(dll|so)") {
        deployPlugins "texttospeech"
    } elseif ($targetBinaryName -imatch ".*Qt5SerialBus.*(dll|so)") {
        deployPlugins "canbus"
    }
}
