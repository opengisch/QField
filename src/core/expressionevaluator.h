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


/**
 * @brief The ExpressionEvaluator class enables evaluation of expression
 * strings and expression templates.
 * \ingroup core
 */
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
    //! Expression evaluator modes
    enum Mode
    {
      ExpressionMode,         //!< Handle expression text as expression strings
      ExpressionTemplateMode, //!< Handle expression text as expression templates
    };
    Q_ENUM( Mode )

    explicit ExpressionEvaluator( QObject *parent = nullptr );

    //! Returns the expression evaluator mode
    Mode mode() const { return mMode; }

    //! Sets the expression evaluator \a mode
    void setMode( Mode mode );

    //! Returns the expression text used when evaluating
    QString expressionText() { return mExpressionText; }

    //! Sets the expression text used when evaluating
    void setExpressionText( const QString &expressionText );

    //! Returns the feature attached to the expression context
    QgsFeature feature() const { return mFeature; }

    //! Sets the feature attached to the expression context
    void setFeature( const QgsFeature &feature );

    //! Returns the map layer attached to the expression context
    QgsMapLayer *layer() const { return mLayer; }

    //! Sets the map layer attached to the expression context
    void setLayer( QgsMapLayer *layer );

    //! Returns the project attached to the expression context
    QgsProject *project() const { return mProject; }

    //! Sets the project attached to the expression context
    void setProject( QgsProject *project );

    //! Returns the map settings attached to the expression context
    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }

    //! Sets the map settings attached to the expression context
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    //! Returns the position information attached to the expression context
    GnssPositionInformation positionInformation() const { return mPositionInformation; }

    //! Sets the position information attached to the expression context
    void setPositionInformation( const GnssPositionInformation &positionInformation );

    //! Returns the cloud user information attached to the expression context
    CloudUserInformation cloudUserInformation() const { return mCloudUserInformation; }

    //! Sets the cloud user information attached to the expression context
    void setCloudUserInformation( const CloudUserInformation &cloudUserInformation );

    //! Returns the evaluated expression text value
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
