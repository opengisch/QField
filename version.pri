
VERSIONCODE = 131
VERSTR = '1234 (43213)'

message( 'Building Version $${VERSTR} ($${VERSIONCODE})' )

ESCAPED_VERSTR = $$replace( $$replace( $$replace( VERSTR, ' ', '\ ' ), '(', '\(' ), ')', '\)' )
DEFINES += "VERSIONCODE=\\\"$${VERSIONCODE}\\\""
DEFINES += "VERSTR=\\\"$${ESCAPED_VERSTR}\\\""
