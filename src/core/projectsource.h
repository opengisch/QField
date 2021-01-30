/***************************************************************************
  projectsource.h - ProjectSource

 ---------------------
 begin                : 19.3.2018
 copyright            : (C) 2018 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PROJECTSOURCE_H
#define PROJECTSOURCE_H

#include <QObject>

/**
 * This class represents an ongoing open project intent.
 * It will notify the system with the projectOpened() signal
 * once the user has selected a project to open.
 *
 * The path can either be a QGIS project file (.qgs, .qgz) or
 * a supported vector/raster dataset. The latter will trigger
 * the creation of a project within which the dataset will
 * be loaded.
 *
 * The default implementation does nothing. You probably
 * want to have a look at the AndroidProjectSource subclass.
 */
class ProjectSource : public QObject
{
    Q_OBJECT
  public:
    explicit ProjectSource( QObject *parent = nullptr );

    virtual ~ProjectSource() = default;

  signals:
    /**
     * This signal communitcates, when a project has been opened.
     */
    void projectOpened( const QString &path );
};

#endif // PROJECTSOURCE_H
