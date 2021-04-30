/***************************************************************************
 expressionevaluator.h - ExpressionEvaluator
                              -------------------
 begin                : January 2020
 copyright            : (C) 2020 by David Signer
 email                : david (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EXPRESSIONEVALUATOR_H
#define EXPRESSIONEVALUATOR_H

#include <QObject>
#include <qgsexpression.h>
#include <qgsexpressioncontext.h>

class ExpressionEvaluator : public QObject
{
  Q_OBJECT

  Q_PROPERTY( QString expressionText READ expressionText WRITE setExpressionText NOTIFY expressionTextChanged )
  Q_PROPERTY( QgsFeature feature READ feature WRITE setFeature NOTIFY featureChanged )
  Q_PROPERTY( QgsMapLayer *layer READ layer WRITE setLayer NOTIFY layerChanged )

public:
  explicit ExpressionEvaluator( QObject *parent = nullptr );

  QString expressionText() { return mExpressionText; }
  QgsFeature feature() { return mFeature; }
  QgsMapLayer *layer() { return mLayer; }

  void setExpressionText( const QString &expressionText );
  void setFeature( const QgsFeature &feature );
  void setLayer( QgsMapLayer *layer );

  //! Returns the evaluated string value
  Q_INVOKABLE QVariant evaluate();

signals:
  void layerChanged( QgsMapLayer *layer );
  void expressionTextChanged( QString expressionText );
  void featureChanged( QgsFeature feature );

private:
  QString mExpressionText;
  QgsFeature mFeature;
  QgsMapLayer *mLayer = nullptr;
};
#endif // EXPRESSIONEVALUATOR_H
