#!/bin/sh

set -eu

SCRIPT_DIR=$(dirname "$0")
case $SCRIPT_DIR in
    "/"*)
        ;;
    ".")
        SCRIPT_DIR=$(pwd)
        ;;
    *)
        SCRIPT_DIR=$(pwd)/$(dirname "$0")
        ;;
esac

LOG_FILE=/tmp/cppcheck_qgis.txt

rm -f ${LOG_FILE}
echo "Checking ${SCRIPT_DIR}/../src ..."

cppcheck --library=qt.cfg --inline-suppr \
         --template='{file}:{line},{severity},{id},{message}' \
         --enable=all --inconclusive --std=c++11 \
         -DPROJ_VERSION_MAJOR=6 \
         -USIP_RUN \
         -DSIP_TRANSFER= \
         -DSIP_TRANSFERTHIS= \
         -DSIP_INOUT= \
         -DSIP_OUT= \
         -DQ_FLAG= \
         -D_QGIS_VERSION_INT=32600 \
         -j $(nproc) \
	 -isrc/qml \
         ${SCRIPT_DIR}/../src \
         >>${LOG_FILE} 2>&1 &

PID=$!
while kill -0 $PID 2>/dev/null; do
    printf "."
    sleep 1
done
echo " done"
if ! wait $PID; then
    echo "cppcheck could not be started"
    exit 1
fi

ret_code=0

for category in "error" "style" "performance" "warning" "clarifyCalculation" "portability"; do
    if grep "${category}," ${LOG_FILE}  >/dev/null; then
        echo "ERROR: Issues in '${category}' category found:"
        grep "${category}," ${LOG_FILE}
        echo ""
        echo "${category} check failed !"
        ret_code=1
    fi
done

if [ ${ret_code} = 0 ]; then
    echo "cppcheck succeeded"
fi

exit ${ret_code}
