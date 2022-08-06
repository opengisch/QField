/***************************************************************************
  sentry_cocoa.mm

 ---------------------
 begin                : August 2022
 copyright            : (C) 2022 by Matthias Kuhn, OPENGIS.ch
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "sentry_config.h"
#include "sentry_wrapper.h"
#include <QtGlobal>
#import <Sentry.h>

namespace sentry_wrapper {
static QtMessageHandler originalMessageHandler = nullptr;
const char *const applicationName = "QField";
void qfMessageHandler(QtMsgType type, const QMessageLogContext &context,
                      const QString &msg) {}

void init() {
  [SentrySDK startWithConfigureOptions:^(SentryOptions *options) {
      options.dsn = [NSString stringWithUTF8String:sentryDsn];
      options.environment = [NSString stringWithUTF8String:sentryEnv];
      options.debug =
          YES; // Enabled debug when first installing is always helpful
  }];
}

void close() {}

void install_message_handler() {
  originalMessageHandler = qInstallMessageHandler(qfMessageHandler);
}

void capture_event(const char *message) {
  SentryId *eventId =
      [SentrySDK captureMessage:[NSString stringWithUTF8String:message]];
#if 0
    // How can we send info level and title with cocoa?
    sentry_wrapper::sentry_capture_event( sentry_value_new_message_event(
      SENTRY_LEVEL_INFO,
      "custom",
      message ) );
#endif
}
}
