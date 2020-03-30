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

class RecentProjectListModel : public QAbstractListModel
{
  Q_OBJECT

  enum ProjectType
  {
    LocalProject,
    CloudProject,
  };

  struct RecentProject
  {
    RecentProject()
    {}

    RecentProject( ProjectType type, const QString &title, const QString &path )
    {
      this->type = type;
      this->title = title;
      this->path = path;
    }

    ProjectType type;
    QString title;
    QString path;
  };

  /*!
     * Roles to get the data of the model.
    */
  enum Roles
  {
    ProjectTypeRole = Qt::UserRole, //! the project type (e.g., local, cloud, etc.)
    ProjectTitleRole,               //! the project title
    ProjectPathRole,                //! the project path
  };

public:
  RecentProjectListModel( QObject *parent = nullptr );

  QHash<int, QByteArray> roleNames() const override;

  int rowCount( const QModelIndex &parent ) const override;
  QVariant data( const QModelIndex &index, int role ) const override;

  Q_INVOKABLE void reloadModel();

signals:
  void projectChanged();

private:
  QList<RecentProject> mRecentProjects;
};

#endif // RECENTPROJECTLISTMODEL_H
