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

#define SUPPORTED_DATASET_THUMBNAIL QStringList( { QStringLiteral( "zip" ), QStringLiteral( "tif" ), QStringLiteral( "tiff" ), QStringLiteral( "pdf" ), QStringLiteral( "jpg" ), QStringLiteral( "jpeg" ), QStringLiteral( "png" ), QStringLiteral( "jp2" ), QStringLiteral( "webp" ) } )

class LocalFilesModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( QString currentTitle READ currentTitle NOTIFY currentPathChanged )
    Q_PROPERTY( QString currentPath READ currentPath WRITE setCurrentPath NOTIFY currentPathChanged )
    Q_PROPERTY( int currentDepth READ currentDepth NOTIFY currentPathChanged )
    Q_PROPERTY( bool isDeletedAllowedInCurrentPath READ isDeletedAllowedInCurrentPath NOTIFY currentPathChanged )

  public:
    enum ItemMetaType
    {
      Folder,
      Project,
      Dataset,
      Favorite,
    };
    Q_ENUM( ItemMetaType )

    enum ItemType
    {
      ApplicationFolder,
      ExternalStorage,
      SimpleFolder,
      ProjectFile,
      VectorDataset,
      RasterDataset,
    };
    Q_ENUM( ItemType )

    struct Item
    {
        Item() = default;

        Item( ItemMetaType metaType, ItemType type, const QString &title, const QString &format, const QString &path, qint64 size = 0 )
          : metaType( metaType )
          , type( type )
          , title( title )
          , format( format )
          , path( path )
          , size( size )
        {}

        ItemMetaType metaType = ItemMetaType::Folder;
        ItemType type = ItemType::SimpleFolder;
        QString title;
        QString format;
        QString path;
        qint64 size = 0;
    };

    enum Role
    {
      ItemMetaTypeRole = Qt::UserRole,
      ItemTypeRole,
      ItemTitleRole,
      ItemFormatRole,
      ItemPathRole,
      ItemSizeRole,
      ItemHasThumbnailRole,
    };
    Q_ENUM( Role )


    explicit LocalFilesModel( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    //! Resets the model and sets the first navigation history item to root
    Q_INVOKABLE void resetToRoot();

    //! Resets the model and sets the first navigation history item to the provided \a path
    Q_INVOKABLE void resetToPath( const QString &path );

    //! Returns the current title derived from the current path
    QString currentTitle() const;

    //! Appends the provided \a path to the navigation history
    void setCurrentPath( const QString &path );

    //! Returns the current path
    QString currentPath() const;

    //! Returns the current navigation history depth
    int currentDepth() const { return mHistory.size(); }

    /**
     * Returns TRUE if deletation of files and folders is allow in the current path
     * \note Currently, deletion is only allowed when the path is a sub-child of an application directory
     */
    bool isDeletedAllowedInCurrentPath() const;

    //! Walks the navigation history back up on step
    Q_INVOKABLE void moveUp();

  signals:

    void currentPathChanged();

  private:
    void reloadModel();
    const QString getCurrentTitleFromPath( const QString &path ) const;

    QStringList mHistory;
    QList<Item> mItems;
};

#endif // LOCALFILESMODEL_H
