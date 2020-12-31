#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${DIR}/../scripts/version_number.sh

set -v

[[ $(apk_version_code v1.2.3 x86) != 010203993 ]] && echo "=> failed" && exit 1 || echo "OK"
[[ $(apk_version_code v1.2.3-rc4 arm64_v8a) != 010203042 ]] && echo "=> failed" && exit 1 || echo "OK"
[[ $(app_version_str v1.2.3) != "1.2.3" ]] && echo "=> failed" && exit 1 || echo "OK"
[[ $(app_version_str v1.2.3-rc4) != "1.2.3 RC4" ]] && echo "=> failed" && exit 1 || echo "OK"
