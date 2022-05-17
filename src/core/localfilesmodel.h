/***************************************************************************
  localfilesmodel.h

 ---------------------
 begin                : 17.05.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef LOCALFILESMODEL_H
#define LOCALFILESMODEL_H

#include <QAbstractListModel>

class LocalFilesModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( QString currentPath READ currentPath WRITE setCurrentPath NOTIFY currentPathChanged )
    Q_PROPERTY( int currentDepth READ currentDepth NOTIFY currentPathChanged )

  public:
    enum ItemType
    {
      Folder,
      Project,
      VectorDataset,
      RasterDataset,
    };

    struct Item
    {
        Item() = default;

        Item( ItemType type, const QString &title, const QString &path )
          : type( type )
          , title( title )
          , path( path )
        {}

        ItemType type = ItemType::Folder;
        QString title;
        QString path;
    };

    enum Role
    {
      ItemTypeRole = Qt::UserRole,
      ItemTitleRole,
      ItemPathRole,
    };
    Q_ENUM( Role )


    explicit LocalFilesModel( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    Q_INVOKABLE void resetToRoot();
    Q_INVOKABLE void resetToPath( const QString &path );

    void setCurrentPath( const QString &path );

    QString currentPath() const;

    int currentDepth() const { return mHistory.size(); }

    Q_INVOKABLE void moveUp();

  signals:

    void currentPathChanged();

  private:
    void reloadModel();

    QStringList mHistory;
    QList<Item> mItems;
};

#endif // LOCALFILESMODEL_H
