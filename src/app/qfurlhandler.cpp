/***************************************************************************
              qfurlhandler.cpp
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

#include "qfappinterface.h"
#include "qfplatformutilities.h"
#include "qfurlhandler.h"

#include <QDebug>
#include <QTimer>

QfUrlHandler::QfUrlHandler( QfAppInterface *iface, QObject *parent )
  : QObject( parent ), mIface( iface )
{
}

void QfUrlHandler::handleUrl( const QUrl &url )
{
  qWarning() << "QField[handleUrl] received url:" << url.toString() << "isLocalFile:" << url.isLocalFile();
  if ( url.isLocalFile() )
  {
    // Import on the next event loop cycle so the potentially heavy work (e.g.
    // unzipping an archive) does not block the incoming-URL handling
    const QString path = url.toLocalFile();
    QTimer::singleShot( 0, [path]() { QfPlatformUtilities::instance()->importFile( path ); } );
    return;
  }

  if ( mIface )
  {
    mIface->executeAction( url.toString() );
  }
}
