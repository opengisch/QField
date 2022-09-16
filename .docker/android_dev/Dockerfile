FROM ubuntu:22.04
ARG NDK_VERSION=22.1.7171670

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

# Replace llvm-readobj from r25 with a small wrapper script to make parameters compatible with androiddeployqt from Qt 5.14
RUN mv /opt/android-sdk-linux/ndk/${NDK_VERSION}/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-readobj /opt/android-sdk-linux/ndk/${NDK_VERSION}/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-readobj-bin
COPY tools/llvm-readobj /opt/android-sdk-linux/ndk/${NDK_VERSION}/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-readobj

RUN mkdir /home/devel
WORKDIR /home/devel
RUN pip3 install aqtinstall cmake && \
    /usr/local/bin/aqt install-qt linux android 5.14.2 -m qtcharts
ENV ANDROID_NDK_HOME=/opt/android-sdk-linux/ndk/${NDK_VERSION}
ENV ANDROID_NDK_VERSION=${NDK_VERSION}
