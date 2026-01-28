/***************************************************************************
                        test_pluginmanager
                        --------------------
  begin                : January 2026
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
#include "pluginmanager.h"

#include <QCoreApplication>
#include <QDir>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTextStream>

namespace
{
  void ensureCoreApplication()
  {
    if ( !QCoreApplication::instance() )
    {
      static int argc = 1;
      static char argv0[] = "qfield-tests";
      static char *argv[] = { argv0, nullptr };
      static QCoreApplication app( argc, argv );
    }
  }

  /**
   * Waits for a spy to reach the expected count by processing events.
   * Required because QML plugin loading happens asynchronously.
   */
  void waitForSpyCount( QSignalSpy &spy, int expectedCount, int timeoutMs = 500 )
  {
    QElapsedTimer timer;
    timer.start();

    while ( spy.count() < expectedCount && timer.elapsed() < timeoutMs )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }
  }

  /**
   * Converts plugin path to settings key using PluginManager's format.
   * Normalizes path separators and replaces '/' with '_'.
   */
  QString pluginPathToKey( const QString &pluginPath )
  {
    QString key = QDir::fromNativeSeparators( pluginPath );
    key.replace( QChar( '/' ), QChar( '_' ) );
    return key;
  }

  QString settingsGroupForPluginPath( const QString &pluginPath )
  {
    return QStringLiteral( "/qfield/plugins/%1" ).arg( pluginPathToKey( pluginPath ) );
  }

  /**
   * RAII class to isolate QSettings to a temporary directory.
   * Prevents tests from polluting real user settings and ensures clean state.
   */
  class ScopedIniSettings
  {
    public:
      explicit ScopedIniSettings( const QString &root )
        : mPreviousDefaultFormat( QSettings::defaultFormat() )
        , mPreviousOrg( QCoreApplication::organizationName() )
        , mPreviousApp( QCoreApplication::applicationName() )
      {
        // Probe current path to restore it later
        QSettings probe( QSettings::IniFormat, QSettings::UserScope, QStringLiteral( "ProbeOrg" ), QStringLiteral( "ProbeApp" ) );
        mPreviousIniUserPath = QFileInfo( probe.fileName() ).absolutePath();

        QSettings::setDefaultFormat( QSettings::IniFormat );
        QSettings::setPath( QSettings::IniFormat, QSettings::UserScope, root );
        QCoreApplication::setOrganizationName( QStringLiteral( "QFieldUnitTests" ) );
        QCoreApplication::setApplicationName( QStringLiteral( "PluginManagerTests" ) );
      }

      ~ScopedIniSettings()
      {
        QCoreApplication::setOrganizationName( mPreviousOrg );
        QCoreApplication::setApplicationName( mPreviousApp );
        QSettings::setPath( QSettings::IniFormat, QSettings::UserScope, mPreviousIniUserPath );
        QSettings::setDefaultFormat( mPreviousDefaultFormat );
      }

    private:
      QSettings::Format mPreviousDefaultFormat;
      QString mPreviousIniUserPath;
      QString mPreviousOrg;
      QString mPreviousApp;
  };

  /**
   * Creates a minimal valid QML plugin for testing.
   * Uses QtQml import to avoid dependency issues in test environments.
   */
  QString createValidPlugin( QTemporaryDir &tempDir, const QString &pluginName )
  {
    const QString pluginDir = tempDir.filePath( pluginName );
    QDir().mkpath( pluginDir );

    const QString pluginPath = pluginDir + "/main.qml";
    QFile file( pluginPath );
    REQUIRE( file.open( QIODevice::WriteOnly | QIODevice::Text ) );

    QTextStream out( &file );
    out << "import QtQml\n"
           "QtObject {\n"
           "  function appWideEnabled() {}\n"
           "  function appWideDisabled() {}\n"
           "  function configure() {}\n"
           "}\n";
    file.close();

    return pluginPath;
  }

  QString createInvalidPlugin( QTemporaryDir &tempDir, const QString &pluginName )
  {
    const QString pluginDir = tempDir.filePath( pluginName );
    QDir().mkpath( pluginDir );

    const QString pluginPath = pluginDir + "/main.qml";
    QFile file( pluginPath );
    REQUIRE( file.open( QIODevice::WriteOnly | QIODevice::Text ) );

    QTextStream out( &file );
    out << "import QtQml\n"
           "This is invalid QML { syntax error\n";
    file.close();

    return pluginPath;
  }

  /**
   * Sets the UUID for a plugin in settings.
   * Note: UUID comes from settings, NOT from QML properties.
   */
  void setPluginUuid( const QString &pluginPath, const QString &uuid )
  {
    QSettings settings;
    settings.beginGroup( settingsGroupForPluginPath( pluginPath ) );
    settings.setValue( QStringLiteral( "uuid" ), uuid );
    settings.endGroup();
    settings.sync();
  }

  void setPermissionGranted( const QString &pluginPath, bool granted )
  {
    QSettings settings;
    settings.beginGroup( settingsGroupForPluginPath( pluginPath ) );
    settings.setValue( QStringLiteral( "permissionGranted" ), granted );
    settings.endGroup();
    settings.sync();
  }

  bool permissionGranted( const QString &pluginPath )
  {
    QSettings settings;
    settings.beginGroup( settingsGroupForPluginPath( pluginPath ) );
    const bool granted = settings.value( QStringLiteral( "permissionGranted" ), false ).toBool();
    settings.endGroup();
    return granted;
  }

  bool userEnabled( const QString &pluginPath )
  {
    QSettings settings;
    settings.beginGroup( settingsGroupForPluginPath( pluginPath ) );
    const bool enabled = settings.value( QStringLiteral( "userEnabled" ), false ).toBool();
    settings.endGroup();
    return enabled;
  }

  bool hasPermissionEntry( const QString &pluginPath )
  {
    QSettings settings;
    settings.beginGroup( settingsGroupForPluginPath( pluginPath ) );
    const bool contains = settings.contains( QStringLiteral( "permissionGranted" ) );
    settings.endGroup();
    return contains;
  }
} // namespace

