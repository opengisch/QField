

# File screendimmer.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**screendimmer.h**](screendimmer_8h.md)

[Go to the documentation of this file](screendimmer_8h.md)


```C++
/***************************************************************************
              screendimmer.h
               ----------------------------------------------------
              date                 : 26.06.2021
              copyright            : (C) 2021 by Mathieu Pellerin
              email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SCREENDIMMER_H
#define SCREENDIMMER_H

#include <QTimer>
#include <qgsapplication.h>

class ScreenDimmer : public QObject
{
    Q_OBJECT

  public:
    explicit ScreenDimmer( QgsApplication *app );

    void setTimeout( int timeoutSeconds );

    void setSuspend( bool suspend );

  protected:
    bool eventFilter( QObject *obj, QEvent *event ) override;

  private:
    void timeout();

    QTimer mTimer;

    int mTimeoutSeconds = 0;
    bool mSuspend = false;
    bool mDimmed = false;
};

#endif // SCREENDIMMER_H
```


