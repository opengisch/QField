VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_FIX = 0
VERSION_RC = 99
VERSION_SUFFIX = ''

CODENAME = 'M...'

VERSION = '$${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_FIX}'

ANDROID_VERSION_SUFFIX = 0
ANDROID_TARGET_ARCH = $$ANDROID_TARGET_ARCH$$

equals ( ANDROID_TARGET_ARCH, 'armeabi-v7a' ) {
  ANDROID_VERSION_SUFFIX = 1
}
equals ( ANDROID_TARGET_ARCH, 'x86' ) {
  ANDROID_VERSION_SUFFIX = 2
}

VERSIONCODE = $$format_number($$format_number($${VERSION_MAJOR}, width=2 zeropad)$$format_number($${VERSION_MINOR}, width=2 zeropad)$$format_number($${VERSION_FIX}, width=2 zeropad)$$format_number($${VERSION_RC}, width=2 zeropad)$$format_number($${ANDROID_VERSION_SUFFIX}))
VERSTR = '$${VERSION}$${VERSION_SUFFIX} - $${CODENAME}'

# Let's force the VERSIONCODE 
# this must match the VERSIONCODE from the android 6+ APK without the last digit
# for example if the APK VERSIONCODE is 10000043, VERSIONCODE_FORCED must be 1000004
VERSIONCODE_FORCED = 1000004
VERSIONCODE = $$format_number($VERSIONCODE_FORCED$ANDROID_VERSION_SUFFIX)

message( 'Building Version $${VERSTR} ($${VERSIONCODE})' )

ESCAPED_VERSTR = $$replace( VERSTR, ' ', '\ ' )
DEFINES += "VERSTR=\\\"$${ESCAPED_VERSTR}\\\""
