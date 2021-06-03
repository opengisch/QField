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


class QFIELD_CORE_EXPORT UrlUtils : public QObject
{
    Q_OBJECT

  public:
    explicit UrlUtils( QObject *parent = nullptr );


    /**
     * Checks whether the provided string is a relative url (has no protocol or starts with `file://`).
     */
    static Q_INVOKABLE bool isRelativeOrFileUrl( const QString &url );
};

#endif // URLUTILS_H
