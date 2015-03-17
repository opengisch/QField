VERSION = 0.3.2
CODENAME = 'Cerro Torre'
VERSTR = '$${VERSION} - $${CODENAME}'

ESCAPED_VERSTR = $$replace( VERSTR, ' ', '\ ' )
DEFINES += "VERSTR=\\\"$${ESCAPED_VERSTR}\\\""
