/***************************************************************************
                        urlutils.h
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

#ifndef URLUTILS_H
#define URLUTILS_H

#include "qfield_core_export.h"

#include <QObject>

/**
 * \ingroup core
 */
class QFIELD_CORE_EXPORT UrlUtils : public QObject
{
    Q_OBJECT

  public:
    explicit UrlUtils( QObject *parent = nullptr );

    /**
     * Checks whether the provided string is a relative \a url (has no protocol or starts with `file://`).
     */
    static Q_INVOKABLE bool isRelativeOrFileUrl( const QString &url );

    //! Returns a URL from a \a url with logic to handle local paths
    static Q_INVOKABLE QUrl fromString( const QString &url );

    /**
     * Returns a \a detail from an \a url. The possible components are:
     * - "scheme", e.g. https
     * - "authority", e.g. qfield.org
     * - "path", e.g. /my/home.html
     * - "fileName", e.g. file.zip
     * - "query", e.g. param=true&other_parem=0
     */
    static Q_INVOKABLE QString urlDetail( const QString &url, const QString &detail );
};

#endif // URLUTILS_H
