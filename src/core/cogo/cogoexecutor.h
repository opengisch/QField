/***************************************************************************
                              cogoexecutor.h
                              -------------------
              begin                : 2025.12.21
              copyright            : (C) 2025 by Mathieu Pellerin
              email                : mathieu (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef COGOEXECUTOR_H
#define COGOEXECUTOR_H

#include "cogooperation.h"
#include "qgsquickmapsettings.h"
#include "rubberbandmodel.h"

#include <QMap>
#include <QObject>
#include <QVariant>

/**
 * \brief A COGO operation exeuctor object allowing for property-driven selectio of COGO operation and parameters.
 * \ingroup core
 */
class CogoExecutor : public QObject
{
    Q_OBJECT

    /**
     * The name of the COGO operation to be executed.
     */
    Q_PROPERTY( QString name READ name WRITE setName NOTIFY nameChanged )

    /**
     * The list of parameters of the COGO operation to be executed.
     */
    Q_PROPERTY( QList<CogoParameter> parameters READ parameters NOTIFY parametersChanged )

    /**
     * The map of parameter values to be used when executing the COGO operation.
     */
    Q_PROPERTY( QVariantMap parameterValues READ parameterValues WRITE setParameterValues NOTIFY parameterValuesChanged )

    /**
     * The visual guides returned by the COGO operation paired with the parameters.
     */
    Q_PROPERTY( QList<CogoVisualGuide> visualGuides READ visualGuides NOTIFY visualGuidesChanged )

    /**
     * COGO operation paired with the parameters' readiness to be executed.
     */
    Q_PROPERTY( bool isReady READ isReady NOTIFY isReadyChanged )

    /**
     * The rubberband model to be used when executing the COGO operation.
     */
    Q_PROPERTY( RubberbandModel *rubberbandModel READ rubberbandModel WRITE setRubberbandModel NOTIFY rubberbandModelChanged )

    /**
     * The map settings used when generating visual guides.
     */
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

  public:
    explicit CogoExecutor( QObject *parent = nullptr );

    //! \copydoc CogoExecutor::name
    QString name() const { return mName; }
    //! \copydoc CogoExecutor::name
    void setName( const QString &name );

    //! \copydoc CogoExecutor::parameters
    QList<CogoParameter> parameters() const { return mParameters; }

    //! \copydoc CogoExecutor::parameterValues
    QVariantMap parameterValues() const { return mParameterValues; }
    //! \copydoc CogoExecutor::parameterValues
    void setParameterValues( const QVariantMap &parameterValues );

    //! \copydoc CogoExecutor::visualGuides
    QList<CogoVisualGuide> visualGuides() const;

    //! \copydoc CogoExecutor::isReady
    bool isReady() const { return mIsReady; }

    //! \copydoc CogoExecutor::mapSettings
    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }
    //! \copydoc CogoExecutor::mapSettings
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    //! \copydoc CogoExecutor::rubberbandModel
    RubberbandModel *rubberbandModel() const { return mRubberbandModel; }
    //! \copydoc CogoExecutor::rubberbandModel
    void setRubberbandModel( RubberbandModel *rubberbandModel );

    /**
     * Executes the COGO operation.
     */
    Q_INVOKABLE bool execute();

  signals:
    //! \copydoc CogoExecutor::name
    void nameChanged();
    //! \copydoc CogoExecutor::parameters
    void parametersChanged();
    //! \copydoc CogoExecutor::parameterValuess
    void parameterValuesChanged();
    //! \copydoc CogoExecutor::visualGuides
    void visualGuidesChanged();
    //! \copydoc CogoExecutor::isReady
    void isReadyChanged();
    //! \copydoc CogoExecutor::mapSettings
    void mapSettingsChanged();
    //! \copydoc CogoExecutor::rubberbandModel
    void rubberbandModelChanged();

  private:
    void getParameters();
    void generateVisualGuides();
    void checkReadiness();

    QString mName;
    QList<CogoParameter> mParameters;

    QVariantMap mParameterValues;
    QList<CogoVisualGuide> mVisualGuides;
    bool mIsReady = false;

    QgsQuickMapSettings *mMapSettings = nullptr;
    RubberbandModel *mRubberbandModel = nullptr;
};

Q_DECLARE_METATYPE( CogoExecutor )
#endif // COGOEXECUTOR_H
