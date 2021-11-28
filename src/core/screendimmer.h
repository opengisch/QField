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

/**
 * @brief The ScreenDimmer class handles dimming of screen brightness.
 */
class ScreenDimmer : public QObject
{
    Q_OBJECT

  public:
    explicit ScreenDimmer( QgsApplication *app );

    /**
     * Sets whether the screen dimmer is \a active or not.
     */
    void setActive( bool active );

    /**
     * Temporarily suspends the screen dimmer when \a suspend is set to TRUE.
     */
    void setSuspend( bool suspend );

  protected:
    bool eventFilter( QObject *obj, QEvent *event ) override;

  private:
    void timeout();

    QTimer mTimer;

    bool mActive = false;
    bool mSuspend = false;
    bool mDimmed = false;
};

#endif // SCREENDIMMER_H
