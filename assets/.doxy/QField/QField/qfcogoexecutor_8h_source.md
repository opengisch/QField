

# File qfcogoexecutor.h

[**File List**](files.md) **>** [**cogo**](dir_bee6354f604373d3fe8d8d947aaacc65.md) **>** [**qfcogoexecutor.h**](qfcogoexecutor_8h.md)

[Go to the documentation of this file](qfcogoexecutor_8h.md)


```C++
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

class QfCogoExecutor : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString name READ name WRITE setName NOTIFY nameChanged )

    
    Q_PROPERTY( QList<QfCogoParameter> parameters READ parameters NOTIFY parametersChanged )

    Q_PROPERTY( QVariantMap parameterValues READ parameterValues WRITE setParameterValues NOTIFY parameterValuesChanged )

    Q_PROPERTY( QList<QfCogoVisualGuide> visualGuides READ visualGuides NOTIFY visualGuidesChanged )

    Q_PROPERTY( bool isReady READ isReady NOTIFY isReadyChanged )

    Q_PROPERTY( QfRubberbandModel *rubberbandModel READ rubberbandModel WRITE setRubberbandModel NOTIFY rubberbandModelChanged )

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

  public:
    explicit QfCogoExecutor( QObject *parent = nullptr );

    QString name() const { return mName; }
    void setName( const QString &name );

    QList<QfCogoParameter> parameters() const { return mParameters; }

    QVariantMap parameterValues() const { return mParameterValues; }
    void setParameterValues( const QVariantMap &parameterValues );

    QList<QfCogoVisualGuide> visualGuides() const;

    bool isReady() const { return mIsReady; }

    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    QfRubberbandModel *rubberbandModel() const { return mRubberbandModel; }
    void setRubberbandModel( QfRubberbandModel *rubberbandModel );

    Q_INVOKABLE bool execute();

  signals:
    void nameChanged();
    void parametersChanged();
    void parameterValuesChanged();
    void visualGuidesChanged();
    void isReadyChanged();
    void mapSettingsChanged();
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
```


