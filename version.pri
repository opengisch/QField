
VERSIONCODE = 1
VERSTR = 'TESTING'

message( 'Building Version $${VERSTR} ($${VERSIONCODE})' )

ESCAPED_VERSTR = $$replace( VERSTR, ' ', '\ ' )
DEFINES += "VERSIONCODE=\\\"$${VERSIONCODE}\\\""
DEFINES += "VERSTR=\\\"$${ESCAPED_VERSTR}\\\""
