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

#include <QObject>


class StringUtils : public QObject
{
    Q_OBJECT

  public:


    explicit StringUtils( QObject *parent = nullptr );


    /**
     * Returns a string with any URL (e.g., http(s)/ftp) and mailto: text converted to valid HTML <a …> links.
     */
    static Q_INVOKABLE QString insertLinks( const QString &string );

    //! Checks whether the string \a term is part of \a source
    static bool fuzzyMatch( const QString &source, const QString &term );
};

#endif // STRINGUTILS_H

