/***************************************************************************
                        urlutils.cpp
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

#include "urlutils.h"

#include <QUrl>


UrlUtils::UrlUtils( QObject *parent )
  : QObject( parent )
{
}


bool UrlUtils::isRelativeOrFileUrl( const QString &url )
{
  if ( url.startsWith( QStringLiteral( "file:///" ) ) )
    return true;

  return QUrl( url ).isRelative();
}
