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
    && \
  rm -rf /var/lib/apt/lists/*
