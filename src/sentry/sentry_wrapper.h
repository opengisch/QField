/***************************************************************************
  sentry_wrapper.h

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

namespace sentry_wrapper
{
  void init();
  void close();
  void install_message_handler();
  void capture_event( const char *message );
} // namespace sentry_wrapper
