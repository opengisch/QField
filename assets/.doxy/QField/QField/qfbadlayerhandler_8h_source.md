

# File qfbadlayerhandler.h

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qfbadlayerhandler.h**](qfbadlayerhandler_8h.md)

[Go to the documentation of this file](qfbadlayerhandler_8h.md)


```C++
/***************************************************************************
  qfbadlayerhandler.h - QfBadLayerHandler

 ---------------------
 begin                : 3.10.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QFBADLAYERHANDLER_H
#define QFBADLAYERHANDLER_H

#include <QStandardItemModel>
#include <qgsproject.h>
#include <qgsprojectbadlayerhandler.h>

class QfBadLayerHandler : public QStandardItemModel, public QgsProjectBadLayerHandler
{
    Q_OBJECT

    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )

  public:
    enum Roles
    {
      DataSourceRole = Qt::UserRole,
      LayerNameRole
    };

    explicit QfBadLayerHandler( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;

    QgsProject *project() const;
    void setProject( QgsProject *project );

    void handleBadLayers( const QList<QDomNode> &layers ) override;

  signals:
    void projectChanged();
    void badLayersFound();

  private:
    QString layerName( const QDomNode &layerNode ) const;

    QgsProject *mProject = nullptr;
};

#endif // QFBADLAYERHANDLER_H
```


