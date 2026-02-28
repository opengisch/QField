

# File printlayoutlistmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**printlayoutlistmodel.h**](printlayoutlistmodel_8h.md)

[Go to the documentation of this file](printlayoutlistmodel_8h.md)


```C++
/***************************************************************************
  printlayoutlistmodel.h

 ---------------------
 begin                : 20.8.2018
 copyright            : (C) 2018 by david
 email                : david at opengisch dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef PRINTLAYOUTLISTMODEL_H
#define PRINTLAYOUTLISTMODEL_H

#include <QAbstractListModel>
#include <qgslayoutmanager.h>
#include <qgslayoutpagecollection.h>
#include <qgsprintlayout.h>

class QgsProject;
class QgsVectorLayer;

class PrintLayoutListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )
    Q_PROPERTY( QgsVectorLayer *atlasCoverageLayer READ atlasCoverageLayer WRITE setAtlasCoverageLayer NOTIFY atlasCoverageLayerChanged )

    
    enum Roles
    {
      TitleRow, 
      IndexRow  
    };

    struct PrintLayout
    {
        explicit PrintLayout( const QString &title )
          : title( title )
        {}

        QString title;
    };

  public:
    explicit PrintLayoutListModel( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    QgsProject *project() const;
    void setProject( QgsProject *project );

    QgsVectorLayer *atlasCoverageLayer() const;
    void setAtlasCoverageLayer( QgsVectorLayer *layer );

    Q_INVOKABLE const QString titleAt( int row ) const;

    Q_INVOKABLE void reloadModel();

  signals:
    void projectChanged();
    void atlasCoverageLayerChanged();

  private:
    QgsProject *mProject = nullptr;
    QgsVectorLayer *mAtlasCoverageLayr = nullptr;

    QList<PrintLayout> mPrintLayouts;
};

#endif // PRINTLAYOUTLISTMODEL_H
```


