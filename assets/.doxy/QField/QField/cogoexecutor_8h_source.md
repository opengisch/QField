

# File cogoexecutor.h

[**File List**](files.md) **>** [**cogo**](dir_bee6354f604373d3fe8d8d947aaacc65.md) **>** [**cogoexecutor.h**](cogoexecutor_8h.md)

[Go to the documentation of this file](cogoexecutor_8h.md)


```C++
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

class CogoExecutor : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString name READ name WRITE setName NOTIFY nameChanged )

    
    Q_PROPERTY( QList<CogoParameter> parameters READ parameters NOTIFY parametersChanged )

    Q_PROPERTY( QVariantMap parameterValues READ parameterValues WRITE setParameterValues NOTIFY parameterValuesChanged )

    Q_PROPERTY( QList<CogoVisualGuide> visualGuides READ visualGuides NOTIFY visualGuidesChanged )

    Q_PROPERTY( bool isReady READ isReady NOTIFY isReadyChanged )

    Q_PROPERTY( RubberbandModel *rubberbandModel READ rubberbandModel WRITE setRubberbandModel NOTIFY rubberbandModelChanged )

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

  public:
    explicit CogoExecutor( QObject *parent = nullptr );

    QString name() const { return mName; }
    void setName( const QString &name );

    QList<CogoParameter> parameters() const { return mParameters; }

    QVariantMap parameterValues() const { return mParameterValues; }
    void setParameterValues( const QVariantMap &parameterValues );

    QList<CogoVisualGuide> visualGuides() const;

    bool isReady() const { return mIsReady; }

    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    RubberbandModel *rubberbandModel() const { return mRubberbandModel; }
    void setRubberbandModel( RubberbandModel *rubberbandModel );

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
    QList<CogoParameter> mParameters;

    QVariantMap mParameterValues;
    QList<CogoVisualGuide> mVisualGuides;
    bool mIsReady = false;

    QgsQuickMapSettings *mMapSettings = nullptr;
    RubberbandModel *mRubberbandModel = nullptr;
};

Q_DECLARE_METATYPE( CogoExecutor )
#endif // COGOEXECUTOR_H
```


