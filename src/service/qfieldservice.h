/***************************************************************************
  qfieldservice.h - QFieldService

 ---------------------
 begin                : 04.12.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfield_service_export.h"

#include <QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
#include <QAndroidService>
#include <QtAndroid>
#else
#include <QtCore/private/qandroidextras_p.h>
#endif

class QFIELD_SERVICE_EXPORT QFieldService : public QAndroidService
{
    Q_OBJECT

  public:
    QFieldService( int &argc, char **argv );

    ~QFieldService() override;
};
