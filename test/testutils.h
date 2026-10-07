/***************************************************************************
                        testutils.h
                        -----------
  begin                : October 2026
  copyright            : (C) 2026 by Kaustuv Pokharel
  email                : kaustuv@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <QCoreApplication>
#include <QFileInfo>
#include <QSettings>


/**
 * RAII class isolating QSettings into a \a root directory for the lifetime of the object,
 * preventing tests from polluting real user settings.
 */
class ScopedIniSettings
{
  public:
    ScopedIniSettings( const QString &root, const QString &applicationName )
      : mPreviousDefaultFormat( QSettings::defaultFormat() )
      , mPreviousOrganizationName( QCoreApplication::organizationName() )
      , mPreviousApplicationName( QCoreApplication::applicationName() )
    {
      QSettings probe( QSettings::IniFormat, QSettings::UserScope, QStringLiteral( "ProbeOrg" ), QStringLiteral( "ProbeApp" ) );
      mPreviousIniUserPath = QFileInfo( probe.fileName() ).absolutePath();

      QSettings::setDefaultFormat( QSettings::IniFormat );
      QSettings::setPath( QSettings::IniFormat, QSettings::UserScope, root );
      QCoreApplication::setOrganizationName( QStringLiteral( "QFieldUnitTests" ) );
      QCoreApplication::setApplicationName( applicationName );
    }

    ~ScopedIniSettings()
    {
      QCoreApplication::setOrganizationName( mPreviousOrganizationName );
      QCoreApplication::setApplicationName( mPreviousApplicationName );
      QSettings::setPath( QSettings::IniFormat, QSettings::UserScope, mPreviousIniUserPath );
      QSettings::setDefaultFormat( mPreviousDefaultFormat );
    }

  private:
    QSettings::Format mPreviousDefaultFormat;
    QString mPreviousIniUserPath;
    QString mPreviousOrganizationName;
    QString mPreviousApplicationName;
};

#endif // TESTUTILS_H