TEST_CASE( "PluginManager construction" )
{
  ensureCoreApplication();

  QTemporaryDir tempDir;
  REQUIRE( tempDir.isValid() );
  const ScopedIniSettings scopedSettings( tempDir.path() );

  QQmlEngine engine;
  PluginManager manager( &engine );

  REQUIRE( manager.pluginModel() != nullptr );
  manager.availableAppPlugins();
  manager.unloadPlugins();
}

TEST_CASE( "PluginManager loadPlugin permission request" )
{
  ensureCoreApplication();

  QTemporaryDir tempDir;
  REQUIRE( tempDir.isValid() );
  const ScopedIniSettings scopedSettings( tempDir.path() );

  QQmlEngine engine;
  PluginManager manager( &engine );

  SECTION( "skipPermissionCheck=true does not request permission" )
  {
    const QString pluginPath = createValidPlugin( tempDir, "skip_perm" );
    QSignalSpy permissionSpy( &manager, &PluginManager::pluginPermissionRequested );

    manager.loadPlugin( pluginPath, "SkipPerm", true );

    REQUIRE( permissionSpy.count() == 0 );
  }

  SECTION( "no previous decision requests permission" )
  {
    const QString pluginPath = createValidPlugin( tempDir, "needs_perm" );
    QSignalSpy permissionSpy( &manager, &PluginManager::pluginPermissionRequested );

    manager.loadPlugin( pluginPath, "NeedsPerm", false, false );

    waitForSpyCount( permissionSpy, 1 );
    REQUIRE( permissionSpy.count() == 1 );
    REQUIRE( permissionSpy.at( 0 ).at( 0 ).toString() == QStringLiteral( "NeedsPerm" ) );
    REQUIRE( permissionSpy.at( 0 ).at( 1 ).toBool() == false );
  }

  SECTION( "project plugin requests permission with isProjectPlugin=true" )
  {
    const QString pluginPath = createValidPlugin( tempDir, "project_perm" );
    QSignalSpy permissionSpy( &manager, &PluginManager::pluginPermissionRequested );

    manager.loadPlugin( pluginPath, "ProjectPlugin", false, true );

    waitForSpyCount( permissionSpy, 1 );
    REQUIRE( permissionSpy.count() == 1 );
    REQUIRE( permissionSpy.at( 0 ).at( 0 ).toString() == QStringLiteral( "ProjectPlugin" ) );
    REQUIRE( permissionSpy.at( 0 ).at( 1 ).toBool() == true );
  }

  SECTION( "saved permissionGranted=true loads without request" )
  {
    const QString pluginPath = createValidPlugin( tempDir, "saved_allow" );
    setPermissionGranted( pluginPath, true );

    QSignalSpy permissionSpy( &manager, &PluginManager::pluginPermissionRequested );
    manager.loadPlugin( pluginPath, "SavedAllow", false );

    REQUIRE( permissionSpy.count() == 0 );
  }

  SECTION( "saved permissionGranted=false does not load or request" )
  {
    const QString pluginPath = createValidPlugin( tempDir, "saved_deny" );
    setPermissionGranted( pluginPath, false );

    QSignalSpy permissionSpy( &manager, &PluginManager::pluginPermissionRequested );
    QSignalSpy enabledSpy( &manager, &PluginManager::appPluginEnabled );

    manager.loadPlugin( pluginPath, "SavedDeny", false );

    REQUIRE( permissionSpy.count() == 0 );
    REQUIRE( enabledSpy.count() == 0 );
  }

  manager.unloadPlugins();
}

