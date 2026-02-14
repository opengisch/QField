

# File qfieldpositioningservice.h

[**File List**](files.md) **>** [**service**](dir_4f691f982165d1317925cee58b4b4f56.md) **>** [**qfieldpositioningservice.h**](qfieldpositioningservice_8h.md)

[Go to the documentation of this file](qfieldpositioningservice_8h.md)


```C++
/***************************************************************************
  qFieldpositioningservice.h - QFieldPositioningService

 ---------------------
 begin                : 21.12.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QFIELDPOSITIONINGSERVICE_H
#define QFIELDPOSITIONINGSERVICE_H

#include "positioningsource.h"
#include "qfield_service_export.h"

#include <QRemoteObjectHost>
#include <QTimer>
#include <QtCore/private/qandroidextras_p.h>
#include <QtGlobal>

class PositioningSource;

class QFIELD_SERVICE_EXPORT QFieldPositioningService : public QAndroidService
{
    Q_OBJECT

  public:
    QFieldPositioningService( int &argc, char **argv );
    ~QFieldPositioningService() override;

  private slots:
    void triggerShowNotification();
    void triggerReturnNotification();
    void triggerStopNotification();

  private:
    PositioningSource *mPositioningSource = nullptr;
    QRemoteObjectHost mHost;

    QTimer mNotificationTimer;
};

#endif // QFIELDPOSITIONINGSERVICE_H
```


