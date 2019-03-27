VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_FIX = 2
VERSION_RC = 99
VERSION_SUFFIX = ''

CODENAME = 'Matterhorn'

VERSION = '$${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_FIX}'

# suffix 1 and 2 are used for the android 5 builds
ANDROID_VERSION_SUFFIX = 0
ANDROID_TARGET_ARCH = $$ANDROID_TARGET_ARCH$$

equals ( ANDROID_TARGET_ARCH, 'armeabi-v7a' ) {
  ANDROID_VERSION_SUFFIX = 3
}
equals ( ANDROID_TARGET_ARCH, 'x86' ) {
  ANDROID_VERSION_SUFFIX = 4
}

VERSIONCODE = $$format_number($$format_number($${VERSION_MAJOR}, width=2 zeropad)$$format_number($${VERSION_MINOR}, width=2 zeropad)$$format_number($${VERSION_FIX}, width=2 zeropad)$$format_number($${VERSION_RC}, width=2 zeropad)$$format_number($${ANDROID_VERSION_SUFFIX}))
VERSTR = '$${VERSION}$${VERSION_SUFFIX} - $${CODENAME}'

message( 'Building Version $${VERSTR} ($${VERSIONCODE})' )

ESCAPED_VERSTR = $$replace( VERSTR, ' ', '\ ' )
DEFINES += "VERSIONCODE=\\\"$${VERSIONCODE}\\\""
DEFINES += "VERSTR=\\\"$${ESCAPED_VERSTR}\\\""
