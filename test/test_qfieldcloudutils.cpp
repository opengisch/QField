/***************************************************************************
  test_qfieldcloudutils.cpp - TestQFieldCloudUtils

 ---------------------
 begin                : Apr 2026
 copyright            : (C) 2026 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "catch2.h"
#include "qfieldcloud/qfcloudconnection.h"
#include "utils/qfcloudutils.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QTemporaryDir>
#include <QTimeZone>


TEST_CASE( "CloudSubscriptionInformation" )
{
  SECTION( "Default constructor" )
  {
    QfCloudSubscriptionInformation info;
    REQUIRE( info.plan.isEmpty() );
    REQUIRE( info.storageTotal == 0 );
    REQUIRE( info.storageUsed == 0 );
    REQUIRE( info.status.isEmpty() );
  }

  SECTION( "Equality operator" )
  {
    QJsonObject obj;
    obj[QStringLiteral( "plan_display_name" )] = QStringLiteral( "Community" );
    obj[QStringLiteral( "active_storage_total_bytes" )] = 104857600.0;
    obj[QStringLiteral( "storage_used_bytes" )] = 52428800.0;
    obj[QStringLiteral( "status" )] = QStringLiteral( "active" );

    QfCloudSubscriptionInformation a( obj );
    QfCloudSubscriptionInformation b( obj );
    REQUIRE( a == b );

    b.storageUsed = 0;
    REQUIRE_FALSE( a == b );
  }
}


TEST_CASE( "QFieldCloudUtils::subscriptionManagementUrl" )
{
  const QString defaultUrl = QfCloudConnection::defaultUrl();
  const QString customUrl = QStringLiteral( "https://my-qfieldcloud.example.com" );

  SECTION( "Non-default server returns empty URL" )
  {
    REQUIRE( QfCloudUtils::subscriptionManagementUrl( customUrl, QStringLiteral( "Pro" ), QString(), QStringLiteral( "alice" ) ).isEmpty() );
  }

  SECTION( "Community plan returns plans page" )
  {
    REQUIRE( QfCloudUtils::subscriptionManagementUrl( defaultUrl, QStringLiteral( "Community" ), QString(), QStringLiteral( "alice" ) ) == QStringLiteral( "https://app.qfield.cloud/plans" ) );
  }

  SECTION( "Paid plan with own project returns billing URL" )
  {
    REQUIRE( QfCloudUtils::subscriptionManagementUrl( defaultUrl, QStringLiteral( "Pro" ), QStringLiteral( "alice" ), QStringLiteral( "alice" ) ) == QStringLiteral( "https://app.qfield.cloud/settings/alice/billing" ) );
  }

  SECTION( "Paid plan with empty owner returns billing URL" )
  {
    REQUIRE( QfCloudUtils::subscriptionManagementUrl( defaultUrl, QStringLiteral( "Pro" ), QString(), QStringLiteral( "bob" ) ) == QStringLiteral( "https://app.qfield.cloud/settings/bob/billing" ) );
  }

  SECTION( "Paid plan with different owner returns empty URL" )
  {
    REQUIRE( QfCloudUtils::subscriptionManagementUrl( defaultUrl, QStringLiteral( "Pro" ), QStringLiteral( "org_owner" ), QStringLiteral( "alice" ) ).isEmpty() );
  }

  SECTION( "Community plan takes precedence over owner check" )
  {
    REQUIRE( QfCloudUtils::subscriptionManagementUrl( defaultUrl, QStringLiteral( "Community" ), QStringLiteral( "org_owner" ), QStringLiteral( "alice" ) ) == QStringLiteral( "https://app.qfield.cloud/plans" ) );
  }
}


void writeFile( const QString &filePath, const QByteArray &content )
{
  QFile file( filePath );
  REQUIRE( file.open( QIODevice::WriteOnly ) );
  file.write( content );
}


TEST_CASE( "QFieldCloudUtils::checkCloudifyFeasibility" )
{
  QTemporaryDir projectDir;
  REQUIRE( projectDir.isValid() );

  const QString projectFilePath = QStringLiteral( "%1/project.qgz" ).arg( projectDir.path() );

  SECTION( "Missing folder returns an error" )
  {
    REQUIRE_FALSE( QfCloudUtils::checkCloudifyFeasibility( QStringLiteral( "%1/missing" ).arg( projectDir.path() ) ).isEmpty() );
  }

  SECTION( "Folder without a valid project file returns an error" )
  {
    REQUIRE_FALSE( QfCloudUtils::checkCloudifyFeasibility( projectDir.path() ).isEmpty() );
    writeFile( projectFilePath, QByteArray() );
    REQUIRE_FALSE( QfCloudUtils::checkCloudifyFeasibility( projectDir.path() ).isEmpty() );
  }

  SECTION( "A single project file passes, a second one returns an error" )
  {
    writeFile( projectFilePath, QByteArray( "project" ) );
    REQUIRE( QfCloudUtils::checkCloudifyFeasibility( projectDir.path() ).isEmpty() );
    REQUIRE( QfCloudUtils::checkCloudifyFeasibility( projectFilePath ).isEmpty() );
    writeFile( QStringLiteral( "%1/other.qgs" ).arg( projectDir.path() ), QByteArray( "project" ) );
    REQUIRE_FALSE( QfCloudUtils::checkCloudifyFeasibility( projectDir.path() ).isEmpty() );
  }
}


TEST_CASE( "QfCloudConnection::signupFormErrors" )
{
  SECTION( "Errors are keyed by field name" )
  {
    // Trimmed from a real signup form, where the captcha nests a plain group
    const QString html = QStringLiteral( "<div class=\"form-group is-invalid required\"><label for=\"id_username\">Username</label>"
                                         "<input type=\"text\" name=\"username\" class=\"form-control is-invalid\" required id=\"id_username\">"
                                         "<div class=\"invalid-feedback\">Must begin with a letter.</div></div>"
                                         "<div class=\"form-group\"><input type=\"text\" name=\"referral_code\" class=\"form-control\"></div>"
                                         "<div class=\"form-group is-invalid required\"><label for=\"qfc-signup_id_captcha_1\">Captcha</label>"
                                         "<div class=\"form-group\"><div class=\"mb-3\"><img src=\"/captcha/image/key/\" alt=\"captcha\" class=\"qfc-captcha-image\" /></div>"
                                         "<input type=\"hidden\" name=\"captcha_0\" value=\"key\" class=\"form-control is-invalid\">"
                                         "<input type=\"text\" name=\"captcha_1\" class=\"form-control is-invalid\"></div>"
                                         "<div class=\"invalid-feedback\">Invalid CAPTCHA</div></div>" );

    const QVariantMap errors = QfCloudConnection::signupFormErrors( html );
    REQUIRE( errors.size() == 2 );
    REQUIRE( errors.value( QStringLiteral( "username" ) ).toString() == QStringLiteral( "Must begin with a letter." ) );
    REQUIRE( errors.value( QStringLiteral( "captcha" ) ).toString() == QStringLiteral( "Invalid CAPTCHA" ) );
  }

  SECTION( "Several messages for one field are joined and unescaped" )
  {
    const QString html = QStringLiteral( "<div class=\"form-group is-invalid required\"><input type=\"password\" name=\"password1\" class=\"form-control is-invalid\">"
                                         "<div class=\"invalid-feedback\">This password is too common.</div>"
                                         "<div class=\"invalid-feedback\">The password can&#x27;t be entirely numeric.</div></div>" );
    const QVariantMap errors = QfCloudConnection::signupFormErrors( html );
    REQUIRE( errors.value( QStringLiteral( "password1" ) ).toString() == QStringLiteral( "This password is too common.\nThe password can't be entirely numeric." ) );
  }

  SECTION( "A page without invalid fields has no errors" )
  {
    REQUIRE( QfCloudConnection::signupFormErrors( QStringLiteral( "<form><div class=\"form-group\"><input name=\"email\"></div></form>" ) ).isEmpty() );
  }
}


TEST_CASE( "QFieldCloudUtils::attachmentsHistory" )
{
  QTemporaryDir cloudDir;
  REQUIRE( cloudDir.isValid() );
  QfCloudUtils::setLocalCloudDirectory( cloudDir.path() );

  const QString username = QStringLiteral( "alice" );
  const QString projectId = QStringLiteral( "11111111-1111-1111-1111-111111111111" );
  const QString otherProjectId = QStringLiteral( "22222222-2222-2222-2222-222222222222" );
  REQUIRE( QDir().mkpath( QStringLiteral( "%1/%2" ).arg( cloudDir.path(), username ) ) );

  const QString photoPath = QStringLiteral( "%1/%2/%3/DCIM/photo, one.jpg" ).arg( cloudDir.path(), username, projectId );
  const QString videoPath = QStringLiteral( "%1/%2/%3/DCIM/video.mp4" ).arg( cloudDir.path(), username, projectId );

  auto historyEntry = []( const QDateTime &timestamp, int httpCode, const QString &fileName ) {
    QfCloudAttachmentHistoryEntry entry;
    entry.timestamp = timestamp;
    entry.httpCode = httpCode;
    entry.fileName = fileName;
    return entry;
  };

  const QDateTime firstUpload( QDate( 2026, 10, 20 ), QTime( 9, 30, 15, 250 ), QTimeZone::utc() );
  const QDateTime secondUpload( QDate( 2026, 10, 21 ), QTime( 14, 5, 0 ), QTimeZone::utc() );

  SECTION( "Empty history" )
  {
    REQUIRE( QfCloudUtils::getAttachmentsHistory( username, projectId ).isEmpty() );
    REQUIRE( QfCloudUtils::getAttachmentsHistory( QString(), projectId ).isEmpty() );
  }

  SECTION( "Entries are filtered by project and ordered from oldest to newest" )
  {
    QfCloudUtils::addAttachmentHistory( username, projectId, historyEntry( firstUpload, 201, photoPath ) );
    QfCloudUtils::addAttachmentHistory( username, otherProjectId, historyEntry( firstUpload, 201, videoPath ) );
    QfCloudUtils::addAttachmentHistory( username, projectId, historyEntry( secondUpload, 402, videoPath ) );

    const QList<QfCloudAttachmentHistoryEntry> entries = QfCloudUtils::getAttachmentsHistory( username, projectId );
    REQUIRE( entries.size() == 2 );
    REQUIRE( entries.at( 0 ).timestamp == firstUpload );
    REQUIRE( entries.at( 0 ).httpCode == 201 );
    REQUIRE( entries.at( 0 ).fileName == photoPath );
    REQUIRE( entries.at( 1 ).timestamp == secondUpload );
    REQUIRE( entries.at( 1 ).httpCode == 402 );
    REQUIRE( entries.at( 1 ).fileName == videoPath );

    REQUIRE( QfCloudUtils::getAttachmentsHistory( username, otherProjectId ).size() == 1 );
  }

  SECTION( "Pending attachments are left untouched" )
  {
    REQUIRE( QDir().mkpath( QFileInfo( photoPath ).absolutePath() ) );
    writeFile( photoPath, QByteArray( "photo" ) );
    QfCloudUtils::addPendingAttachments( username, projectId, QStringList() << photoPath );
    REQUIRE( QfCloudUtils::getPendingAttachments( username ).values( projectId ) == QStringList() << photoPath );

    QfCloudUtils::addAttachmentHistory( username, projectId, historyEntry( firstUpload, 201, videoPath ) );

    REQUIRE( QfCloudUtils::getPendingAttachments( username ).values( projectId ) == QStringList() << photoPath );
    REQUIRE( QfCloudUtils::getAttachmentsHistory( username, projectId ).size() == 1 );
  }

  QfCloudUtils::setLocalCloudDirectory( QString() );
}
