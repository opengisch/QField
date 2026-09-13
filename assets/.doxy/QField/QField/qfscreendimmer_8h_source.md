

# File qfscreendimmer.h

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qfscreendimmer.h**](qfscreendimmer_8h.md)

[Go to the documentation of this file](qfscreendimmer_8h.md)


```C++
/***************************************************************************
              qfscreendimmer.h
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

#ifndef QFSCREENDIMMER_H
#define QFSCREENDIMMER_H

#include <QTimer>
#include <qgsapplication.h>

class QfScreenDimmer : public QObject
{
    Q_OBJECT

  public:
    explicit QfScreenDimmer( QgsApplication *app );

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

#endif // QFSCREENDIMMER_H
```


