

# File qfrecentprojectlistmodel.h

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfrecentprojectlistmodel.h**](qfrecentprojectlistmodel_8h.md)

[Go to the documentation of this file](qfrecentprojectlistmodel_8h.md)


```C++
/***************************************************************************
  qfrecentprojectlistmodel.h

 ---------------------
 begin                : 02.1.2020
 copyright            : (C) 2020 by Mathieu Pellerin
 email                : nirvn dot asia at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QFRECENTPROJECTLISTMODEL_H
#define QFRECENTPROJECTLISTMODEL_H

#include <QAbstractListModel>

class QfRecentProjectListModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    enum ProjectType
    {
      LocalProject,
      CloudProject,
      LinkProject,
      LocalDataset,
    };
    Q_ENUM( ProjectType )

    struct RecentProject
    {
        RecentProject() = default;

        RecentProject( ProjectType type, const QString &title, const QString &path, const QString &thumbnail = QString() )
          : type( type )
          , title( title )
          , path( path )
          , thumbnail( thumbnail )
        {}

        ProjectType type = ProjectType::LocalProject;
        QString title;
        QString path;
        QString thumbnail;
    };

    enum Role
    {
      ProjectTypeRole = Qt::UserRole, 
      ProjectTitleRole,               
      ProjectPathRole,                
      ProjectThumbnailRole,           
    };
    Q_ENUM( Role )


    explicit QfRecentProjectListModel( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    Q_INVOKABLE void reloadModel();

    Q_INVOKABLE static void removeRecentProject( const QString &path );

    static QList<RecentProject> recentProjects( bool skipNonAvailable = false );

    static void saveRecentProjects( const QList<RecentProject> &projects );

  signals:
    void projectChanged();

  private:
    QList<RecentProject> mRecentProjects;
};

#endif // QFRECENTPROJECTLISTMODEL_H
```


