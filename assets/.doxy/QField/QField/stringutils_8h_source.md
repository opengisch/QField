

# File stringutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**stringutils.h**](stringutils_8h.md)

[Go to the documentation of this file](stringutils_8h.md)


```C++
/***************************************************************************
                        stringutils.h
                        ---------------
  begin                : Jun 2020
  copyright            : (C) 2020 by Ivan Ivanov
  email                : ivan@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include "qfield_core_export.h"

#include <QColor>
#include <QObject>
#include <qgscoordinatereferencesystem.h>
#include <qgspoint.h>

class QFIELD_CORE_EXPORT StringUtils : public QObject
{
    Q_OBJECT

  public:
    explicit StringUtils( QObject *parent = nullptr );

    static Q_INVOKABLE QString insertLinks( const QString &string );

    static Q_INVOKABLE bool hasLinks( const QString &string );

    static Q_INVOKABLE QString createUuid();

    static bool fuzzyMatch( const QString &source, const QString &term );

    static double calcFuzzyScore( const QString &string, const QString &searchTerm );

    static Q_INVOKABLE QString highlightText( const QString &string, const QString &highlightText, const QColor &highlightColor = QColor() );

    static Q_INVOKABLE QString pointInformation( const QgsPoint &point, const QgsCoordinateReferenceSystem &crs );

    static const QString stringListToCsv( QStringList list );

    static const QStringList csvToStringList( const QString &string );

    static Q_INVOKABLE QString replaceFilenameTags( const QString &string, const QString &filename );
};

#endif // STRINGUTILS_H
```


