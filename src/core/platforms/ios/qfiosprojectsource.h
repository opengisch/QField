/***************************************************************************
  qfiosprojectsource.h - QfIosProjectSource

  begin                : September 2021
  copyright            : (C) 2020 by Denis Rouzaud
  email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QFIOSPROJECTSOURCE_H
#define QFIOSPROJECTSOURCE_H

#include "qfprojectsource.h"


class QfIosProjectSource : public QfProjectSource
{
    Q_OBJECT

  public:
    explicit QfIosProjectSource( QObject *parent = nullptr );

    QString projectFromFolder( const QString &folder ) const;

  public slots:
    void pickProject();

  private:
    QQuickItem *mParent = nullptr;
    class ProjectDelegateContainer;
    ProjectDelegateContainer *mDelegate = nullptr;
};


#endif // QFIOSPROJECTSOURCE_H
