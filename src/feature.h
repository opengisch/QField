/***************************************************************************
                            feature.h
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias.kuhn (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSEXTENDEDFEATURE_H
#define QGSEXTENDEDFEATURE_H

#include <qgsfeature.h>
#include <qgsvectorlayer.h>

class Feature
{
  public:
    Feature( const QgsFeature& feature, QgsVectorLayer* layer );
    Feature();

    const QgsAttributes attributes() const
    {
      return mFeature.attributes();
    }

    const QgsFields* fields() const
    {
      return mFeature.fields();
    }

    const QVariant attribute( int index ) const
    {
      return mFeature.attribute( index );
    }

    QgsVectorLayer* layer() const
    {
      return mLayer;
    }

    QgsFeatureId id() const
    {
      return mFeature.id();
    }

    const QgsFeature& qgsFeature() const
    {
      return mFeature;
    }

    const QString displayText() const;

  private:
    // TODO: Use implicity sharing for this
    QgsFeature mFeature;
    QgsVectorLayer* mLayer;
};

Q_DECLARE_METATYPE( Feature )

#endif // QGSEXTENDEDFEATURE_H
