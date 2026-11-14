

# File qfhelplocatorfilter.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**qfhelplocatorfilter.h**](qfhelplocatorfilter_8h.md)

[Go to the documentation of this file](qfhelplocatorfilter_8h.md)


```C++
/***************************************************************************
  qfhelplocatorfilter.h

 ---------------------
 begin                : 02.07.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef QFHELPLOCATORFILTER_H
#define QFHELPLOCATORFILTER_H

#include <QObject>
#include <qgslocatorfilter.h>


class QfLocatorModelSuperBridge;

class QfHelpLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

  public:
    enum ActionOrigin
    {
      Normal,
    };

    explicit QfHelpLocatorFilter( QfLocatorModelSuperBridge *locatorBridge, QObject *parent = nullptr );
    QfHelpLocatorFilter *clone() const override;
    QString name() const override { return QStringLiteral( "optionpages" ); }
    QString displayName() const override { return tr( "QField Documentation" ); }
    QString description() const override { return tr( "Returns QField documentation pages matching terms." ); }
    Priority priority() const override { return Medium; }
    QString prefix() const override { return QStringLiteral( "?" ); }

    QString searchUrl() const { return mSearchUrl; }
    void setSearchUrl( const QString &url ) { mSearchUrl = url; }

    void fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback ) override;
    void triggerResult( const QgsLocatorResult &result ) override;
    void triggerResultFromAction( const QgsLocatorResult &result, const int actionId ) override;

  private:
    QfLocatorModelSuperBridge *mLocatorBridge = nullptr;
    QString mSearchUrl = QStringLiteral( "https://docs.qfield.org/search/search_index.json" );
};

#endif // QFHELPLOCATORFILTER_H
```


