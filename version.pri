VERSION_MAJOR = 0
VERSION_MINOR = 8
VERSION_FIX   = 4

CODENAME = 'Hiendertelltihoren'

VERSION = '$${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_FIX}'

ANDROID_VERSION_SUFFIX = 0
ANDROID_TARGET_ARCH = $$ANDROID_TARGET_ARCH$$

equals ( ANDROID_TARGET_ARCH, 'armeabi-v7a' ) {
  ANDROID_VERSION_SUFFIX = 1
}
equals ( ANDROID_TARGET_ARCH, 'x86' ) {
  ANDROID_VERSION_SUFFIX = 2
}

VERSIONCODE = $$format_number($$format_number($${VERSION_MAJOR}, width=2 zeropad)$$format_number($${VERSION_MINOR}, width=2 zeropad)$$format_number($${VERSION_FIX}, width=2 zeropad)$$format_number($${ANDROID_VERSION_SUFFIX}))
VERSTR = '$${VERSION} - $${CODENAME}'

message( 'Building Version $${VERSTR} ($${VERSIONCODE})' )

ESCAPED_VERSTR = $$replace( VERSTR, ' ', '\ ' )
DEFINES += "VERSTR=\\\"$${ESCAPED_VERSTR}\\\""
