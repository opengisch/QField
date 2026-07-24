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
#include "qfieldurlhandler.h"

QFieldUrlHandler::QFieldUrlHandler( AppInterface *iface, QObject *parent )
  : QObject( parent ), mIface( iface )
{
}

void QFieldUrlHandler::handleUrl( const QUrl &url )
{
  if ( mIface )
  {
    mIface->executeAction( url.toString() );
  }
}
