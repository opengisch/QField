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

    Q_PROPERTY( int delay READ delay WRITE setDelay NOTIFY delayChanged )
    Q_PROPERTY( QString name READ name WRITE setName NOTIFY nameChanged )
    Q_PROPERTY( QString displayName READ displayName WRITE setDisplayName NOTIFY displayNameChanged )
    Q_PROPERTY( QString prefix READ prefix WRITE setPrefix NOTIFY prefixChanged )

    Q_PROPERTY( QVariantMap parameters READ parameters WRITE setParameters NOTIFY parametersChanged )
    Q_PROPERTY( QUrl source READ source WRITE setSource NOTIFY sourceChanged )

    Q_PROPERTY( LocatorModelSuperBridge *locatorBridge READ locatorBridge WRITE setLocatorBridge NOTIFY locatorBridgeChanged )

  public:
    explicit QFieldLocatorFilter( QObject *parent = nullptr );

    //! Clone the locator filter
    QFieldLocatorFilter *clone() const override;

    //! Returns the delay before which the fetching of results is triggered
    int delay() const { return fetchResultsDelay(); }

    //! Sets the delay before which the fetching of results is triggered
    void setDelay( int delay );

    //! \copydoc QgsLocatorFilter::name
    QString name() const override { return mName; }

    /**
     * Sets the unique name of the filter. This should be an untranslated string identifying the filter.
     */
    void setName( const QString &name );

    //! \copydoc QgsLocatorFilter::displayName
    QString displayName() const override { return mDisplayName; }

    /**
     * Sets a translated, user-friendly name for the filter.
     */
    void setDisplayName( const QString &displayName );

    //! \copydoc QgsLocatorFilter::prefix
    QString prefix() const override { return mPrefix; }

    /**
     * Sets the search prefix character(s) for this filter. Prefix a search
     * with these characters will restrict the locator search to only include
     * results from this filter.
     * \note The prefix must be >= 3 characters otherwise it will be ignored.
     */
    void setPrefix( const QString &prefix );

    /**
     * Returns the source QML file which will process the locator filter results.
     */
    QUrl source() const { return mSource; }

    /**
     * Setsthe source QML file which will process the locator filter results.
     */
    void setSource( const QUrl &source );

    /**
     * Returns additional locator filter parameters which will possed onto the source QML
     * that will process the locator filter results.
     */
    QVariantMap parameters() const { return mParameters; }

    /**
     * Sets additional locator filter parameters which will possed onto the source QML
     * that will process the locator filter results.
     */
    void setParameters( const QVariantMap &parameters );

    /**
     * Returns the locator bridge object.
     */
    LocatorModelSuperBridge *locatorBridge() const { return mLocatorBridge; }

    /**
     * Sets the locator bridge object.
     */
    void setLocatorBridge( LocatorModelSuperBridge *locatorBridge );

    Priority priority() const override { return Medium; }
    void fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback ) override;
    void triggerResult( const QgsLocatorResult &result ) override;
    void triggerResultFromAction( const QgsLocatorResult &result, const int actionId ) override;

  signals:
    //! Emitted when the fetch result delay has changed
    void delayChanged();

    //! Emitted when the name has changed
    void nameChanged();

    //! Emitted when the display name has changed
    void displayNameChanged();

    //! Emitted when the prefix has changed
    void prefixChanged();

    //! Emitted when the parameters object has changed
    void parametersChanged();

    //! Emitted when the source has changed
    void sourceChanged();

    //! Emitted when the locator bridge has changed
    void locatorBridgeChanged();

  private slots:
    void fetchResultsEnded();
    void prepareResult( const QVariant &details );

  private:
    QString mName;
    QString mDisplayName;
    QString mPrefix;

    bool mFetchResultsEnded = false;

    QVariantMap mParameters;
    QUrl mSource;

    LocatorModelSuperBridge *mLocatorBridge = nullptr;
};

#endif // QFIELDLOCATORFILTER_H
