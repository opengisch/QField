/***************************************************************************

               ----------------------------------------------------
              date                 : 15.2.2015
              copyright            : (C) 2015 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "badlayerhandler.h"

BadLayerHandler::BadLayerHandler()
  : QgsProjectBadLayerHandler()
{

}

BadLayerHandler::~BadLayerHandler()
{

}



void BadLayerHandler::handleBadLayers( QList<QDomNode> layers, QDomDocument projectDom )
{
}
