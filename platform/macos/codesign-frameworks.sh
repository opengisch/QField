#!/bin/bash

# Source https://stackoverflow.com/a/11284404/2319028


APP_DIR=$1
CODE_SIGN_IDENTITY=$2
echo "Signing ${APP_DIR} with ${CODE_SIGN_IDENTITY}."

ITEMS=""

FRAMEWORKS_DIR="${APP_DIR}/Contents/Frameworks"
if [ -d "$FRAMEWORKS_DIR" ] ; then
    FRAMEWORKS=$(find "${FRAMEWORKS_DIR}" -depth -type d -name "*.framework" -or -name "*.dylib" -or -name "*.bundle" | sed -e "s/\(.*framework\)/\1\/Versions\/A\//")
    RESULT=$?
    if [[ $RESULT != 0 ]] ; then
        exit 1
    fi

    ITEMS="${FRAMEWORKS}"
fi

PLUGINS_DIR="${APP_DIR}/Contents/PlugIns"
if [ -d "$PLUGINS_DIR" ] ; then
    PLUGINS=$(find "${PLUGINS_DIR}" -depth -name "*.dylib" -or -name "*.bundle")
    RESULT=$?
    if [[ $RESULT != 0 ]] ; then
        exit 1
    fi

    ITEMS="${ITEMS}"$'\n'"${PLUGINS}"
fi

LOGINITEMS_DIR="${TARGET_BUILD_DIR}/Contents/Library/LoginItems/"
if [ -d "$LOGINITEMS_DIR" ] ; then
    LOGINITEMS=$(find "${LOGINITEMS_DIR}" -depth -type d -name "*.app")
    RESULT=$?
    if [[ $RESULT != 0 ]] ; then
        exit 1
    fi

    ITEMS="${ITEMS}"$'\n'"${LOGINITEMS}"
fi

echo "Entitlements:"
echo "${CODE_SIGN_ENTITLEMENTS}"

echo "Found:"
echo "${ITEMS}"

# Change the Internal Field Separator (IFS) so that spaces in paths will not cause problems below.
SAVED_IFS=$IFS
IFS=$(echo -en "\n\b")

# Loop through all items.
for ITEM in $ITEMS;
do
    echo "Signing '${ITEM}'"
#    codesign --force --verbose --sign "${CODE_SIGN_IDENTITY}" --entitlements "${CODE_SIGN_ENTITLEMENTS}" "${ITEM}"
    codesign --force --verbose --sign "${CODE_SIGN_IDENTITY}" "${ITEM}"
    RESULT=$?
    if [[ $RESULT != 0 ]] ; then
        echo "Failed to sign '${ITEM}'."
        IFS=$SAVED_IFS
        exit 1
    fi
done

# Restore $IFS.
IFS=$SAVED_IFS
