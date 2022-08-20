#!/bin/bash

set +e

bootcomplete=""
failcounter=0
timeout=600
sleeptime=10
maxfail=$((timeout / sleeptime))

until [[ "${bootcomplete}" =~ "1" ]]; do
    bootcomplete=`adb -e shell getprop dev.bootcomplete 2>&1 &`
    if [[ "${bootcomplete}" =~ "" ]]; then
        ((failcounter += 1))
        echo "Waiting for emulator to start"
        if [[ ${failcounter} -gt ${maxfail} ]]; then
            echo "Timeout ($timeout seconds) reached; failed to start emulator"
            while pkill -9 "emulator" >/dev/null 2>&1; do
                echo "Killing emulator process...."
                pgrep "emulator"
            done
            echo "Process terminated"
            pgrep "emulator"
            exit 1
        fi
    fi
    sleep ${sleeptime}
done

echo "Emulator is ready"
