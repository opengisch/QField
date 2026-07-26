/***************************************************************************
  recentprojectlistmodel.h

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
#ifndef RECENTPROJECTLISTMODEL_H
#define RECENTPROJECTLISTMODEL_H

#include <QAbstractListModel>

/**
 * \ingroup core
 */
class RecentProjectListModel : public QAbstractListModel
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

    /*!
     * Roles to get the data of the model.
    */
    enum Role
    {
      ProjectTypeRole = Qt::UserRole, //! the project type (e.g., local, cloud, etc.)
      ProjectTitleRole,               //! the project title
      ProjectPathRole,                //! the project path
      ProjectThumbnailRole,           //! the project thumbnail
    };
    Q_ENUM( Role )


    explicit RecentProjectListModel( QObject *parent = nullptr );

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

#endif // RECENTPROJECTLISTMODEL_H
