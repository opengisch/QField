#include "sentry_config.h"
#include "sentry_wrapper.h"
#include <QtGlobal>
#import <Sentry.h>
#import <UIKit/UIKit.h>

/*!
 *  A small hack to make ios post initialization load sentry
 *  See: https://bugreports.qt-project.org/browse/QTBUG-38184
 */
@interface QIOSApplicationDelegate
@end
//! Add a category to QIOSApplicationDelegate
@interface QIOSApplicationDelegate (QFieldApplicationDelegate)
@end

@implementation QIOSApplicationDelegate (QFieldApplicationDelegate)
- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

  [SentrySDK startWithConfigureOptions:^(SentryOptions *options) {
      options.dsn = [NSString stringWithUTF8String:sentryDsn];
      options.environment = [NSString stringWithUTF8String:sentryEnv];
      options.debug =
          YES; // Enabled debug when first installing is always helpful
  }];

  return YES;
}
@end

namespace sentry_wrapper {
static QtMessageHandler originalMessageHandler = nullptr;
const char *const applicationName = "QField";
void qfMessageHandler(QtMsgType type, const QMessageLogContext &context,
                      const QString &msg) {}

void init() {}

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
