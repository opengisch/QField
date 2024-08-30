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

#include "gnsspositioninformation.h"
#include "qfieldcloudutils.h"
#include "qgsquickmapsettings.h"

#include <QObject>
#include <qgsexpression.h>
#include <qgsexpressioncontext.h>
#include <qgsmaplayer.h>
#include <qgsproject.h>

class ExpressionEvaluator : public QObject
{
    Q_OBJECT

    Q_PROPERTY( Mode mode READ mode WRITE setMode NOTIFY modeChanged )

    Q_PROPERTY( QString expressionText READ expressionText WRITE setExpressionText NOTIFY expressionTextChanged )

    Q_PROPERTY( QgsFeature feature READ feature WRITE setFeature NOTIFY featureChanged )
    Q_PROPERTY( QgsMapLayer *layer READ layer WRITE setLayer NOTIFY layerChanged )
    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )
    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( GnssPositionInformation positionInformation READ positionInformation WRITE setPositionInformation NOTIFY positionInformationChanged )
    Q_PROPERTY( CloudUserInformation cloudUserInformation READ cloudUserInformation WRITE setCloudUserInformation NOTIFY cloudUserInformationChanged )

  public:
    enum Mode
    {
      ExpressionMode,
      ExpressionTemplateMode
    };
    Q_ENUM( Mode )

    explicit ExpressionEvaluator( QObject *parent = nullptr );

    Mode mode() const { return mMode; }
    void setMode( Mode mode );

    QString expressionText() { return mExpressionText; }
    void setExpressionText( const QString &expressionText );

    QgsFeature feature() const { return mFeature; }
    void setFeature( const QgsFeature &feature );

    QgsMapLayer *layer() const { return mLayer; }
    void setLayer( QgsMapLayer *layer );

    QgsProject *project() const { return mProject; }
    void setProject( QgsProject *project );

    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    GnssPositionInformation positionInformation() const { return mPositionInformation; }
    void setPositionInformation( const GnssPositionInformation &positionInformation );

    CloudUserInformation cloudUserInformation() const { return mCloudUserInformation; }
    void setCloudUserInformation( const CloudUserInformation &cloudUserInformation );

    //! Returns the evaluated string value
    Q_INVOKABLE QVariant evaluate();

  signals:
    void modeChanged();
    void expressionTextChanged();
    void featureChanged();
    void layerChanged();
    void projectChanged();
    void mapSettingsChanged();
    void positionInformationChanged();
    void cloudUserInformationChanged();

  private:
    Mode mMode = ExpressionMode;

    QString mExpressionText;

    QgsFeature mFeature;
    QgsMapLayer *mLayer = nullptr;
    QgsProject *mProject = nullptr;
    QgsQuickMapSettings *mMapSettings = nullptr;
    GnssPositionInformation mPositionInformation;
    CloudUserInformation mCloudUserInformation;
};
#endif // EXPRESSIONEVALUATOR_H
