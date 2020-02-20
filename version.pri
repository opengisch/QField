
VERSIONCODE = 131
VERSTR = '1234 (43213)'

message( 'Building Version $${VERSTR} ($${VERSIONCODE})' )

DEFINES += "VERSIONCODE=\\\"$${VERSIONCODE}\\\""
DEFINES += "VERSTR=\"\\\"$${VERSTR}\\\"\""
