VERSION_MAJOR = 0
VERSION_MINOR = 3
VERSION_FIX   = 3

VERSION = '$${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_FIX}'

VERSIONCODE = $$format_number($$format_number($${VERSION_MAJOR}, width=2 zeropad)$$format_number($${VERSION_MINOR}, width=2 zeropad)$$format_number($${VERSION_FIX}, width=2 zeropad))

CODENAME = 'Cerro Torre'
VERSTR = '$${VERSION} - $${CODENAME}'

ESCAPED_VERSTR = $$replace( VERSTR, ' ', '\ ' )
DEFINES += "VERSTR=\\\"$${ESCAPED_VERSTR}\\\""
