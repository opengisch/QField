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

    //! Returns a URL from a \a url with logic to presence or absence of file://
    static Q_INVOKABLE QString toLocalFile( const QString &url );

    /**
     * Returns a \a detail from an \a url. The possible components are:
     * - "scheme", e.g. https
     * - "authority", e.g. qfield.org
     * - "path", e.g. /my/home.html
     * - "fileName", e.g. file.zip
     * - "query", e.g. param=true&other_parem=0
     */
    static Q_INVOKABLE QString urlDetail( const QString &url, const QString &detail );

    /**
     * Returns QField action details extracted from a compatible \a url.
     */
    static Q_INVOKABLE QVariantMap getActionDetails( const QString &url );

    /**
     * Returns a QField action url string.
     * \param scheme the action scheme, a qfield value will return a "qfield://" scheme an https value
     * will return an "https://qfield.org" scheme and domain name
     * \param type the action type (local or cloud)
     * \param details the action details transformed into URL query parameters
     */
    static Q_INVOKABLE QString createActionUrl( const QString &scheme, const QString &type, const QVariantMap &details );

    /**
     * Returns an encoded URL built using parameters' pair of key and value as
     * query items.
     */
    static Q_INVOKABLE QString createEncodedUrl( const QVariantMap &parameters );
};

#endif // URLUTILS_H
