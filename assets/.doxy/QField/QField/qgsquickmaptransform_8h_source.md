

# File qgsquickmaptransform.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qgsquick**](dir_19c3d7c8ba6dc95926fecc96295d1560.md) **>** [**qgsquickmaptransform.h**](qgsquickmaptransform_8h.md)

[Go to the documentation of this file](qgsquickmaptransform_8h.md)


```C++
/***************************************************************************
  qgsquickmaptransform.h
  --------------------------------------
  Date                 : 27.12.2014
  Copyright            : (C) 2014 by Matthias Kuhn
  Email                : matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSQUICKMAPTRANSFORM_H
#define QGSQUICKMAPTRANSFORM_H

#include <QMatrix4x4>
#include <QQuickItem>


class QgsQuickMapSettings;

class QgsQuickMapTransform : public QQuickTransform
{
    Q_OBJECT

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

  public:
    QgsQuickMapTransform() = default;
    ~QgsQuickMapTransform() = default;

    void applyTo( QMatrix4x4 *matrix ) const;

    QgsQuickMapSettings *mapSettings() const;

    void setMapSettings( QgsQuickMapSettings *mapSettings );

  signals:
    void mapSettingsChanged();

  private slots:
    void updateMatrix();

  private:
    QgsQuickMapSettings *mMapSettings = nullptr; // not owned
    QMatrix4x4 mMatrix;
};

#endif // QGSQUICKMAPTRANSFORM_H
```


