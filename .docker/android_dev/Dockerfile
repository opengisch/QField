FROM qbsbuild/qbsdev:focal-android-ndk-r21e-0

RUN apt-get update && \
  DEBIAN_FRONTEND=noninteractive apt-get -qq install \
    cmake \
    git \
    build-essential \
    curl \
    zip \
    unzip \
    tar \
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
    && \
  apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF \
    && \
  apt-add-repository 'deb https://download.mono-project.com/repo/ubuntu stable-focal main' \
    && \
  DEBIAN_FRONTEND=noninteractive apt-get -qq install \
    mono-complete \
    && \
  rm -rf /var/lib/apt/lists/*
WORKDIR /home/devel
RUN pip3 install aqtinstall cmake && \
    /usr/local/bin/aqt install-qt linux android 5.14.2 -m qtcharts

ENV PATH="/home/devel/.local/bin:${PATH}"

# Needs to be in sync with cmake/Platform.cmake's ANDROID_TARGET_PLATFORM value
RUN /home/devel/android/tools/bin/sdkmanager --verbose "platforms;android-30"
