# Development guide

## Getting the source code

First check out the source code.
The following commands will fetch the latest code.

```sh
# For this document we will assume you are inside a folder for development.
# We will refer to that as `dev`.
#
# I usually have a folder called `dev` in my user folder and inside a subfolder
# for each software project, here that would be `/home/user/dev/qfield`.
# You are free to choose yours.

git clone git@github.com:opengisch/QField.git
# Alternatively you can use the following URL in case you have not set up SSH keys for github
#   https://github.com/opengisch/QField.git
```

## Linux

You need to have cmake installed.

You have two options to build QField. Using system packages which will
reuse packages installed from your package manager. Or using vcpkg which
will build all the packages from source.

### Using system packages

This will use your system packages.
Make sure you have installed the appropriate `-dev` or `-devel` packages
using your system package manager.
This is much faster to build than the using vcpkg and often the preferred
development method.

Installing all [QGIS development packages](https://github.com/qgis/QGIS/blob/master/INSTALL.md#33-install-build-dependencies)
is a good start. The next step is to install QField specific dependencies,
here is a non-exhaustive list of them on Ubuntu.

```
# TODO: update to qt6 as soon as distros start to ship qt6.5
sudo apt install libqt5sensors5-dev libqt5webview5-dev libqt5multimedia5-plugins libqt5multimedia5 qtmultimedia5-dev libzxingcore-dev libqt5bluetooth5 libqt5charts5 qml-module-qtcharts qtconnectivity5-dev qml-module-qtbluetooth qml-module-qtlocation qml-module-qtwebengine qml-module-qtgraphicaleffects qml-module-qt-labs-settings qml-module-qtquick-controls2 qml-module-qtquick-layouts qml-module-qtwebview qml-module-qtmultimedia qml-module-qtquick-shapes qml-module-qtsensors qml-module-qt-labs-calendar qml-module-qtquick-particles2 zipcmp zipmerge ziptool
```

### Configure
```sh
# Building for Qt6 is the standard, but currently no
# distributions ship Qt 6.5.0
# To be updated
cmake -S QField -B build -D BUILD_WITH_QT6=OFF
```

If you use a locally built QGIS installed to a different
location, use `-DQGIS_ROOT=` to specify this path.

### Using vcpkg

This will build the complete dependency chain from scratch.

```sh
cmake -S QField -B build -DWITH_VCPKG=ON
```

Since this is now building a lot, grab yourself a cold or hot drink
and take a good break. It could well take several hours.

### Build

Now build the application.

```sh
cmake --build build
```

### Tests

To build with tests, you can specify `-DENABLE_TESTS=ON`.
To run the tests, run `ctest` in the build folder.

The testing framework `Catch2` has minimal version 3, so you might need to install it separately and pass it with `-DCatch2_ROOT=${HOME}/vcpkg/packages/catch2_x64-linux`.

## Macos

You need to have cmake and Xcode installed.

The following line will configure the build.

```sh
# We call this from the `dev` folder again
cmake -S QField -B build -GXcode -Tbuildsystem=1 -DWITH_VCPKG=ON
```

Please note that this will download and build the complete dependency
chain of QField. If you ever wanted to read a good book, you will have
a couple of hours to get started.

```sh
cmake --build build
```

## Windows

You need to have the following tools available to build

- cmake
- Visual Studio

### Configure

QField on Windows is always built using vcpkg.
A couple of specific variables should be specified.
The `x-buildtrees-root` flag needs to point to a short path
in order to avoid running into [issues with long paths](https://docs.microsoft.com/en-us/windows/win32/fileio/naming-a-file#enable-long-paths-in-windows-10-version-1607-and-later).


```sh
cmake -S QField -B build \
  -D VCPKG_TARGET_TRIPLET=x64-windows-static \
  -D CMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded\$<\$<CONFIG:Debug>:Debug> \
  -D PKG_CONFIG_EXECUTABLE=build/vcpkg_installed/x64-windows/tools/pkgconf/pkgconf.exe \
  -D VCPKG_INSTALL_OPTIONS="--x-buildtrees-root=C:/build"
```

### Build

```sh
cmake --build build
```

## Android

Android runs on a number of different CPU architectures.
The most common one is `arm64`. The platform to build for is specified via triplet.

The following triplets are possible:

- `arm64-android`
- `arm-android`
- `x64-android`
- `x86-android`

### Using a docker image

There is a simple script that helps building everything by using a docker image.

```sh
triplet=arm64-android ./scripts/build.sh
```

### Building locally

Make sure you have the following tools installed

- cmake
- The Android SDK including NDK

#### Configure

```sh
export ANDROID_NDK_HOME=[path to your android ndk]
cmake -S QField \
      -B build \
      -D VCPKG_TARGET_TRIPLET=arm64-android
```

#### Build

```sh
cmake --build build
```

## iOS Simulators for x64 processors

To compile for iOS simulator, make sure you have installed recent versions of flex and bison (e.g. via homebrew) and added to the path.
You also need the Qt sdk for ios installed.

```sh
brew install flex bison
aqt install-qt mac ios 6.5.0 -O 6.5.0 -m qt5compat qtcharts qtpositioning qtserialport qtconnectivity qtmultimedia qtwebview qtsensors --autodesktop
```

```sh
export PATH="$(brew --prefix flex)/bin:$PATH"
export PATH="$(brew --prefix bison)/bin:$PATH"
export Qt6_DIR=6.5.0/ios
```

### Configure

```sh
cmake -S . -B build-x64-ios -DVCPKG_TARGET_TRIPLET=x64-ios -GXcode -DWITH_VCPKG=ON -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphonesimulator -DCMAKE_SYSTEM_PROCESSOR=x86_64 -DCMAKE_FIND_ROOT_PATH=$Qt6_DIR
cmake --build build-x64-ios
```


## iOS application (ARM-64 processors architecture)

```sh
# Firstly, some compilation dependencies need to be installed, Homebrew could be used.

/bin/bash -c "$( curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh )" # Installing Homebrew
( echo; echo 'eval "$( /opt/homebrew/bin/brew shellenv )"' ) >> $HOME/.zprofile
eval "$( /opt/homebrew/bin/brew shellenv )"

brew install cmake flex bison python pkg-config autoconf automake libtool autoconf-archive # Compilation Tools


# Secondly, Xcode must be installed through the AppStore, then configured.

xcode-select --install
xcodebuild -downloadPlatform iOS
sudo xcode-select --switch /Library/Developer/CommandLineTools
sudo xcode-select -s /Applications/Xcode.app/Contents/Developer


# Thirdly, Installing Qt (Can be installed using 'aqtinstall' or the official tools).

QT_VERSION="6.6.1"
QT_ROOT="$HOME/Documents/qt6"

python3 -m pip install --upgrade pip && python3 -m pip install --upgrade setuptools && pip3 install aqtinstall
aqt install-qt mac ios ${QT_VERSION} -O ${QT_ROOT} -m qt5compat qtcharts qtpositioning qtconnectivity qtmultimedia qtwebview qtsensors --autodesktop


# Fourthly, use CMake to configure the sources and set up the environment for the system to find the build tools.

export PATH="$( brew --prefix flex )/bin:$PATH" && export PATH="$( brew --prefix bison )/bin:$PATH"

QT_ROOT="${QT_ROOT}/${QT_VERSION}"
export Qt6_DIR=${QT_ROOT}
export QT_HOST_PATH=${QT_ROOT}/macos/lib/cmake
export Qt6HostInfo_DIR=${QT_ROOT}/macos/lib/cmake/Qt6HostInfo

cmake -S . -B build-arm64-ios \
	-DCMAKE_FIND_ROOT_PATH=${QT_ROOT}/ios \
	-DCMAKE_PREFIX_PATH=${QT_ROOT}/ios/lib/cmake \
	-DQT_HOST_PATH_CMAKE_DIR=${QT_ROOT}/macos/lib/cmake \
	-DQt6HostInfo_DIR=${QT_ROOT}/macos/lib/cmake/Qt6HostInfo \
	-DVCPKG_TARGET_TRIPLET=arm64-ios \
	-DVCPKG_BUILD_TYPE=release \
	-DCMAKE_SYSTEM_NAME=iOS \
	-DCMAKE_OSX_SYSROOT=iphoneos \
	-DCMAKE_OSX_ARCHITECTURES=arm64 \
	-DWITH_VCPKG=ON \
	-DSYSTEM_QT=ON \
	-GXcode

# The "build/CMakeFiles/[version]/CMakeSystem.cmake" file may need to be modified.
# The "CMAKE_SYSTEM_PROCESSOR" variable must be set to "aarch64" manually (only if the cmake configuration fails).

# In "Xcode" build settings, it is possible that the additional flag "CoreFoundation" had to be removed manually.


# Finally, compile. To install an app on iOS, it must be signed using Xcode tools.
cmake --build build-arm64-ios
```


## Contribute

Before commiting, install pre-commit to auto-format your code.

```
pip install pre-commit
pre-commit install
```
