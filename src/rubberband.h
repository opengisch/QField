/***************************************************************************
  rubberband.h - Rubberband

 ---------------------
 begin                : 11.6.2016
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
#ifndef RUBBERBAND_H
#define RUBBERBAND_H

#include <QQuickItem>

class RubberbandModel;
class MapSettings;

class Rubberband : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY( RubberbandModel* model READ model WRITE setModel NOTIFY modelChanged )
    Q_PROPERTY( MapSettings* mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

  public:
    Rubberband( QQuickItem* parent = nullptr );

    RubberbandModel* model() const;
    void setModel( RubberbandModel* model );

    MapSettings* mapSettings() const;
    void setMapSettings( MapSettings* mapSettings );

  signals:
    void modelChanged();
    void mapSettingsChanged();

  private slots:
    void markDirty();

  private:
    QSGNode* updatePaintNode( QSGNode* n, QQuickItem::UpdatePaintNodeData* );

    RubberbandModel* mModel;
    MapSettings* mMapSettings;
    bool mDirty;
};

#endif // RUBBERBAND_H
