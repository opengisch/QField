# syntax=docker/dockerfile:1.4
FROM ubuntu:24.04

ENV ANDROID_NDK_VERSION=28.2.13676358

# Add apt packages

## vcpkg prerequisites
ENV APT_PACKAGES="git curl zip unzip tar"

## Common build prereqs
ENV APT_PACKAGES="$APT_PACKAGES g++ vim pkg-config cmake ninja-build ca-certificates"

ENV APT_PACKAGES="$APT_PACKAGES autoconf nasm bison python2 flex build-essential libtool libtool-bin gettext automake autoconf-archive libltdl-dev"

## Python related
ENV APT_PACKAGES="$APT_PACKAGES python3-setuptools python3-pip python3-jinja2 python3-venv"

## freeglut
ENV APT_PACKAGES="$APT_PACKAGES libxi-dev libgl1-mesa-dev libglu1-mesa-dev mesa-common-dev libxrandr-dev libxxf86vm-dev"

# glfw3
ENV APT_PACKAGES="$APT_PACKAGES libxinerama-dev libxcursor-dev"

# qtbase
ENV APT_PACKAGES="$APT_PACKAGES libxext-dev libxfixes-dev libxrender-dev \
  libxcb1-dev libx11-xcb-dev libxcb-glx0-dev libxcb-util0-dev \
  libxkbcommon-dev libxcb-keysyms1-dev \
  libxcb-image0-dev libxcb-shm0-dev libxcb-icccm4-dev libxcb-sync-dev \
  libxcb-xfixes0-dev libxcb-shape0-dev libxcb-randr0-dev \
  libxcb-render-util0-dev libxcb-xinerama0-dev libxcb-xkb-dev libxcb-xinput-dev \
  libxcb-cursor-dev libxkbcommon-x11-dev libcups2-dev"

## qtbase for android
ENV APT_PACKAGES="$APT_PACKAGES openjdk-17-jre-headless openjdk-17-jdk-headless"

RUN <<END_OF_SCRIPT bash
set -e
export DEBIAN_FRONTEND=noninteractive

# Apt prereqs itself
apt-get -y update
apt-get -y --no-install-recommends install ca-certificates

# Add apt repos

# Run apt things
apt-get -y update
apt-get -y dist-upgrade

apt-get -y --no-install-recommends install $APT_PACKAGES

# Android SDK/NDK
curl -o /android-sdk-10406996.zip https://dl.google.com/android/repository/commandlinetools-linux-10406996_latest.zip
unzip /android-sdk-10406996.zip -d android-sdk
rm -f android-sdk-10406996.zip

shopt -s extglob
mkdir /android-sdk/cmdline-tools/latest
mv /android-sdk/cmdline-tools/!(latest) /android-sdk/cmdline-tools/latest
yes | /android-sdk/cmdline-tools/latest/bin/sdkmanager --licenses
/android-sdk/cmdline-tools/latest/bin/sdkmanager "platforms;android-35" "build-tools;35.0.1" "platform-tools" "tools" "ndk;${ANDROID_NDK_VERSION}"
END_OF_SCRIPT

ENV ANDROID_SDK_ROOT /android-sdk
ENV ANDROID_NDK_HOME=${ANDROID_SDK_ROOT}/ndk/${ANDROID_NDK_VERSION}

WORKDIR /usr/src
