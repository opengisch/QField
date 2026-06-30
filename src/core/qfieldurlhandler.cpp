/***************************************************************************
              qfieldurlhandler.cpp
              -------------------
              begin                : April 2025
              copyright            : (C) 2025 by Mathieu Pellerin
              email                : mathieu (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "appinterface.h"
#include "platformutilities.h"
#include "qfieldurlhandler.h"

#include <QTimer>

QFieldUrlHandler::QFieldUrlHandler( AppInterface *iface, QObject *parent )
  : QObject( parent ), mIface( iface )
{
}

void QFieldUrlHandler::handleUrl( const QUrl &url )
{
  if ( url.isLocalFile() )
  {
    // Import on the next event loop cycle so the potentially heavy work (e.g.
    // unzipping an archive) does not block the incoming-URL handling
    const QString path = url.toLocalFile();
    QTimer::singleShot( 0, [path]() { PlatformUtilities::instance()->importFile( path ); } );
    return;
  }

  if ( mIface )
  {
    mIface->executeAction( url.toString() );
  }
}
