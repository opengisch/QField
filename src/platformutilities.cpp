/***************************************************************************
                            platformutilities.cpp  -  utilities for qfield

                              -------------------
              begin                : Wed Dec 04 10:48:28 CET 2015
              copyright            : (C) 2015 by Marco Bernasocchi
              email                : marco@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "platformutilities.h"
#include <QDebug>

PlatformUtilities::~PlatformUtilities()
{
}

QString PlatformUtilities::configDir() const
{
  return QString();
}

QString PlatformUtilities::shareDir() const
{
  return QString();
}

QString PlatformUtilities::qgsProject() const
{
  return QString();
}

bool PlatformUtilities::createDir( const QString &path, const QString &dirname ) const
{
  Q_UNUSED( path )
  Q_UNUSED( dirname )
  return true;
}

bool PlatformUtilities::rmFile( const QString &path, const QString &filename ) const
{
  Q_UNUSED( path )
  Q_UNUSED( filename )
  return true;
}

PictureSource* PlatformUtilities::getPicture( const QString& prefix )
{
  Q_UNUSED( prefix )
  return nullptr;
}

void PlatformUtilities::open( const QString& data, const QString& type )
{
  Q_UNUSED( data )
  Q_UNUSED( type )
}

