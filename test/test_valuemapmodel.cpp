/***************************************************************************
                        test_valuemapmodel
                        --------------------
  begin                : September 2026
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
#include "qfvaluemapmodel.h"
#include "qfvaluemapmodelbase.h"

#include <QAbstractItemModelTester>
#include <QSignalSpy>
#include <qgsvaluemapfieldformatter.h>

#include <memory>

static QVariantList listConfig()
{
  return {
    QVariantMap { { QStringLiteral( "Buckfast bee" ), QStringLiteral( "Apis Mellifera" ) } },
    QVariantMap { { QStringLiteral( "Carniolan honey bee" ), QStringLiteral( "Apis Mellifera Carnica" ) } },
    QVariantMap { { QStringLiteral( "European honey bee" ), QStringLiteral( "Apis Mellifera Mellifera" ) } },
  };
}

static QVariantMap mapConfig()
{
  return {
    { QStringLiteral( "Zebra" ), QStringLiteral( "z" ) },
    { QStringLiteral( "Apple" ), QStringLiteral( "a" ) },
    { QStringLiteral( "Mango" ), QStringLiteral( "m" ) },
  };
}


TEST_CASE( "ValueMapModelBase" )
{
  SECTION( "default" )
  {
    QfValueMapModelBase model;
    REQUIRE( model.rowCount() == 0 );
    REQUIRE_FALSE( model.map().isValid() );
    REQUIRE( model.keyToIndex( QStringLiteral( "Apis Mellifera" ) ) == -1 );
    REQUIRE_FALSE( model.keyForValue( QStringLiteral( "Buckfast bee" ) ).isValid() );
  }

  SECTION( "role names" )
  {
    QfValueMapModelBase model;
    const QHash<int, QByteArray> roles = model.roleNames();
    REQUIRE( roles.value( QfValueMapModel::KeyRole ) == QByteArray( "key" ) );
    REQUIRE( roles.value( QfValueMapModel::ValueRole ) == QByteArray( "value" ) );
    REQUIRE( roles.value( Qt::DisplayRole ) == QByteArray( "display" ) );
  }

  SECTION( "list config" )
  {
    QfValueMapModelBase model;
    QSignalSpy changedSpy( &model, &QfValueMapModelBase::mapChanged );
    QSignalSpy insertedSpy( &model, &QAbstractItemModel::rowsInserted );

    model.setMap( listConfig() );

    REQUIRE( changedSpy.count() == 1 );
    REQUIRE( insertedSpy.count() == 1 );
    REQUIRE( insertedSpy.at( 0 ).at( 1 ).toInt() == 0 );
    REQUIRE( insertedSpy.at( 0 ).at( 2 ).toInt() == 2 );
    REQUIRE( model.rowCount() == 3 );

    REQUIRE( model.index( 0, 0 ).data( QfValueMapModel::KeyRole ).toString() == QStringLiteral( "Apis Mellifera" ) );
    REQUIRE( model.index( 0, 0 ).data( QfValueMapModel::ValueRole ).toString() == QStringLiteral( "Buckfast bee" ) );
    REQUIRE( model.index( 1, 0 ).data( QfValueMapModel::KeyRole ).toString() == QStringLiteral( "Apis Mellifera Carnica" ) );
    REQUIRE( model.index( 1, 0 ).data( QfValueMapModel::ValueRole ).toString() == QStringLiteral( "Carniolan honey bee" ) );
    REQUIRE( model.index( 2, 0 ).data( QfValueMapModel::KeyRole ).toString() == QStringLiteral( "Apis Mellifera Mellifera" ) );
    REQUIRE( model.index( 2, 0 ).data( QfValueMapModel::ValueRole ).toString() == QStringLiteral( "European honey bee" ) );
    REQUIRE( model.index( 2, 0 ).data( Qt::DisplayRole ).toString() == QStringLiteral( "European honey bee" ) );

    REQUIRE( model.map().typeId() == QMetaType::QVariantList );
    REQUIRE( model.map() == QVariant( listConfig() ) );
  }

  SECTION( "map config" )
  {
    QfValueMapModelBase model;
    QSignalSpy changedSpy( &model, &QfValueMapModelBase::mapChanged );
    QSignalSpy insertedSpy( &model, &QAbstractItemModel::rowsInserted );

    model.setMap( mapConfig() );

    REQUIRE( changedSpy.count() == 1 );
    REQUIRE( insertedSpy.count() == 1 );
    REQUIRE( model.rowCount() == 3 );
    REQUIRE( model.index( 0, 0 ).data( QfValueMapModel::ValueRole ).toString() == QStringLiteral( "Apple" ) );
    REQUIRE( model.index( 0, 0 ).data( QfValueMapModel::KeyRole ).toString() == QStringLiteral( "a" ) );
    REQUIRE( model.index( 1, 0 ).data( QfValueMapModel::ValueRole ).toString() == QStringLiteral( "Mango" ) );
    REQUIRE( model.index( 2, 0 ).data( QfValueMapModel::ValueRole ).toString() == QStringLiteral( "Zebra" ) );
    REQUIRE( model.keyToIndex( QStringLiteral( "m" ) ) == 1 );
    REQUIRE( model.keyForValue( QStringLiteral( "Zebra" ) ).toString() == QStringLiteral( "z" ) );
    REQUIRE( model.map().typeId() == QMetaType::QVariantMap );
  }

  SECTION( "keyToIndex" )
  {
    QfValueMapModelBase model;
    model.setMap( listConfig() );
    REQUIRE( model.keyToIndex( QStringLiteral( "Apis Mellifera" ) ) == 0 );
    REQUIRE( model.keyToIndex( QStringLiteral( "Apis Mellifera Carnica" ) ) == 1 );
    REQUIRE( model.keyToIndex( QStringLiteral( "Apis Mellifera Mellifera" ) ) == 2 );
    REQUIRE( model.keyToIndex( QStringLiteral( "apis mellifera" ) ) == -1 );
    REQUIRE( model.keyToIndex( QStringLiteral( "Buckfast bee" ) ) == -1 );
    REQUIRE( model.keyToIndex( QStringLiteral( "Apis" ) ) == -1 );
  }

  SECTION( "keyForValue" )
  {
    QfValueMapModelBase model;
    model.setMap( listConfig() );
    REQUIRE( model.keyForValue( QStringLiteral( "Buckfast bee" ) ).toString() == QStringLiteral( "Apis Mellifera" ) );
    REQUIRE( model.keyForValue( QStringLiteral( "Carniolan honey bee" ) ).toString() == QStringLiteral( "Apis Mellifera Carnica" ) );
    REQUIRE_FALSE( model.keyForValue( QStringLiteral( "buckfast bee" ) ).isValid() );
    REQUIRE_FALSE( model.keyForValue( QStringLiteral( "Apis Mellifera" ) ).isValid() );
  }

  SECTION( "numeric keys" )
  {
    QfValueMapModelBase model;
    model.setMap( QVariantList {
      QVariantMap { { QStringLiteral( "One" ), 1 } },
      QVariantMap { { QStringLiteral( "Two" ), 2 } },
      QVariantMap { { QStringLiteral( "Ten" ), QStringLiteral( "10" ) } },
    } );
    REQUIRE( model.keyToIndex( 1 ) == 0 );
    REQUIRE( model.keyToIndex( QStringLiteral( "1" ) ) == 0 );
    REQUIRE( model.keyToIndex( 2.0 ) == 1 );
    REQUIRE( model.keyToIndex( 10 ) == 2 );
    REQUIRE( model.keyToIndex( 1.5 ) == -1 );

    const QVariant key = model.keyForValue( QStringLiteral( "Two" ) );
    REQUIRE( key.typeId() == QMetaType::Int );
    REQUIRE( key.toInt() == 2 );
  }

  SECTION( "null value" )
  {
    QfValueMapModelBase model;
    model.setMap( QVariantList {
      QVariantMap { { QStringLiteral( "<NULL>" ), QgsValueMapFieldFormatter::NULL_VALUE } },
      QVariantMap { { QStringLiteral( "Yes" ), QStringLiteral( "Y" ) } },
    } );
    REQUIRE( model.rowCount() == 2 );
    REQUIRE( model.index( 0, 0 ).data( QfValueMapModel::KeyRole ).toString() == QgsValueMapFieldFormatter::NULL_VALUE );
    REQUIRE( model.index( 0, 0 ).data( QfValueMapModel::ValueRole ).toString() == QStringLiteral( "<NULL>" ) );
    REQUIRE( model.keyToIndex( QgsValueMapFieldFormatter::NULL_VALUE ) == 0 );
    REQUIRE_FALSE( model.keyForValue( QStringLiteral( "<NULL>" ) ).isValid() );
    REQUIRE( model.keyForValue( QStringLiteral( "Yes" ) ).toString() == QStringLiteral( "Y" ) );
  }

  SECTION( "duplicates" )
  {
    QfValueMapModelBase model;
    model.setMap( QVariantList {
      QVariantMap { { QStringLiteral( "A" ), QStringLiteral( "x" ) } },
      QVariantMap { { QStringLiteral( "B" ), QStringLiteral( "x" ) } },
      QVariantMap { { QStringLiteral( "A" ), QStringLiteral( "y" ) } },
    } );
    REQUIRE( model.rowCount() == 3 );
    REQUIRE( model.keyToIndex( QStringLiteral( "x" ) ) == 0 );
    REQUIRE( model.keyToIndex( QStringLiteral( "y" ) ) == 2 );
    REQUIRE( model.keyForValue( QStringLiteral( "A" ) ).toString() == QStringLiteral( "x" ) );
    REQUIRE( model.keyForValue( QStringLiteral( "B" ) ).toString() == QStringLiteral( "x" ) );
  }

  SECTION( "replace map" )
  {
    QfValueMapModelBase model;
    model.setMap( listConfig() );
    const QVariantList config { QVariantMap { { QStringLiteral( "Only" ), QStringLiteral( "o" ) } } };
    model.setMap( config );
    REQUIRE( model.rowCount() == 1 );
    REQUIRE( model.index( 0, 0 ).data( QfValueMapModel::KeyRole ).toString() == QStringLiteral( "o" ) );
    REQUIRE( model.keyToIndex( QStringLiteral( "Apis Mellifera" ) ) == -1 );
    REQUIRE( model.map() == QVariant( config ) );
  }

  SECTION( "clear map" )
  {
    QfValueMapModelBase model;
    model.setMap( listConfig() );
    QSignalSpy changedSpy( &model, &QfValueMapModelBase::mapChanged );

    model.setMap( QVariantList() );
    REQUIRE( model.rowCount() == 0 );
    REQUIRE( changedSpy.count() == 1 );
    REQUIRE( model.map().typeId() == QMetaType::QVariantList );

    model.setMap( QVariant() );
    REQUIRE( model.rowCount() == 0 );
    REQUIRE( changedSpy.count() == 2 );
    REQUIRE_FALSE( model.map().isValid() );
    REQUIRE_FALSE( model.keyForValue( QStringLiteral( "Buckfast bee" ) ).isValid() );
  }

  SECTION( "invalid index" )
  {
    QfValueMapModelBase model;
    model.setMap( listConfig() );
    REQUIRE_FALSE( model.data( QModelIndex(), QfValueMapModel::KeyRole ).isValid() );
    REQUIRE_FALSE( model.data( QModelIndex(), QfValueMapModel::ValueRole ).isValid() );
  }

  SECTION( "QAbstractItemModelTester" )
  {
    std::unique_ptr<QfValueMapModelBase> model = std::make_unique<QfValueMapModelBase>();
    std::unique_ptr<QAbstractItemModelTester> tester = std::make_unique<QAbstractItemModelTester>( model.get(), QAbstractItemModelTester::FailureReportingMode::Fatal );
  }
}


TEST_CASE( "ValueMapModel" )
{
  SECTION( "forwarding" )
  {
    QfValueMapModel model;
    QSignalSpy changedSpy( &model, &QfValueMapModel::mapChanged );
    model.setMap( listConfig() );
    REQUIRE( changedSpy.count() == 1 );
    REQUIRE( model.rowCount() == 3 );
    REQUIRE( model.map() == QVariant( listConfig() ) );
    REQUIRE( model.filterRole() == QfValueMapModel::ValueRole );
    REQUIRE( model.keyToIndex( QStringLiteral( "Apis Mellifera Carnica" ) ) == 1 );
    REQUIRE( model.keyForValue( QStringLiteral( "European honey bee" ) ).toString() == QStringLiteral( "Apis Mellifera Mellifera" ) );
  }

  SECTION( "filter" )
  {
    QfValueMapModel model;
    model.setMap( listConfig() );
    model.setFilterCaseSensitivity( Qt::CaseInsensitive );

    model.setFilterFixedString( QStringLiteral( "honey" ) );
    REQUIRE( model.rowCount() == 2 );
    REQUIRE( model.index( 0, 0 ).data( QfValueMapModel::ValueRole ).toString() == QStringLiteral( "Carniolan honey bee" ) );
    REQUIRE( model.index( 1, 0 ).data( QfValueMapModel::ValueRole ).toString() == QStringLiteral( "European honey bee" ) );

    model.setFilterFixedString( QStringLiteral( "CARNI" ) );
    REQUIRE( model.rowCount() == 1 );

    model.setFilterFixedString( QStringLiteral( "Apis" ) );
    REQUIRE( model.rowCount() == 0 );

    model.setFilterFixedString( QString() );
    REQUIRE( model.rowCount() == 3 );
  }

  SECTION( "filter keeps source rows" )
  {
    QfValueMapModel model;
    model.setMap( listConfig() );
    model.setFilterFixedString( QStringLiteral( "European" ) );
    REQUIRE( model.rowCount() == 1 );
    REQUIRE( model.keyToIndex( QStringLiteral( "Apis Mellifera Mellifera" ) ) == 2 );
  }
}
