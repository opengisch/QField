/***************************************************************************
                            feedback.cpp

                              -------------------
              begin                : January 2021
              copyright            : (C) 2021 by Matthias Kuhn
              email                : matthias@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "feedback.h"

Feedback::Feedback()
{
  connect( this, &QgsFeedback::progressChanged, this, [this]( double progress )
  {
    mProgressProxy = progress;
    emit progressChanged();
  } );
}

QString Feedback::status() const
{
  return mStatus;
}

void Feedback::setStatus( const QString &status )
{
  if ( status == mStatus )
    return;
  mStatus = status;
  emit statusChanged();
}

double Feedback::progress()
{
  return mProgressProxy;
}
