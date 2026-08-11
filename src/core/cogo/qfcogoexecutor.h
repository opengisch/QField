/***************************************************************************
                              qfcogoexecutor.h
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

#ifndef QFCOGOEXECUTOR_H
#define QFCOGOEXECUTOR_H

#include "qfcogooperation.h"
#include "qfrubberbandmodel.h"
#include "qgsquickmapsettings.h"

#include <QMap>
#include <QObject>
#include <QVariant>

/**
 * \brief A COGO operation exeuctor object allowing for property-driven access to COGO operations.
 * \ingroup core
 */
class QfCogoExecutor : public QObject
{
    Q_OBJECT

    /**
     * The name of the COGO operation to be executed.
     */
    Q_PROPERTY( QString name READ name WRITE setName NOTIFY nameChanged )

    /**
     * The list of parameters of the COGO operation to be executed.
     */
    Q_PROPERTY( QList<QfCogoParameter> parameters READ parameters NOTIFY parametersChanged )

    /**
     * The map of parameter values to be used when executing the COGO operation.
     */
    Q_PROPERTY( QVariantMap parameterValues READ parameterValues WRITE setParameterValues NOTIFY parameterValuesChanged )

    /**
     * The visual guides returned by the COGO operation paired with the parameters.
     */
    Q_PROPERTY( QList<QfCogoVisualGuide> visualGuides READ visualGuides NOTIFY visualGuidesChanged )

    /**
     * COGO operation paired with the parameters' readiness to be executed.
     */
    Q_PROPERTY( bool isReady READ isReady NOTIFY isReadyChanged )

    /**
     * The rubberband model to be used when executing the COGO operation.
     */
    Q_PROPERTY( QfRubberbandModel *rubberbandModel READ rubberbandModel WRITE setRubberbandModel NOTIFY rubberbandModelChanged )

    /**
     * The map settings used when generating visual guides.
     */
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

  public:
    explicit QfCogoExecutor( QObject *parent = nullptr );

    //! \copydoc QfCogoExecutor::name
    QString name() const { return mName; }
    //! \copydoc QfCogoExecutor::name
    void setName( const QString &name );

    //! \copydoc QfCogoExecutor::parameters
    QList<QfCogoParameter> parameters() const { return mParameters; }

    //! \copydoc QfCogoExecutor::parameterValues
    QVariantMap parameterValues() const { return mParameterValues; }
    //! \copydoc QfCogoExecutor::parameterValues
    void setParameterValues( const QVariantMap &parameterValues );

    //! \copydoc QfCogoExecutor::visualGuides
    QList<QfCogoVisualGuide> visualGuides() const;

    //! \copydoc QfCogoExecutor::isReady
    bool isReady() const { return mIsReady; }

    //! \copydoc QfCogoExecutor::mapSettings
    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }
    //! \copydoc QfCogoExecutor::mapSettings
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    //! \copydoc QfCogoExecutor::rubberbandModel
    QfRubberbandModel *rubberbandModel() const { return mRubberbandModel; }
    //! \copydoc QfCogoExecutor::rubberbandModel
    void setRubberbandModel( QfRubberbandModel *rubberbandModel );

    /**
     * Executes the COGO operation.
     */
    Q_INVOKABLE bool execute();

  signals:
    //! \copydoc QfCogoExecutor::name
    void nameChanged();
    //! \copydoc QfCogoExecutor::parameters
    void parametersChanged();
    //! \copydoc QfCogoExecutor::parameterValuess
    void parameterValuesChanged();
    //! \copydoc QfCogoExecutor::visualGuides
    void visualGuidesChanged();
    //! \copydoc QfCogoExecutor::isReady
    void isReadyChanged();
    //! \copydoc QfCogoExecutor::mapSettings
    void mapSettingsChanged();
    //! \copydoc QfCogoExecutor::rubberbandModel
    void rubberbandModelChanged();

  private:
    void getParameters();
    void generateVisualGuides();
    void checkReadiness();

    QString mName;
    QList<QfCogoParameter> mParameters;

    QVariantMap mParameterValues;
    QList<QfCogoVisualGuide> mVisualGuides;
    bool mIsReady = false;

    QgsQuickMapSettings *mMapSettings = nullptr;
    QfRubberbandModel *mRubberbandModel = nullptr;
};

Q_DECLARE_METATYPE( QfCogoExecutor )
#endif // QFCOGOEXECUTOR_H
