#ifndef ANDROIDVIEWSTATUS_H
#define ANDROIDVIEWSTATUS_H

#include <QAndroidActivityResultReceiver>
#include "viewstatus.h"

class AndroidViewStatus : public ViewStatus, public QAndroidActivityResultReceiver
{
  public:
    AndroidViewStatus();

    void handleActivityResult( int receiverRequestCode, int resultCode, const QAndroidJniObject &data ) override;
};

#endif // ANDROIDVIEWSTATUS_H
