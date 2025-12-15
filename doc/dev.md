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
here is a non-exhaustive list of them on Ubuntu 24.10.

```
sudo apt install build-essential cmake qt6-sensors-dev qt6-declarative-dev qt6-positioning-dev qt6-svg-dev qt6-webview-dev qt6-multimedia-dev qt6-connectivity-dev libzxing-dev libqt6charts6 qml6-module-qtcharts qml6-module-qtlocation qml6-module-qtwebengine qml6-module-qt-labs-settings qml6-module-qtquick-controls qml6-module-qtquick-layouts qml6-module-qtwebview qml6-module-qtmultimedia qml6-module-qtquick-shapes qml6-module-qtsensors qml6-module-qtquick-particles zipcmp zipmerge ziptool
```

### Configure
```sh
cmake -S QField -B build
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
cmake -S QField -B build -G Xcode -DWITH_VCPKG=ON
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
  -D CMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded$<$<CONFIG:Debug>:Debug>" \
  -D PKG_CONFIG_EXECUTABLE=build/vcpkg_installed/x64-windows-static/tools/pkgconf/pkgconf.exe \
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
- `arm-neon-android`
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
```

```sh
export PATH="$(brew --prefix flex)/bin:$PATH"
export PATH="$(brew --prefix bison)/bin:$PATH"
```

### Configure

```sh
cmake -S . -B build-x64-ios \
  -DVCPKG_TARGET_TRIPLET=x64-ios \
  -GXcode \
  -DWITH_VCPKG=ON \
  -DCMAKE_SYSTEM_NAME=iOS \
  -DCMAKE_OSX_SYSROOT=iphonesimulator \
  -DCMAKE_SYSTEM_PROCESSOR=x86_64 \
  -DWITH_SERIALPORT=OFF

# Build
cmake --build build-x64-ios
```

## iOS application (ARM-64 processors architecture)

```sh
# Firstly, some compilation dependencies need to be installed
# Homebrew can be used for this : https://docs.brew.sh/Installation
brew install cmake flex bison python pkg-config autoconf automake libtool autoconf-archive nasm

# Secondly, Xcode must be installed through the AppStore, then configured
xcode-select --install
sudo xcode-select --switch /Library/Developer/CommandLineTools
sudo xcode-select -s /Applications/Xcode.app/Contents/Developer
xcodebuild -downloadPlatform iOS

# Setup the environment for the build tools
export PATH="$(brew --prefix flex)/bin:$(brew --prefix bison)/bin:$PATH"

# Configure using CMake
#
# In the command below, the following line depends on the host system, i.e. the system
# you are building on. In case you are building on a x64 (Intel) based system instead of
# an Apple Silicon-based system (M1/M2/..), change this line:
#
#   -DVCPKG_HOST_TRIPLET=arm64-osx \
#
# into
#
#   -DVCPKG_HOST_TRIPLET=x64-osx \
#

cmake -S . -B build-arm64-ios \
  -DVCPKG_HOST_TRIPLET=arm64-osx \
  -DVCPKG_TARGET_TRIPLET=arm64-ios \
  -DWITH_VCPKG=ON \
  -DVCPKG_BUILD_TYPE=release \
  -DCMAKE_SYSTEM_NAME=iOS \
  -DCMAKE_OSX_SYSROOT=iphoneos \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DCMAKE_SYSTEM_PROCESSOR=arm64 \
  -DWITH_SERIALPORT=OFF \
  -GXcode
```

Verify CMake has generated the project without any errors.
### Open the generated Xcode project in xcode first with:
```sh
open build-arm64-ios/QField.xcodeproj
```

1. In Xcode:

   - Select the **QField** scheme.
   - Choose a real iOS device or a connected device as the run target.
   - Go to the **QField** target -> **Signing & Capabilities** tab.
   - Set your **Team** and ensure a valid **Bundle Identifier** and signing certificate are configured.
   - Resolve any provisioning profile issues shown by Xcode if any.
2. Once signing is set up, build and run QField from Xcode to verify the project compiles and deploys correctly to your device.

### Building from the terminal

After you have successfully built the app from Xcode (and signing is correctly configured), you can build from the command line as usual:

```sh
cmake --build build-arm64-ios
```


## Contribute

Before commiting, install pre-commit to auto-format your code.

```sh
pip install pre-commit
pre-commit install
```

## Developer Walkthrough for Linux

The commands below are using `apt`, so they are written for Debian based systems like Ubuntu. Adapt accordingly for other distributions.

### Initial Setup

1. **Clone the Source Code:**
   - Use `git` to clone the repository from GitHub.

2. **Install Required Tools:**
   - Install Qt Creator, a compiler, and other dependencies:
     ```bash
     sudo apt update
     sudo apt install qtcreator build-essential
     ```

#### Build

1. **Build from Command Line:**
   - Run the convenience script to build the project:
     ```bash
     ./scripts/build-for-linux.sh
     ```

2. **Resolve Dependencies:**
   - If the build fails, check the error output for missing dependencies and install them as recommended.

#### Setup the IDE

1. **Open Qt Creator:**
   - Launch Qt Creator from your application menu or terminal.

2. **Setup Qt Installation:**
   - Go to `Preferences -> Kits -> Qt Versions -> Add`.
   - Select `qmake` from `build-x64-linux/vcpkg_installed/x64-linux/tools/Qt6/bin/qmake`.

3. **Create a New Kit:**
   - Add a new kit in the `Kits` tab, selecting the newly added Qt version.

4. **Create a New Project:**
   - Open the `CMakeLists.txt` file from the source code directory.
   - Select the newly created kit with a Debug configuration, pointing to the `build-x64-linux` directory.

You are now ready to develop and run the project using Qt Creator!

## Dev Container

QField provides a devcontainer configuration that helps you get started with development quickly. The devcontainer is based on Ubuntu 22.04 and includes all necessary dependencies.

### Prerequisites

- Podman or Docker installed on your system
- CLion or Visual Studio Code with the "Dev Containers" extension

### Using with CLion

1. Open the QField project in your CLion
2. The IDE should detect the devcontainer configuration and prompt you to use it
3. Click "Reopen in Container" and wait for the environment to be ready

### Using with VS Code

1. Open the QField folder in VS Code
2. When prompted, click "Reopen in Container" or run the "Remote-Containers: Reopen in Container" command
3. Wait for the container to build and initialize

### Building in the Container

Once inside the container, you can follow the regular [vcpkg build instructions](#using-vcpkg) or use automated tools provided by the IDE. Pre-commit is shipped with the devcontainer, you just have to initialize it for the repo with `pre-commit install`.

### GUI Support

If you got xcb errors when starting the built QField, you can enable GUI support by running this command on your **host** system:

```sh
xhost +local:
```
