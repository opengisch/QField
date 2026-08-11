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

#include <QJsonObject>


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
