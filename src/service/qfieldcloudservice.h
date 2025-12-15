/***************************************************************************
  qFieldcloudservice.h - QFieldCloudService

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
#ifndef QFIELDCLOUDSERVICE_H
#define QFIELDCLOUDSERVICE_H

#include "qfield_service_export.h"
#include "qfieldcloudconnection.h"

#include <QtCore/private/qandroidextras_p.h>
#include <QtGlobal>

class QFIELD_SERVICE_EXPORT QFieldCloudService : public QAndroidService
{
    Q_OBJECT

  public:
    QFieldCloudService( int &argc, char **argv );
    ~QFieldCloudService() override;

    void initService();
    void uploadPendingAttachments();

  private:
    std::unique_ptr<QFieldCloudConnection> mCloudConnection;
};

#endif // QFIELDCLOUDSERVICE_H
