VERSION_MAJOR = 1
VERSION_MINOR = 4
VERSION_FIX = 0
# for RC (v1.2.3-rc4) or dev (v1.2.3-dev456)
VERSION_NUMBER = 999
VERSION_SUFFIX_STR = ''

CODENAME = 'Olavtoppen'

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
equals ( ANDROID_TARGET_ARCH, 'arm64-v8a' ) {
  ANDROID_VERSION_SUFFIX = 5
}
equals( ANDROID_TARGET_ARCH, 'x86_64' ) {
  ANDROID_VERSION_SUFFIX = 6
}

VERSIONCODE = $$format_number($$format_number($${VERSION_MAJOR}, width=2 zeropad)$$format_number($${VERSION_MINOR}, width=2 zeropad)$$format_number($${VERSION_FIX}, width=2 zeropad)$$format_number($${VERSION_NUMBER}, width=3 zeropad)$$format_number($${ANDROID_VERSION_SUFFIX}))
VERSTR = '$${VERSION}$${VERSION_SUFFIX_STR} - $${CODENAME}'

message( 'Building Version $${VERSTR} ($${VERSIONCODE})' )

ESCAPED_VERSTR = $$replace( VERSTR, ' ', '\ ' )
DEFINES += "VERSIONCODE=\\\"$${VERSIONCODE}\\\""
DEFINES += "VERSTR=\\\"$${ESCAPED_VERSTR}\\\""
