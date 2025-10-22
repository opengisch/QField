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

/**
 * \ingroup core
 */
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

    //! Returns the LocalFileItem at the given \a index
    Q_INVOKABLE LocalFileItem get( int index ) const;

    //! Resets the model and sets the first navigation history item to root
    Q_INVOKABLE void resetToRoot();

    //! Resets the model and sets the first navigation history item to the provided \a path
    Q_INVOKABLE void resetToPath( const QString &path );

    //! Returns whether \a path's favorite state is editable
    Q_INVOKABLE bool isPathFavoriteEditable( const QString &path );

    //! Adds the \a path to the favorites list
    Q_INVOKABLE void addToFavorites( const QString &path );

    //! Removes the \a path to the favorites list
    Q_INVOKABLE void removeFromFavorites( const QString &path );

    //! Returns the current title derived from the current path
    QString currentTitle() const;

    //! Appends the provided \a path to the navigation history
    void setCurrentPath( const QString &path );

    //! Returns the current path
    QString currentPath() const;

    //! Returns the current navigation history depth
    int currentDepth() const { return static_cast<int>( mHistory.size() ); }

    /**
     * Returns TRUE if deletation of files and folders is allow in the current path
     * \note Currently, deletion is only allowed when the path is a sub-child of an application directory
     */
    bool isDeletedAllowedInCurrentPath() const;

    //! Walks the navigation history back up on step
    Q_INVOKABLE void moveUp();

    //! Returns whether list is in multi-selection mode or not
    bool inSelectionMode();

    //! Set checked state of an item in list
    Q_INVOKABLE void setChecked( const int &mIdx, const bool &checked );

    //! Set checked state of all items to false
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


/**
 * \ingroup core
 */
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
