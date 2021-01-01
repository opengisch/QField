/***************************************************************************
  changelogcontents.h - Changelog

 ---------------------
 begin                : Nov 2020
 copyright            : (C) 2020 by Ivan Ivanov
 email                : ivan@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "changelogcontents.h"
#include "qregularexpression.h"
#include "qgsnetworkaccessmanager.h"
#include "qjsondocument.h"
#include "qjsonarray.h"
#include "qjsonobject.h"

ChangelogContents::ChangelogContents( QObject *parent ):
    QObject( parent )
{
}


void ChangelogContents::request()
{
  QgsNetworkAccessManager *manager = QgsNetworkAccessManager::instance();

  mMarkdown.clear();
  emit markdownChanged();

  mStatus = LoadingStatus;
  emit statusChanged();

  connect(manager, &QNetworkAccessManager::finished, this, [ = ]( QNetworkReply *reply ) {
    QJsonParseError error;
    QJsonDocument json = QJsonDocument::fromJson( reply->readAll(), &error );

    if ( error.error != QJsonParseError::NoError )
    {
      mStatus = ErrorStatus;
      emit statusChanged();
      return;
    }

    QString changelog;
    QString versionNumbersOnly;
    QList<int> qfieldVersion = parseVersion( APP_VERSION_STR );
    const QJsonArray releases = json.array();

    for ( const QJsonValue &releaseValue : releases )
    {
      QVariantMap release = releaseValue.toObject().toVariantMap();
      QList<int> releaseVersion = parseVersion( release.value( QStringLiteral( "tag_name" ) ).toString() );

      if (releaseVersion.isEmpty())
        continue;

      // most probably developer version with no proper version set
      if (qfieldVersion.isEmpty())
        qfieldVersion = releaseVersion;

      if (qfieldVersion[0] != releaseVersion[0] || qfieldVersion[1] != releaseVersion[1])
        continue;

      if ( versionNumbersOnly.isEmpty() )
      {
        versionNumbersOnly = QStringLiteral("%1.%2.%3").arg( releaseVersion[0] ).arg( releaseVersion[1] ).arg( releaseVersion[2] );
      }

      const QString releaseChangelog = QStringLiteral("\n#\n# ") + release["name"].toString() + QStringLiteral("\n\n") + release["body"].toString() + QStringLiteral("\n");
      changelog = releaseChangelog + changelog;
    }

    changelog += QStringLiteral("\n") + QStringLiteral("[") + tr("Previous releases on GitHub") + QStringLiteral("](https://github.com/opengisch/qfield/releases)");

    QRegularExpression regexpFirstTitle( QStringLiteral( "^\n#\n# " ) );
    changelog = changelog.replace( regexpFirstTitle, QStringLiteral("\n# ") );
    QRegularExpression regexpAllTitles( QStringLiteral( "^##(.+)$" ), QRegularExpression::MultilineOption );
    changelog = changelog.replace( regexpAllTitles, QStringLiteral("\n###\n##\\1\n\n\n") );
    changelog = "Up to release **" + versionNumbersOnly + "**" + changelog;

    mStatus = SuccessStatus;
    mMarkdown = changelog;

    emit statusChanged();
    emit markdownChanged();
  });

  manager->get( QNetworkRequest( QUrl( QStringLiteral( "https://api.github.com/repos/opengisch/qfield/releases" ) ) ) );
}

QString ChangelogContents::markdown()
{
  return mMarkdown;
}

ChangelogContents::Status ChangelogContents::status()
{
  return mStatus;
}

QList<int> ChangelogContents::parseVersion( const QString &version )
{
  QRegularExpression regexp( "^[a-z]*" );
  QString cleanVersion = QString( version ).replace( regexp, "" );
  QStringList parts = cleanVersion.split( QStringLiteral( "." ) );

  if ( parts.size() != 3 )
    return QList<int>();

  if (parts[0].toInt() >= 0 && parts[1].toInt() >= 0 && parts[2].toInt() >= 0)
    return QList<int>({parts[0].toInt(), parts[1].toInt(), parts[2].toInt()});

  return QList<int>();
}
