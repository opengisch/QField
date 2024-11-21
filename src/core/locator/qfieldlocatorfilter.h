/***************************************************************************
  qfieldlocatorfilter.h

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


#ifndef QFIELDLOCATORFILTER_H
#define QFIELDLOCATORFILTER_H

#include "locatormodelsuperbridge.h"

#include <QObject>
#include <QQmlComponent>
#include <qgslocatorfilter.h>


class LocatorModelSuperBridge;

/**
 * QFieldLocatorFilter is a locator filter item for QField plugins to integrate with
 * locator searches.
 */
class QFieldLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

    Q_PROPERTY( QString name READ name WRITE setName NOTIFY nameChanged )
    Q_PROPERTY( QString displayName READ displayName WRITE setDisplayName NOTIFY displayNameChanged )
    Q_PROPERTY( QString prefix READ prefix WRITE setPrefix NOTIFY prefixChanged )
    Q_PROPERTY( QUrl source READ source WRITE setSource NOTIFY sourceChanged )
    Q_PROPERTY( LocatorModelSuperBridge *locatorBridge READ locatorBridge WRITE setLocatorBridge NOTIFY locatorBridgeChanged )

  public:
    explicit QFieldLocatorFilter( QObject *parent = nullptr );
    QFieldLocatorFilter *clone() const override;

    QString name() const override { return mName; }
    void setName( const QString &name );

    QString displayName() const override { return mDisplayName; }
    void setDisplayName( const QString &displayName );

    QString prefix() const override { return mPrefix; }
    void setPrefix( const QString &prefix );

    QUrl source() const { return mSource; }
    void setSource( const QUrl &source );

    LocatorModelSuperBridge *locatorBridge() const { return mLocatorBridge; }
    void setLocatorBridge( LocatorModelSuperBridge *locatorBridge );

    Priority priority() const override { return Medium; }

    void fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback ) override;
    void triggerResult( const QgsLocatorResult &result ) override;
    void triggerResultFromAction( const QgsLocatorResult &result, const int actionId ) override;

  signals:
    void nameChanged();
    void displayNameChanged();
    void prefixChanged();
    void sourceChanged();
    void locatorBridgeChanged();

  private slots:
    void fetchResultsEnded();

  private:
    QString mName;
    QString mDisplayName;
    QString mPrefix;

    bool mFetchResultsEnded = false;

    QUrl mSource;

    LocatorModelSuperBridge *mLocatorBridge = nullptr;
};

#endif // QFIELDLOCATORFILTER_H
