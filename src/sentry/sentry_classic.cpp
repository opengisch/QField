#include "sentry_config.h"
#include "sentry_wrapper.h"

#include <QStandardPaths>
#include <QString>
#include <QtGlobal>

#include <sentry.h>
#ifdef ANDROID
#include <android/log.h>
#endif

namespace sentry_wrapper
{
  static QtMessageHandler originalMessageHandler = nullptr;

  static const char *
    logLevelForMessageType( QtMsgType msgType )
  {
    switch ( msgType )
    {
      case QtDebugMsg:
        return "debug";
      case QtWarningMsg:
        return "warning";
      case QtCriticalMsg:
        return "error";
      case QtFatalMsg:
        return "fatal";
      case QtInfoMsg:
        Q_FALLTHROUGH();
      default:
        return "info";
    }
  }

  void qfMessageHandler( QtMsgType type, const QMessageLogContext &context, const QString &msg )
  {
    sentry_value_t crumb
      = sentry_value_new_breadcrumb( "default", qUtf8Printable( msg ) );

    sentry_value_set_by_key(
      crumb, "category", sentry_value_new_string( context.category ) );
    sentry_value_set_by_key(
      crumb, "level", sentry_value_new_string( logLevelForMessageType( type ) ) );

    sentry_value_t location = sentry_value_new_object();
    sentry_value_set_by_key(
      location, "file", sentry_value_new_string( context.file ) );
    sentry_value_set_by_key(
      location, "line", sentry_value_new_int32( context.line ) );
    sentry_value_set_by_key( crumb, "data", location );

    sentry_add_breadcrumb( crumb );

#if ANDROID
    QString report = msg;
    if ( context.file && !QString( context.file ).isEmpty() )
    {
      report += " in file ";
      report += QString( context.file );
      report += " line ";
      report += QString::number( context.line );
    }

    if ( context.function && !QString( context.function ).isEmpty() )
    {
      report += +" function ";
      report += QString( context.function );
    }

    const char *const local = report.toLocal8Bit().constData();
    switch ( type )
    {
      case QtDebugMsg:
        __android_log_write( ANDROID_LOG_DEBUG, applicationName, local );
        break;
      case QtInfoMsg:
        __android_log_write( ANDROID_LOG_INFO, applicationName, local );
        break;
      case QtWarningMsg:
        __android_log_write( ANDROID_LOG_WARN, applicationName, local );
        break;
      case QtCriticalMsg:
        __android_log_write( ANDROID_LOG_ERROR, applicationName, local );
        break;
      case QtFatalMsg:
      default:
        __android_log_write( ANDROID_LOG_FATAL, applicationName, local );
        abort();
    }
#endif

    if ( originalMessageHandler )
      originalMessageHandler( type, context, msg );
  }

  void install_message_handler()
  {
    originalMessageHandler = qInstallMessageHandler( qfMessageHandler );
  }

  void init()
  {
    sentry_options_t *options = sentry_options_new();
    sentry_options_set_dsn( options, sentryDsn );
    sentry_options_set_environment( options, sentryEnv );
    sentry_options_set_debug( options, 1 );
    sentry_options_set_database_path( options, ( QStandardPaths::writableLocation( QStandardPaths::AppDataLocation ) + '/' + QStringLiteral( ".sentry-native" ) ).toUtf8().constData() );
    sentry_init( options );
  }

  void close()
  {
    sentry_close();
  }
} // namespace sentry_wrapper
