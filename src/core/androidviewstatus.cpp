#include "androidviewstatus.h"
#include "qgsmessagelog.h"
#include "qdebug.h"

#include <QtAndroid>

AndroidViewStatus::AndroidViewStatus()
  : ViewStatus( nullptr )
  , QAndroidActivityResultReceiver()
{
}

void AndroidViewStatus::handleActivityResult( int receiverRequestCode, int resultCode, const QAndroidJniObject &data )
{
  //on open file externally
  if ( receiverRequestCode == 102 )
  {
    QAndroidJniObject extras = data.callObjectMethod( "getExtras", "()Landroid/os/Bundle;" );
    QAndroidJniObject errorMessage = QAndroidJniObject::fromString( "ERROR_MESSAGE" );
    errorMessage = extras.callObjectMethod( "getString", "(Ljava/lang/String;)Ljava/lang/String;",
                                            errorMessage.object<jstring>() );
    if ( !errorMessage.toString().isEmpty() )
    {
      emit statusReceived( errorMessage.toString() );
    }
  }
}
