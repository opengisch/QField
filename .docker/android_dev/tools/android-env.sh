#!/usr/bin/env bash

export ANDROID_HOME=/opt/android-sdk-linux
export ANDROID_SDK_ROOT=${ANDROID_HOME}
export ANDROID_SDK_HOME=${ANDROID_HOME}
export ANDROID_SDK=${ANDROID_HOME}

export PATH=${PATH}:${ANDROID_HOME}/cmdline-tools/latest/bin:${ANDROID_HOME}/platform-tools:${ANDROID_HOME}/tools/bin:${ANDROID_HOME}/emulator:${ANDROID_HOME}/bin:

if [[ ! -z "$http_proxy" ]] || [[ ! -z "$https_proxy" ]]; then
    export JAVA_OPTS="-Djava.net.useSystemProxies=true $JAVA_OPTS -Dhttp.noProxyHosts=${no_proxy}"
    # This only works if there is a proxy listening on docker host machine and
    # container is started with --net=host. No other options for now. Thanks
    # google....
    export SDKMNGR_OPTS=" --proxy=http --proxy_host=127.0.0.1 --proxy_port=3128 --no_https "
fi

function print_header() {
    figlet SBB CFF FFS
    figlet welcome to
    figlet andep
    echo ''
    echo ''
    echo ''
}

function help() {
    figlet "usage:"
    echo "update_sdk: Updates the SDK"
    echo "andep: Installs one or more android Packets."
    echo "   -Example: anddep \"platforms;android-26\""
    echo "help: Shows this help"
    echo ''
    echo ''
    echo ''
}

function update_sdk() {
    android-accept-licenses.sh "sdkmanager ${SDKMNGR_OPTS} --update"
}

function andep() {
    if [ -z ${1} ]; then
        help
        return 1
    fi
    android-accept-licenses.sh  "sdkmanager ${SDKMNGR_OPTS} ${1}"
}

export -f help
export -f update_sdk
export -f andep
