

# File androidprojectsource.h

[**File List**](files.md) **>** [**android**](dir_4a4823380b8154b853061c3a6b746fec.md) **>** [**androidprojectsource.h**](androidprojectsource_8h.md)

[Go to the documentation of this file](androidprojectsource_8h.md)


```C++
/***************************************************************************
  androidprojectsource.h - AndroidProjectSource

 ---------------------
 begin                : 19.3.2018
 copyright            : (C) 2018 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef ANDROIDPROJECTSOURCE_H
#define ANDROIDPROJECTSOURCE_H

#include "projectsource.h"

#include <QtCore/private/qandroidextras_p.h>

class AndroidProjectSource : public ProjectSource, public QAndroidActivityResultReceiver
{
    Q_OBJECT

  public:
    explicit AndroidProjectSource( QObject *parent = nullptr );

    void handleActivityResult( int receiverRequestCode, int resultCode, const QJniObject &data ) override;
};

#endif // ANDROIDPROJECTSOURCE_H
```


