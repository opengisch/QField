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

#include <QDebug>

QFieldUrlHandler::QFieldUrlHandler( AppInterface *iface, QObject *parent )
  : QObject( parent ), mIface( iface )
{
}

void QFieldUrlHandler::handleUrl( const QUrl &url )
{
  qInfo() << "QField[handleUrl] received url:" << url.toString() << "isLocalFile:" << url.isLocalFile();

  // A file shared with QField is imported, while a qfield:// URL is an action to execute
  if ( url.isLocalFile() )
  {
    PlatformUtilities::instance()->importFile( url.toLocalFile() );
    return;
  }

  if ( mIface )
  {
    mIface->executeAction( url.toString() );
  }
}