TEST_CASE( "PluginManager enabled/disabled signals" )
{
  ensureCoreApplication();

  QTemporaryDir tempDir;
  REQUIRE( tempDir.isValid() );
  const ScopedIniSettings scopedSettings( tempDir.path() );

  QQmlEngine engine;
  PluginManager manager( &engine );

  SECTION( "grant permanent persists settings without emitting appPluginEnabled" )
  {
    const QString pluginPath = createValidPlugin( tempDir, "enable_signal" );
    const QString uuid = QStringLiteral( "uuid-enable" );
    setPluginUuid( pluginPath, uuid );

    QSignalSpy permissionSpy( &manager, &PluginManager::pluginPermissionRequested );
    QSignalSpy enabledSpy( &manager, &PluginManager::appPluginEnabled );

    manager.loadPlugin( pluginPath, "EnableSignal", false );
    waitForSpyCount( permissionSpy, 1, 2000 );
    REQUIRE( permissionSpy.count() == 1 );

    manager.grantRequestedPluginPermission( true );

    // Grant path does not emit enabled signal -> plugin loads via different flow
    waitForSpyCount( enabledSpy, 1, 200 );
    REQUIRE( enabledSpy.count() == 0 );

    REQUIRE( permissionGranted( pluginPath ) == true );
    REQUIRE( userEnabled( pluginPath ) == true );
  }

  SECTION( "deny permanent emits appPluginDisabled when UUID exists" )
  {
    const QString pluginPath = createValidPlugin( tempDir, "disable_signal" );
    const QString uuid = QStringLiteral( "uuid-disable" );
    setPluginUuid( pluginPath, uuid );

    QSignalSpy permissionSpy( &manager, &PluginManager::pluginPermissionRequested );
    QSignalSpy disabledSpy( &manager, &PluginManager::appPluginDisabled );

    manager.loadPlugin( pluginPath, "DisableSignal", false );
    waitForSpyCount( permissionSpy, 1, 2000 );
    REQUIRE( permissionSpy.count() == 1 );

    manager.denyRequestedPluginPermission( true );
    waitForSpyCount( disabledSpy, 1, 2000 );

    REQUIRE( disabledSpy.count() == 1 );
    REQUIRE( disabledSpy.at( 0 ).at( 0 ).toString() == uuid );
    REQUIRE( permissionGranted( pluginPath ) == false );
  }

  manager.unloadPlugins();
}

