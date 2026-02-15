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

/**
 * \ingroup core
 */
class QFIELD_CORE_EXPORT StringUtils : public QObject
{
    Q_OBJECT

  public:
    explicit StringUtils( QObject *parent = nullptr );

    //! Returns a string with any URL (e.g., http(s)/ftp) and mailto: text converted to valid HTML <a …> links
    static Q_INVOKABLE QString insertLinks( const QString &string );

    //! Returns whether a string contains one or more URLs
    static Q_INVOKABLE bool hasLinks( const QString &string );

    //! Returns a new UUID string
    static Q_INVOKABLE QString createUuid();

    //! Checks whether the string \a term is part of \a source
    static bool fuzzyMatch( const QString &source, const QString &term );

    //! Calculates a fuzzy matching score between a \a string and a \a searchTerm.
    static double calcFuzzyScore( const QString &string, const QString &searchTerm );

    //! Returns a string highlighting a text using HTML formatting
    static Q_INVOKABLE QString highlightText( const QString &string, const QString &highlightText, const QColor &highlightColor = QColor() );

    //! Returns a string containing the \a point location and details of the \a crs
    static Q_INVOKABLE QString pointInformation( const QgsPoint &point, const QgsCoordinateReferenceSystem &crs );

    //! Returns a CSV formatted string from a \a list
    static const QString stringListToCsv( QStringList list );

    //! Returns a list from a CSV formatted \a string
    static const QStringList csvToStringList( const QString &string );

    /**
     * Returns a string in which QField-specific filename tags have been replaced with their
     * values.
     */
    static Q_INVOKABLE QString replaceFilenameTags( const QString &string, const QString &filename );
};

#endif // STRINGUTILS_H
