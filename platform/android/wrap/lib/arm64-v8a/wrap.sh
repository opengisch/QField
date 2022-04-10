#!/system/bin/sh
HERE="$(cd "$(dirname "$0")" && pwd)"
export ASAN_OPTIONS=log_to_syslog=false,allow_user_segv_handler=1
export LD_PRELOAD=$HERE/libclang_rt.asan-aarch64-android.so
"$@"
