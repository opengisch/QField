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
RUN pip3 install aqtinstall cmake && \
    /usr/local/bin/aqt install-qt linux android "${INSTALL_QT_VERSION}" "${INSTALL_QT_ARCH}" -m qt5compat qtcharts qtpositioning qtserialport qtconnectivity qtimageformats qtmultimedia qtwebview qtsensors --autodesktop
ENV ANDROID_NDK_HOME=/opt/android-sdk-linux/ndk/${NDK_VERSION}
ENV ANDROID_NDK_VERSION=${NDK_VERSION}
