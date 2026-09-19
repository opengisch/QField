

# File qffeaturelistextentcontroller.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qffeaturelistextentcontroller.h**](qffeaturelistextentcontroller_8h.md)

[Go to the documentation of this file](qffeaturelistextentcontroller_8h.md)


```C++
/***************************************************************************

               ----------------------------------------------------
              date                 : 27.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias.kuhn (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFFEATURELISTEXTENTCONTROLLER_H
#define QFFEATURELISTEXTENTCONTROLLER_H

#include "qffeaturelistmodelselection.h"
#include "qfmultifeaturelistmodel.h"
#include "qgsquickmapsettings.h"

#include <QObject>
#include <QQuickItem>

class QfFeatureListExtentController : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QfMultiFeatureListModel *model MEMBER mModel NOTIFY modelChanged )
    Q_PROPERTY( QfFeatureListModelSelection *selection MEMBER mSelection NOTIFY selectionChanged )
    Q_PROPERTY( bool autoZoom MEMBER mAutoZoom NOTIFY autoZoomChanged )
    Q_PROPERTY( bool keepScale MEMBER mKeepScale NOTIFY keepScaleChanged )
    Q_PROPERTY( QgsQuickMapSettings *mapSettings MEMBER mMapSettings NOTIFY mapSettingsChanged )

  public:
    explicit QfFeatureListExtentController( QObject *parent = nullptr );
    ~QfFeatureListExtentController();

    QfFeatureListModelSelection *selection() const;

    QfMultiFeatureListModel *model() const;

    void requestFeatureFormState();

  public slots:
    void zoomToSelected( bool skipIfIntersects = false ) const;

    void zoomToAllFeatures() const;

    QgsPoint getCentroidFromSelected() const;

  signals:
    void autoZoomChanged();
    void keepScaleChanged();
    void selectionChanged();
    void modelChanged();
    void mapSettingsChanged();
    void featureFormStateRequested();
    void requestJumpToPoint( const QgsPoint &center, const double &scale = -1.0, bool handleMargins = false ) const;

  private slots:
    void onModelChanged();
    void onCurrentSelectionChanged();

  private:
    QfMultiFeatureListModel *mModel = nullptr;
    QfFeatureListModelSelection *mSelection = nullptr;
    QgsQuickMapSettings *mMapSettings = nullptr;
    bool mAutoZoom = false;
    bool mKeepScale = false;
};

#endif // QFFEATURELISTEXTENTCONTROLLER_H
```


