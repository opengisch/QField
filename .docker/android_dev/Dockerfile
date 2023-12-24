FROM ubuntu:22.04
ARG NDK_VERSION=25.1.8937393

ENV ANDROID_SDK_HOME /opt/android-sdk-linux
ENV ANDROID_SDK_ROOT /opt/android-sdk-linux
ENV ANDROID_HOME /opt/android-sdk-linux
ENV ANDROID_SDK /opt/android-sdk-linux

ENV DEBIAN_FRONTEND noninteractive

# Install required tools
# Dependencies to execute Android builds

RUN dpkg --add-architecture i386 && apt-get update -yqq && apt-get install -y \
  curl \
  expect \
  git \
  make \
  libc6:i386 \
  libgcc1:i386 \
  libncurses5:i386 \
  libstdc++6:i386 \
  zlib1g:i386 \
  openjdk-11-jdk \
  wget \
  unzip \
  vim \
  openssh-client \
  locales \
  cmake \
  git \
  build-essential \
  curl \
  zip \
  unzip \
  tar \
  libarchive-tools \
  autoconf \
  autoconf-archive \
  python3-pip \
  pkg-config \
  libtool \
  flex \
  bison \
  ninja-build \
  jq \
  dirmngr \
  gnupg \
  apt-transport-https \
  ca-certificates \
  software-properties-common \
  gperf \
  autopoint \
  '^libxcb.*-dev' \
  libx11-xcb-dev \
  libegl1-mesa \
  libegl1-mesa-dev \
  libglu1-mesa-dev \
  libxrender-dev \
  libxi-dev \
  libxkbcommon-dev \
  libxkbcommon-x11-dev \
  libgstreamer-gl1.0-0 \
  libgstreamer-plugins-base1.0-0 \
  libfuse2 \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/* \
  && localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8

ENV LANG en_US.UTF-8

RUN groupadd android && useradd -d /opt/android-sdk-linux -g android -u 1000 android

COPY tools /opt/tools

COPY licenses /opt/licenses

WORKDIR /opt/android-sdk-linux

RUN /opt/tools/entrypoint.sh built-in

RUN yes | /opt/android-sdk-linux/cmdline-tools/latest/bin/sdkmanager --licenses && /opt/android-sdk-linux/cmdline-tools/latest/bin/sdkmanager "ndk;${NDK_VERSION}"

RUN mkdir /home/devel
WORKDIR /home/devel
ARG INSTALL_QT_ARCH
ARG INSTALL_QT_VERSION
ENV ANDROID_NDK_HOME=/opt/android-sdk-linux/ndk/${NDK_VERSION}
ENV ANDROID_NDK_VERSION=${NDK_VERSION}