TEST_CASE( "PluginManager grant/deny permission" )
{
  ensureCoreApplication();

  QTemporaryDir tempDir;
  REQUIRE( tempDir.isValid() );
  const ScopedIniSettings scopedSettings( tempDir.path() );

  QQmlEngine engine;
  PluginManager manager( &engine );

  SECTION( "grant permanent with UUID sets permissionGranted and userEnabled" )
  {
    const QString pluginPath = createValidPlugin( tempDir, "grant_uuid" );
    setPluginUuid( pluginPath, QStringLiteral( "grant-uuid" ) );

    QSignalSpy permissionSpy( &manager, &PluginManager::pluginPermissionRequested );
    manager.loadPlugin( pluginPath, "GrantUuid", false );
    waitForSpyCount( permissionSpy, 1 );

    manager.grantRequestedPluginPermission( true );

    REQUIRE( permissionGranted( pluginPath ) == true );
    REQUIRE( userEnabled( pluginPath ) == true );
  }

  SECTION( "grant permanent without UUID sets permissionGranted only" )
  {
    const QString pluginPath = createValidPlugin( tempDir, "grant_nouuid" );

    QSignalSpy permissionSpy( &manager, &PluginManager::pluginPermissionRequested );
    manager.loadPlugin( pluginPath, "GrantNoUuid", false );
    waitForSpyCount( permissionSpy, 1 );

    manager.grantRequestedPluginPermission( true );

    REQUIRE( permissionGranted( pluginPath ) == true );
    REQUIRE( userEnabled( pluginPath ) == false );
  }

  SECTION( "deny permanent sets permissionGranted=false and emits appPluginDisabled" )
  {
    const QString pluginPath = createValidPlugin( tempDir, "deny_perm" );
    const QString uuid = QStringLiteral( "deny-uuid" );
    setPluginUuid( pluginPath, uuid );

    QSignalSpy permissionSpy( &manager, &PluginManager::pluginPermissionRequested );
    QSignalSpy disabledSpy( &manager, &PluginManager::appPluginDisabled );

    manager.loadPlugin( pluginPath, "DenyPerm", false );
    waitForSpyCount( permissionSpy, 1 );

    manager.denyRequestedPluginPermission( true );

    REQUIRE( permissionGranted( pluginPath ) == false );
    waitForSpyCount( disabledSpy, 1 );
    REQUIRE( disabledSpy.count() == 1 );
    REQUIRE( disabledSpy.at( 0 ).at( 0 ).toString() == uuid );
  }

  SECTION( "multiple requests: grant applies to last requested plugin" )
  {
    const QString plugin1 = createValidPlugin( tempDir, "first" );
    const QString plugin2 = createValidPlugin( tempDir, "second" );

    QSignalSpy permissionSpy( &manager, &PluginManager::pluginPermissionRequested );

    manager.loadPlugin( plugin1, "First", false );
    manager.loadPlugin( plugin2, "Second", false );

    waitForSpyCount( permissionSpy, 2 );

    // grant applies to the last requested plugin (plugin2)
    manager.grantRequestedPluginPermission( true );

    REQUIRE( hasPermissionEntry( plugin1 ) == false );
    REQUIRE( permissionGranted( plugin2 ) == true );
  }

  manager.unloadPlugins();
}

