/***************************************************************************
                            expressioncontextutils.h
                              -------------------
              begin                : 5.12.2017
              copyright            : (C) 2017 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef EXPRESSIONCONTEXTUTILS_H
#define EXPRESSIONCONTEXTUTILS_H

#include <qgsexpressioncontext.h>
#include <qgsexpression.h>
#include "snappingresult.h"
#include <QObject>

class QGeoPositionInfoSource;

class ExpressionContextUtils
{
  public:
    static QgsExpressionContextScope *positionScope( QGeoPositionInfoSource *source );
    static QgsExpressionContextScope *mapToolCaptureScope( const SnappingResult &topSnappingResult );

  private:
    ExpressionContextUtils() = default;
};

class ExpressionUtils : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString expressionText READ expressionText WRITE setExpressionText NOTIFY expressionTextChanged )
    Q_PROPERTY( QgsFeature feature READ feature WRITE setFeature NOTIFY featureChanged )
    Q_PROPERTY( QgsMapLayer *layer READ layer WRITE setLayer NOTIFY layerChanged )

  public:
    explicit ExpressionUtils( QObject *parent = nullptr );

    QString expressionText() { return mExpressionText; }
    QgsFeature feature() { return mFeature; }
    QgsMapLayer *layer() { return mLayer; }

    void setExpressionText( QString expressionText ) { mExpressionText = expressionText; }
    void setFeature( QgsFeature feature ) { mFeature = feature; }
    void setLayer( QgsMapLayer *layer ) { mLayer = layer; }

    Q_INVOKABLE QString evaluate();

  signals:
    void layerChanged( QgsMapLayer *layer );
    void expressionTextChanged( QString expressionText );
    void featureChanged( QgsFeature feature );

  private:
    QString mExpressionText;
    QgsFeature mFeature;
    QgsMapLayer *mLayer = nullptr;

};

#endif // EXPRESSIONCONTEXTUTILS_H
