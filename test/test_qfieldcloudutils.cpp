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
#include <QJsonObject>
#include <QTemporaryDir>


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


TEST_CASE( "QFieldCloudUtils::signupFormErrors" )
{
  SECTION( "Errors are keyed by field name" )
  {
    // Trimmed from a signup form rendered by app.qfield.cloud, the captcha nests a plain group
    const QString html = QStringLiteral( "<div class=\"form-group is-invalid required\"><label for=\"id_username\">Username</label>"
                                         "<input type=\"text\" name=\"username\" class=\"form-control is-invalid\" required id=\"id_username\">"
                                         "<div class=\"invalid-feedback\">Must begin with a letter.</div></div>"
                                         "<div class=\"form-group\"><input type=\"text\" name=\"referral_code\" class=\"form-control\"></div>"
                                         "<div class=\"form-group is-invalid required\"><label for=\"qfc-signup_id_captcha_1\">Captcha</label>"
                                         "<div class=\"form-group\"><div class=\"mb-3\"><img src=\"/captcha/image/key/\" alt=\"captcha\" class=\"qfc-captcha-image\" /></div>"
                                         "<input type=\"hidden\" name=\"captcha_0\" value=\"key\" class=\"form-control is-invalid\">"
                                         "<input type=\"text\" name=\"captcha_1\" class=\"form-control is-invalid\"></div>"
                                         "<div class=\"invalid-feedback\">Invalid CAPTCHA</div></div>" );

    const QVariantMap errors = QfCloudUtils::signupFormErrors( html );
    REQUIRE( errors.size() == 2 );
    REQUIRE( errors.value( QStringLiteral( "username" ) ).toString() == QStringLiteral( "Must begin with a letter." ) );
    REQUIRE( errors.value( QStringLiteral( "captcha" ) ).toString() == QStringLiteral( "Invalid CAPTCHA" ) );
  }

  SECTION( "Several messages for one field are joined and unescaped" )
  {
    const QString html = QStringLiteral( "<div class=\"form-group is-invalid required\"><input type=\"password\" name=\"password1\" class=\"form-control is-invalid\">"
                                         "<div class=\"invalid-feedback\">This password is too common.</div>"
                                         "<div class=\"invalid-feedback\">The password can&#x27;t be entirely numeric.</div></div>" );
    const QVariantMap errors = QfCloudUtils::signupFormErrors( html );
    REQUIRE( errors.value( QStringLiteral( "password1" ) ).toString() == QStringLiteral( "This password is too common.\nThe password can't be entirely numeric." ) );
  }

  SECTION( "A page without invalid fields has no errors" )
  {
    REQUIRE( QfCloudUtils::signupFormErrors( QStringLiteral( "<form><div class=\"form-group\"><input name=\"email\"></div></form>" ) ).isEmpty() );
  }
}
