

# File sensorlistmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**sensorlistmodel.h**](sensorlistmodel_8h.md)

[Go to the documentation of this file](sensorlistmodel_8h.md)


```C++
/***************************************************************************
 sensorlistmodel.h
 ---------------------
 begin                : 25.03.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu at opengisch dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SENSORLISTMODEL_H
#define SENSORLISTMODEL_H

#include "qgssensormodel.h"

#include <QSortFilterProxyModel>
#include <qobjectuniqueptr.h>

class QgsProject;
class QgsVectorLayer;

class SensorListModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )
    Q_PROPERTY( bool showConnectedOnly READ showConnectedOnly WRITE setShowConnectedOnly NOTIFY showConnectedOnlyChanged )

  public:
    explicit SensorListModel( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;

    QgsProject *project() const;

    void setProject( QgsProject *project );

    bool showConnectedOnly() const { return mShowConnectedOnly; }

    void setShowConnectedOnly( bool showConnectedOnly );

    Q_INVOKABLE void connectSensorId( const QString &id ) const;

    Q_INVOKABLE void disconnectSensorId( const QString &id ) const;

  protected:
    bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;

  signals:

    void projectChanged();

    void showConnectedOnlyChanged();

    void sensorErrorOccurred( const QString &errorString );

  private:
    void handleSensorError( const QString &id );

    QgsProject *mProject = nullptr;
    QObjectUniquePtr<QgsSensorModel> mSensorModel;

    bool mShowConnectedOnly = false;
};

#endif // SENSORLISTMODEL_H
```


