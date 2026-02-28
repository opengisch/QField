

# File localfilesmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**localfilesmodel.h**](localfilesmodel_8h.md)

[Go to the documentation of this file](localfilesmodel_8h.md)


```C++
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

class LocalFileItem;

class LocalFilesModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( QString currentTitle READ currentTitle NOTIFY currentPathChanged )
    Q_PROPERTY( QString currentPath READ currentPath WRITE setCurrentPath NOTIFY currentPathChanged )
    Q_PROPERTY( int currentDepth READ currentDepth NOTIFY currentPathChanged )
    Q_PROPERTY( bool isDeletedAllowedInCurrentPath READ isDeletedAllowedInCurrentPath NOTIFY currentPathChanged )
    Q_PROPERTY( bool inSelectionMode READ inSelectionMode NOTIFY inSelectionModeChanged )

  public:
    enum ItemMetaType
    {
      Favorite,
      Folder,
      File,
      Project,
      Dataset,
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
      OtherFile,
    };
    Q_ENUM( ItemType )

    enum Role
    {
      ItemMetaTypeRole = Qt::UserRole,
      ItemTypeRole,
      ItemTitleRole,
      ItemFormatRole,
      ItemPathRole,
      ItemSizeRole,
      ItemHasThumbnailRole,
      ItemIsFavoriteRole,
      ItemHasWebdavConfigurationRole,
      ItemCheckedRole,
    };
    Q_ENUM( Role )


    explicit LocalFilesModel( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    Q_INVOKABLE LocalFileItem get( int index ) const;

    Q_INVOKABLE void resetToRoot();

    Q_INVOKABLE void resetToPath( const QString &path );

    Q_INVOKABLE bool isPathFavoriteEditable( const QString &path );

    Q_INVOKABLE void addToFavorites( const QString &path );

    Q_INVOKABLE void removeFromFavorites( const QString &path );

    QString currentTitle() const;

    void setCurrentPath( const QString &path );

    QString currentPath() const;

    int currentDepth() const { return static_cast<int>( mHistory.size() ); }

    bool isDeletedAllowedInCurrentPath() const;

    Q_INVOKABLE void moveUp();

    bool inSelectionMode();

    Q_INVOKABLE void setChecked( const int &mIdx, const bool &checked );

    Q_INVOKABLE void clearSelection();

  signals:

    void currentPathChanged();

    void inSelectionModeChanged();

  private:
    void reloadModel();
    const QString getCurrentTitleFromPath( const QString &path ) const;

    QStringList mHistory;
    QList<LocalFileItem> mItems;

    QStringList mFavorites;

    QString mCreatedProjectsPath;
    QString mImportedProjectsPath;
    QString mImportedDatasetsPath;
    QString mSampleProjectsPath;
};


class LocalFileItem
{
    Q_GADGET

    Q_PROPERTY( LocalFilesModel::ItemMetaType metaType READ metaType CONSTANT )
    Q_PROPERTY( LocalFilesModel::ItemType type READ type CONSTANT )
    Q_PROPERTY( QString title READ title CONSTANT )
    Q_PROPERTY( QString format READ format CONSTANT )
    Q_PROPERTY( QString path READ path CONSTANT )
    Q_PROPERTY( qint64 size READ size CONSTANT )
    Q_PROPERTY( bool checked READ checked CONSTANT )

  public:
    LocalFileItem() = default;

    LocalFileItem( LocalFilesModel::ItemMetaType metaType, LocalFilesModel::ItemType type, const QString &title, const QString &format, const QString &path, qint64 size = 0, bool checked = false )
      : mMetaType( metaType )
      , mType( type )
      , mTitle( title )
      , mFormat( format )
      , mPath( path )
      , mSize( size )
      , mChecked( checked )
    {}

    LocalFilesModel::ItemMetaType metaType() const { return mMetaType; }
    LocalFilesModel::ItemType type() const { return mType; }
    QString title() const { return mTitle; }
    QString format() const { return mFormat; }
    QString path() const { return mPath; }
    qint64 size() const { return mSize; }

    bool checked() const { return mChecked; }
    void setChecked( bool checked ) { mChecked = checked; }

  private:
    LocalFilesModel::ItemMetaType mMetaType = LocalFilesModel::ItemMetaType::Folder;
    LocalFilesModel::ItemType mType = LocalFilesModel::ItemType::SimpleFolder;
    QString mTitle;
    QString mFormat;
    QString mPath;
    qint64 mSize = 0;
    bool mChecked = false;
};

Q_DECLARE_METATYPE( LocalFileItem )
#endif // LOCALFILESMODEL_H
```


