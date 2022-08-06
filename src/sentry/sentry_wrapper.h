namespace sentry_wrapper
{
  void init();
  void close();
  void install_message_handler();
  void capture_event( const char *message );
} // namespace sentry_wrapper