TEST_CASE( "PluginManager clearPluginPermissions" )
{
  ensureCoreApplication();

  QTemporaryDir tempDir;
  REQUIRE( tempDir.isValid() );
  const ScopedIniSettings scopedSettings( tempDir.path() );

  QQmlEngine engine;
  PluginManager manager( &engine );

  const QString userPlugin = createValidPlugin( tempDir, "user_enabled" );
  const QString autoPlugin = createValidPlugin( tempDir, "auto_enabled" );

  QSettings settings;
  settings.beginGroup( settingsGroupForPluginPath( userPlugin ) );
  settings.setValue( QStringLiteral( "permissionGranted" ), true );
  settings.setValue( QStringLiteral( "userEnabled" ), true );
  settings.endGroup();

  settings.beginGroup( settingsGroupForPluginPath( autoPlugin ) );
  settings.setValue( QStringLiteral( "permissionGranted" ), true );
  settings.setValue( QStringLiteral( "userEnabled" ), false );
  settings.endGroup();

  manager.clearPluginPermissions();

  // user-enabled plugins keep there permissions
  REQUIRE( hasPermissionEntry( userPlugin ) == true );
  REQUIRE( hasPermissionEntry( autoPlugin ) == false );

  manager.unloadPlugins();
}

TEST_CASE( "PluginManager installFromUrl validation" )
{
  ensureCoreApplication();

  QTemporaryDir tempDir;
  REQUIRE( tempDir.isValid() );
  const ScopedIniSettings scopedSettings( tempDir.path() );

  QQmlEngine engine;
  PluginManager manager( &engine );

  QSignalSpy triggeredSpy( &manager, &PluginManager::installTriggered );

  manager.installFromUrl( "" );
  REQUIRE( triggeredSpy.count() == 0 );

  manager.installFromUrl( "   " );
  REQUIRE( triggeredSpy.count() == 0 );

  manager.unloadPlugins();
}

TEST_CASE( "PluginManager findProjectPlugin" )
{
  QTemporaryDir tempDir;
  REQUIRE( tempDir.isValid() );

  SECTION( "exact match" )
  {
    const QString projectPath = tempDir.filePath( "test_project.qgs" );
    QFile( projectPath ).open( QIODevice::WriteOnly );

    const QString pluginPath = tempDir.filePath( "test_project.qml" );
    QFile( pluginPath ).open( QIODevice::WriteOnly );

    REQUIRE( PluginManager::findProjectPlugin( projectPath ) == pluginPath );
  }

  SECTION( "_qfield suffix variant" )
  {
    const QString projectPath = tempDir.filePath( "cloud_project_qfield.qgs" );
    QFile( projectPath ).open( QIODevice::WriteOnly );

    const QString pluginPath = tempDir.filePath( "cloud_project.qml" );
    QFile( pluginPath ).open( QIODevice::WriteOnly );

    REQUIRE( PluginManager::findProjectPlugin( projectPath ) == pluginPath );
  }

  SECTION( "prefer exact match over suffix variant" )
  {
    const QString projectPath = tempDir.filePath( "project_qfield.qgs" );
    QFile( projectPath ).open( QIODevice::WriteOnly );

    const QString exactPlugin = tempDir.filePath( "project_qfield.qml" );
    QFile( exactPlugin ).open( QIODevice::WriteOnly );

    const QString variantPlugin = tempDir.filePath( "project.qml" );
    QFile( variantPlugin ).open( QIODevice::WriteOnly );

    REQUIRE( PluginManager::findProjectPlugin( projectPath ) == exactPlugin );
  }

  SECTION( "no plugin found" )
  {
    const QString projectPath = tempDir.filePath( "no_plugin.qgs" );
    QFile( projectPath ).open( QIODevice::WriteOnly );

    REQUIRE( PluginManager::findProjectPlugin( projectPath ).isEmpty() );
  }
}

TEST_CASE( "PluginManager edge cases" )
{
  ensureCoreApplication();

  QTemporaryDir tempDir;
  REQUIRE( tempDir.isValid() );
  const ScopedIniSettings scopedSettings( tempDir.path() );

  QQmlEngine engine;
  PluginManager manager( &engine );

  // verify no crashes on invalid operations
  manager.unloadPlugin( "/non/existent/path.qml" );
  manager.loadPlugin( "/non/existent/path.qml", "NonExistent", true );
  manager.grantRequestedPluginPermission( false );
  manager.denyRequestedPluginPermission( false );

  manager.unloadPlugins();
}
