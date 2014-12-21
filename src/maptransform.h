/***************************************************************************

               ----------------------------------------------------
              date                 : 20.12.2014
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

#ifndef MAPTRANSFORM_H
#define MAPTRANSFORM_H

#include <QtQuick/QQuickTransform>
#include <QtGui/QMatrix4x4>

#include "qgsmapsettingsvariant.h"

class MapTransform : public QQuickTransform
{
    Q_OBJECT
    Q_PROPERTY( QVariant mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

  public:
    MapTransform();
    ~MapTransform();

    void applyTo( QMatrix4x4* matrix ) const;

    const QVariant mapSettings() const;
    void setMapSettings( const QVariant& mapSettings );

  signals:
    void mapSettingsChanged();

  private:
    QgsMapSettings mMapSettings;
    QMatrix4x4 mMatrix;
};

#endif // MAPTRANSFORM_H
