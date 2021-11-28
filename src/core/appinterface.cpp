/***************************************************************************
                            appinterface.cpp
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
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
#include "qgismobileapp.h"

#include <QDirIterator>
#include <QFileInfo>

AppInterface *AppInterface::sAppInterface = nullptr;

AppInterface::AppInterface( QgisMobileapp *app )
  : mApp( app )
{
}

void AppInterface::removeRecentProject( const QString &path )
{
  return mApp->removeRecentProject( path );
}

void AppInterface::loadLastProject()
{
  return mApp->loadLastProject();
}

void AppInterface::loadFile( const QString &path, const QString &name )
{
  const QUrl url( path );
  return mApp->loadProjectFile( url.isLocalFile() ? url.toLocalFile() : url.path(), name );
}

void AppInterface::reloadProject()
{
  return mApp->reloadProjectFile();
}

void AppInterface::readProject()
{
  return mApp->readProjectFile();
}

bool AppInterface::print( const QString &layoutName )
{
  return mApp->print( layoutName );
}

bool AppInterface::printAtlasFeatures( const QString &layoutName, const QList<long long> &featureIds )
{
  return mApp->printAtlasFeatures( layoutName, featureIds );
}

void AppInterface::openFeatureForm()
{
  emit openFeatureFormRequested();
}

void AppInterface::setScreenDimmerActive( bool active )
{
  mApp->setScreenDimmerActive( active );
}

QVariantMap AppInterface::availableLanguages() const
{
  QVariantMap languages;
  QDirIterator it( QStringLiteral( ":/i18n/" ), { QStringLiteral( "*.qm" ) }, QDir::Files );
  while ( it.hasNext() )
  {
    it.next();
    if ( it.fileName().startsWith( "qfield_" ) )
    {
      const int delimiter = it.fileName().indexOf( '.' );
      const QString languageCode = it.fileName().mid( 7, delimiter - 7 );
      const bool hasCoutryCode = languageCode.indexOf( '_' ) > -1;

      const QLocale locale( languageCode );
      QString displayName;
      if ( languageCode == QStringLiteral( "en" ) )
      {
        displayName = QStringLiteral( "english" );
      }
      else if ( locale.nativeLanguageName().isEmpty() )
      {
        displayName = QStringLiteral( "code (%1)" ).arg( languageCode );
      }
      else
      {
        displayName = locale.nativeLanguageName().toLower() + ( hasCoutryCode ? QStringLiteral( " / %1" ).arg( locale.nativeCountryName() ) : QString() );
      }

      languages.insert( languageCode, displayName );
    }
  }
  return languages;
}

bool AppInterface::isFileExtensionSupported( const QString &filename ) const
{
  const QFileInfo fi( filename );
  const QString suffix = fi.suffix().toLower();
  return SUPPORTED_PROJECT_EXTENSIONS.contains( suffix ) || SUPPORTED_VECTOR_EXTENSIONS.contains( suffix ) || SUPPORTED_RASTER_EXTENSIONS.contains( suffix );
}
