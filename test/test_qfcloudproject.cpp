/***************************************************************************
                        test_qfcloudproject
                        --------------------
  begin                : August 2026
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
#include "catch2.h"
#include "qfcloudproject.h"

#include <QVariantHash>


// Builds a minimal project details hash as returned by /api/v1/projects/.
// fromDetails() tolerates missing keys (they default) and handles a null
// connection, and with no local path it skips all setProjectSetting/filesystem
// work, so no network / server / filesystem is required.
static QVariantHash makeDetails( const QString &id, const QString &projectType )
{
  QVariantHash details;
  details.insert( QStringLiteral( "id" ), id );
  details.insert( QStringLiteral( "name" ), QStringLiteral( "test-project" ) );
  details.insert( QStringLiteral( "owner" ), QStringLiteral( "tester" ) );
  if ( !projectType.isNull() )
    details.insert( QStringLiteral( "project_type" ), projectType );
  return details;
}


TEST_CASE( "QfCloudProject project type" )
{
  SECTION( "Regular type" )
  {
    QfCloudProject *project = QfCloudProject::fromDetails( makeDetails( QStringLiteral( "p1" ), QStringLiteral( "regular" ) ), nullptr );
    REQUIRE( project->projectType() == QfCloudProject::ProjectType::Regular );
    delete project;
  }

  SECTION( "Shared datasets type" )
  {
    QfCloudProject *project = QfCloudProject::fromDetails( makeDetails( QStringLiteral( "p2" ), QStringLiteral( "shared_datasets" ) ), nullptr );
    REQUIRE( project->projectType() == QfCloudProject::ProjectType::SharedDatasets );
    delete project;
  }

  SECTION( "Template type" )
  {
    QfCloudProject *project = QfCloudProject::fromDetails( makeDetails( QStringLiteral( "p3" ), QStringLiteral( "template" ) ), nullptr );
    REQUIRE( project->projectType() == QfCloudProject::ProjectType::Template );
    delete project;
  }

  SECTION( "Missing project_type falls back to Regular" )
  {
    QfCloudProject *project = QfCloudProject::fromDetails( makeDetails( QStringLiteral( "p4" ), QString() ), nullptr );
    REQUIRE( project->projectType() == QfCloudProject::ProjectType::Regular );
    delete project;
  }

  SECTION( "Unknown project_type falls back to Regular" )
  {
    QfCloudProject *project = QfCloudProject::fromDetails( makeDetails( QStringLiteral( "p5" ), QStringLiteral( "something_new" ) ), nullptr );
    REQUIRE( project->projectType() == QfCloudProject::ProjectType::Regular );
    delete project;
  }

  SECTION( "Case-insensitive parsing" )
  {
    QfCloudProject *project = QfCloudProject::fromDetails( makeDetails( QStringLiteral( "p6" ), QStringLiteral( "TEMPLATE" ) ), nullptr );
    REQUIRE( project->projectType() == QfCloudProject::ProjectType::Template );
    delete project;
  }
}


TEST_CASE( "QfCloudProject projectTypeFromString" )
{
  REQUIRE( QfCloudProject::projectTypeFromString( QStringLiteral( "regular" ) ) == QfCloudProject::ProjectType::Regular );
  REQUIRE( QfCloudProject::projectTypeFromString( QStringLiteral( "shared_datasets" ) ) == QfCloudProject::ProjectType::SharedDatasets );
  REQUIRE( QfCloudProject::projectTypeFromString( QStringLiteral( "template" ) ) == QfCloudProject::ProjectType::Template );
  REQUIRE( QfCloudProject::projectTypeFromString( QString() ) == QfCloudProject::ProjectType::Regular );
  REQUIRE( QfCloudProject::projectTypeFromString( QStringLiteral( "garbage" ) ) == QfCloudProject::ProjectType::Regular );
}


TEST_CASE( "QfCloudProject setProjectType updates the type" )
{
  QfCloudProject *project = QfCloudProject::fromDetails( makeDetails( QStringLiteral( "p7" ), QStringLiteral( "regular" ) ), nullptr );
  REQUIRE( project->projectType() == QfCloudProject::ProjectType::Regular );

  project->setProjectType( QfCloudProject::ProjectType::SharedDatasets );
  REQUIRE( project->projectType() == QfCloudProject::ProjectType::SharedDatasets );

  project->setProjectType( QfCloudProject::ProjectType::Template );
  REQUIRE( project->projectType() == QfCloudProject::ProjectType::Template );

  delete project;
}
