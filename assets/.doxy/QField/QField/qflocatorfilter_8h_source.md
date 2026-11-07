

# File qflocatorfilter.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**qflocatorfilter.h**](qflocatorfilter_8h.md)

[Go to the documentation of this file](qflocatorfilter_8h.md)


```C++
/***************************************************************************
  qflocatorfilter.h

 ---------------------
  Date                 : November 2024
  Copyright            : (C) 2024 by Mathieu Pellerin
  Email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef QFLOCATORFILTER_H
#define QFLOCATORFILTER_H

#include "qflocatormodelsuperbridge.h"

#include <QObject>
#include <QQmlComponent>
#include <qgslocatorfilter.h>


class QfLocatorModelSuperBridge;

class QfLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

    Q_PROPERTY( int delay READ delay WRITE setDelay NOTIFY delayChanged )
    Q_PROPERTY( QString name READ name WRITE setName NOTIFY nameChanged )
    Q_PROPERTY( QString displayName READ displayName WRITE setDisplayName NOTIFY displayNameChanged )
    Q_PROPERTY( QString prefix READ prefix WRITE setPrefix NOTIFY prefixChanged )
    Q_PROPERTY( QString description READ description WRITE setDescription NOTIFY descriptionChanged )

    Q_PROPERTY( QVariantMap parameters READ parameters WRITE setParameters NOTIFY parametersChanged )
    Q_PROPERTY( QUrl source READ source WRITE setSource NOTIFY sourceChanged )

    Q_PROPERTY( QfLocatorModelSuperBridge *locatorBridge READ locatorBridge WRITE setLocatorBridge NOTIFY locatorBridgeChanged )

  public:
    explicit QfLocatorFilter( QObject *parent = nullptr );

    QfLocatorFilter *clone() const override;

    int delay() const { return fetchResultsDelay(); }

    void setDelay( int delay );

    QString name() const override { return mName; }

    void setName( const QString &name );

    QString displayName() const override { return mDisplayName; }

    void setDisplayName( const QString &displayName );

    QString prefix() const override { return mPrefix; }

    void setPrefix( const QString &prefix );

    QString description() const override { return mDescription; }

    void setDescription( const QString &description );

    QUrl source() const { return mSource; }

    void setSource( const QUrl &source );

    QVariantMap parameters() const { return mParameters; }

    void setParameters( const QVariantMap &parameters );

    QfLocatorModelSuperBridge *locatorBridge() const { return mLocatorBridge; }

    void setLocatorBridge( QfLocatorModelSuperBridge *locatorBridge );

    Priority priority() const override { return Medium; }
    void fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback ) override;
    void triggerResult( const QgsLocatorResult &result ) override;
    void triggerResultFromAction( const QgsLocatorResult &result, const int actionId ) override;

  signals:
    void delayChanged();

    void nameChanged();

    void displayNameChanged();

    void prefixChanged();

    void descriptionChanged();

    void parametersChanged();

    void sourceChanged();

    void locatorBridgeChanged();

  private slots:
    void fetchResultsEnded();
    void prepareResult( const QVariant &details );

  private:
    QString mName;
    QString mDisplayName;
    QString mPrefix;
    QString mDescription;

    bool mFetchResultsEnded = false;

    QVariantMap mParameters;
    QUrl mSource;

    QfLocatorModelSuperBridge *mLocatorBridge = nullptr;
};

#endif // QFLOCATORFILTER_H
```


