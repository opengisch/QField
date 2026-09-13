

# File sentry\_wrapper.h

[**File List**](files.md) **>** [**sentry**](dir_94ae90e00fc602cdc900d6f900906c1c.md) **>** [**sentry\_wrapper.h**](sentry__wrapper_8h.md)

[Go to the documentation of this file](sentry__wrapper_8h.md)


```C++
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
  void capture_event( const char *message, const char *cloudUser );
} // namespace sentry_wrapper
```


