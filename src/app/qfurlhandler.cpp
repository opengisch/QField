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

QfUrlHandler::QfUrlHandler( QfAppInterface *iface, QObject *parent )
  : QObject( parent ), mIface( iface )
{
}

void QfUrlHandler::handleUrl( const QUrl &url )
{
  // A file shared with QField is imported, while a qfield:// URL is an action to execute
  if ( url.isLocalFile() )
  {
    QfPlatformUtilities::instance()->importFile( url.toLocalFile() );
    return;
  }

  if ( mIface )
  {
    mIface->executeAction( url.toString() );
  }
}
