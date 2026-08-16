/***************************************************************************
    qfcloudprojectsmodel.h
    ---------------------
    begin                : January 2020
    copyright            : (C) 2020 by Matthias Kuhn
    email                : matthias at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFCLOUDPROJECTSMODEL_H
#define QFCLOUDPROJECTSMODEL_H

#include "qfcloudproject.h"
#include "qgsgpkgflusher.h"

#include <QAbstractListModel>
#include <QJsonArray>
#include <QNetworkReply>
#include <QPointer>
#include <QSortFilterProxyModel>
#include <QTimer>


class QNetworkRequest;
class QfCloudConnection;
class QfLayerObserver;
class QgsMapLayer;

/**
 * \ingroup core
 */
class QfCloudProjectsModel : public QAbstractListModel
{
    Q_OBJECT

    // moc needs the full definition to build the metaobject.
    Q_MOC_INCLUDE( "qflayerobserver.h" )

    //! The current cloud connection.
    Q_PROPERTY( QfCloudConnection *cloudConnection READ cloudConnection WRITE setCloudConnection NOTIFY cloudConnectionChanged )
    //! The current layer observer.
    Q_PROPERTY( QfLayerObserver *layerObserver READ layerObserver WRITE setLayerObserver NOTIFY layerObserverChanged )
    //! The current geopackage flusher
    Q_PROPERTY( QgsGpkgFlusher *gpkgFlusher READ gpkgFlusher WRITE setGpkgFlusher NOTIFY gpkgFlusherChanged )

    //! Returns TRUE whether the model is being refreshed
    Q_PROPERTY( bool isRefreshing READ isRefreshing NOTIFY isRefreshingChanged )

    //! Returns TRUE whether the model is creating a project
    Q_PROPERTY( bool isCreating READ isCreating NOTIFY isCreatingChanged )

    Q_PROPERTY( bool hasTemplates READ hasTemplates NOTIFY hasTemplatesChanged )

    //! Currently busy project ids.
    Q_PROPERTY( QSet<QString> busyProjectIds READ busyProjectIds NOTIFY busyProjectIdsChanged )

    //! The current cloud project id of the currently opened project (empty string for non-cloud projects).
    Q_PROPERTY( QString currentProjectId READ currentProjectId WRITE setCurrentProjectId NOTIFY currentProjectIdChanged )
    //! The current cloud project. (null for non-cloud projects).
    Q_PROPERTY( QfCloudProject *currentProject READ currentProject NOTIFY currentProjectChanged )

  public:
    enum ColumnRole
    {
      IdRole = Qt::UserRole + 1,
      OwnerRole,
      PublicRole,
      FeaturedRole,
      NameRole,
      DescriptionRole,
      ModificationRole,
      CheckoutRole,
      StatusRole,
      ProjectOutdatedRole,
      ProjectFileOutdatedRole,
      ErrorStatusRole,
      ErrorStringRole,
      DownloadProgressRole,
      DownloadSizeRole,
      PackagingStatusRole,
      PackagedLayerErrorsRole,
      PushDeltaProgressRole,
      PushDeltaStatusRole,
      PushDeltaStatusStringRole,
      LocalDeltasCountRole,
      LocalPathRole,
      LastLocalExportedAtRole,
      LastLocalPushDeltasRole,
      UserRoleRole,
      UserRoleOriginRole,
      ProjectTypeRole
    };

    //! Attributes controlling fetching of projects
    enum class ProjectsRequestAttribute
    {
      FetchPublicProjects = QNetworkRequest::User + 1,
      ProjectsFetchOffset = QNetworkRequest::User + 2,
      ResetModel = QNetworkRequest::User + 3,
      ProjectId = QNetworkRequest::User + 4,
      ProjectOwnerName = QNetworkRequest::User + 5,
      ProjectSearchTerm = QNetworkRequest::User + 6,
      FromProjectId = QNetworkRequest::User + 7,
    };

    Q_ENUM( ColumnRole )

    QfCloudProjectsModel();

    //! Returns the currently used cloud connection.
    QfCloudConnection *cloudConnection() const;

    //! Sets the cloud connection.
    void setCloudConnection( QfCloudConnection *cloudConnection );

    //! Returns the currently used layer observer.
    QfLayerObserver *layerObserver() const;

    //! Sets the layer observer.
    void setLayerObserver( QfLayerObserver *layerObserver );

    //! Returns TRUE whether the model is being refreshed
    bool isRefreshing() const { return mIsRefreshing; }

    //! Returns TRUE whether the model is being refreshed
    bool isCreating() const { return mIsCreating; }

    //! Returns the cloud project id of the currently opened project.
    QString currentProjectId() const;

    //! Sets the cloud project id of the currently opened project.
    void setCurrentProjectId( const QString &currentProjectId );

    //! Returns the cloud project of the currently oepened project or NULL for non-cloud projects.
    QfCloudProject *currentProject() const;

    //! Returns the geopackage flusher
    QgsGpkgFlusher *gpkgFlusher() const { return mGpkgFlusher; }

    //! Sets the geopackage flusher
    void setGpkgFlusher( QgsGpkgFlusher *flusher );

    //! Returns a set containing the currently busy project ids.
    QSet<QString> busyProjectIds() const;

    /**
     * Requests the cloud projects list from the server.
     * \param shouldResetModel set to TRUE to reset the model
     * \param projectFetchOffset offset for pagination
     */
    Q_INVOKABLE void refreshProjectsList( bool shouldResetModel = true, int projectFetchOffset = 0 );

    //! Pushes all local deltas for given \a projectId. If \a shouldDownloadUpdates is true, also calls `downloadProject`.
    Q_INVOKABLE void projectPush( const QString &projectId, const bool shouldDownloadUpdates );

    //! Remove local cloud project with given \a projectId from the device storage
    Q_INVOKABLE void removeLocalProject( const QString &projectId );

    //! Reverts the deltas of the current cloud project. The changes would applied in reverse order and opposite methods, e.g. "delete" becomes "create".
    Q_INVOKABLE bool revertLocalChangesFromCurrentProject();

    //! Discards the delta records of the current cloud project.
    Q_INVOKABLE bool discardLocalChangesFromCurrentProject();

    //! Returns the model role names.
    QHash<int, QByteArray> roleNames() const override;

    //! Returns number of rows.
    int rowCount( const QModelIndex &parent ) const override;

    //! Returns the data at given \a index with given \a role.
    QVariant data( const QModelIndex &index, int role ) const override;

    //! Downloads a cloud project with given \a projectId and all of its files.
    Q_INVOKABLE void projectPackageAndDownload( const QString &projectId );

    //! Cancels ongoing cloud project download with \a projectId.
    Q_INVOKABLE void projectCancelDownload( const QString &projectId );

    //! Configure localized data paths for cloud projects when available.
    Q_INVOKABLE void updateLocalizedDataPaths( const QString &projectPath );

    //! Return the cloud project for a given \a projectId.
    Q_INVOKABLE QfCloudProject *findProject( const QString &projectId ) const;

    //! Fetches a cloud project for a given \a projectId and appends it to the model.
    Q_INVOKABLE void appendProject( const QString &projectId, bool forceRefresh = false );

    //! Fetches all cloud projects tied to a given \a search term and/or \a owner name.
    Q_INVOKABLE void appendProjects( const QString &owner, const QString &search, int projectFetchOffset = 0 );

    //! Returns a list of unique project owners, excluding projects where the user has access only through public visibility.
    Q_INVOKABLE QStringList uniqueOwners() const;

    /**
     * Transform a locally-stored project into a cloud project by uploading its content to the
     * QFieldCloud server.
     *
     * The converted project will then be removed from the local storage in favor of a newly packaged
     * cloud project downloaded from the server.
     */
    Q_INVOKABLE void createProject( const QString &name, const QString &fromProjectId = QString() );

    /**
     * Returns TRUE if the source model contains at least one template project,
     * regardless of the currently applied filters.
     */
    bool hasTemplates() const;

  signals:
    void cloudConnectionChanged();
    void layerObserverChanged();
    void isRefreshingChanged();
    void isCreatingChanged();
    void currentProjectIdChanged();
    void currentProjectChanged();
    void busyProjectIdsChanged();
    void gpkgFlusherChanged();
    void warning( const QString &message );

    void projectCreated( const QString &projectId, const QString &fromProjectId, const bool hasError, const QString &errorString );
    void projectAppended( const QString &projectId, const bool hasError = false, const QString &errorString = QString() );
    void projectsAppended( const QString &owner, const QString &search, const bool hasError = false, const QString &errorString = QString() );
    void projectDownloaded( const QString &projectId, const QString &projectName, const QString &projectOwner, const bool hasError = false, const QString &errorString = QString() );
    void pushFinished( const QString &projectId, bool isDownloadingProject, bool hasError = false, const QString &errorString = QString() );

    void projectUploaded( const QString &projectId );

    void hasTemplatesChanged();

  private slots:
    void connectionStatusChanged();
    void usernameChanged();
    void urlChanged();

    void projectListReceived();
    void projectReceived();
    void projectCreationReceived();

  private:
    void setupProjectConnections( QfCloudProject *project );

    QModelIndex findProjectIndex( const QString &projectId ) const;

    void loadProjects( const QJsonArray &remoteProjects = QJsonArray(), bool skipLocalProjects = false );
    void insertProjects( const QList<QfCloudProject *> &projects );
    void resetProjects();

    inline QString layerFileName( const QgsMapLayer *layer ) const;

    void updateHasTemplates();
    bool mHasTemplates = false;

    QList<QfCloudProject *> mProjects;
    QfCloudConnection *mCloudConnection = nullptr;

    bool mIsRefreshing = false;
    bool mIsCreating = false;

    QString mCurrentProjectId;
    QPointer<QfCloudProject> mCurrentProject;

    QfLayerObserver *mLayerObserver = nullptr;
    QgsGpkgFlusher *mGpkgFlusher = nullptr;
    QString mUsername;
    QString mUrl;

    const int mProjectsPerFetch = 250;
};

/**
 * \ingroup core
 */
class QfCloudProjectsFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( QfCloudProjectsModel *projectsModel READ projectsModel WRITE setProjectsModel NOTIFY projectsModelChanged )
    Q_PROPERTY( QString textFilter READ textFilter WRITE setTextFilter NOTIFY textFilterChanged )
    Q_PROPERTY( bool showLocalOnly READ showLocalOnly WRITE setShowLocalOnly NOTIFY showLocalOnlyChanged )
    Q_PROPERTY( bool showInValidProjects READ showInValidProjects WRITE setShowInValidProjects NOTIFY showInValidProjectsChanged )
    Q_PROPERTY( bool showFeaturedOnTop READ showFeaturedOnTop WRITE setShowFeaturedOnTop NOTIFY showFeaturedOnTopChanged )
    Q_PROPERTY( bool isSearching READ isSearching NOTIFY isSearchingChanged )
    Q_PROPERTY( bool showTemplates READ showTemplates WRITE setShowTemplates NOTIFY showTemplatesChanged )

  public:
    explicit QfCloudProjectsFilterModel( QObject *parent = nullptr );

    /**
     * Returns the source cloud projects model from which the filtered list is derived.
     */
    QfCloudProjectsModel *projectsModel() const;

    /**
     * Sets the source cloud projects model from which the filtered list is derived.
     * \param projectsModel the source cloud project model
     */
    void setProjectsModel( QfCloudProjectsModel *projectsModel );

    /**
     * Returns whether the filtered cloud projects list will only contain those available locally.
     */
    bool showLocalOnly() const;

    /**
     * Sets whether the filtered cloud projects list will only contain those available locally.
     */
    void setShowLocalOnly( bool showLocalOnly );

    /**
     * Sets a \a text string filter projects by matching it against the project's name or description
     * as well well as owner's name.
     */
    void setTextFilter( const QString &text );

    /**
     * Returns the current text string used to filter projects.
     */
    QString textFilter() const;

    /**
     * Sets whether to include projects that are considered invalid (i.e., failing status)
     * in the filtered list of cloud projects.
     *
     * @param showInValidProjects If true, invalid (failing) projects will be shown;
     * otherwise, they will be filtered out.
     */
    void setShowInValidProjects( bool showInValidProjects );

    /**
     * Returns TRUE if invalid (failing) projects are currently included
     * in the filtered list of cloud projects.
     */
    bool showInValidProjects() const;

    /**
     * Returns TRUE when the filtered list shows template projects only, hiding all
     * non-template projects. When FALSE, template projects are excluded from the list.
     */
    bool showTemplates() const;

    /**
     * Sets whether the filtered list shows template projects only.
     * \param showTemplates when TRUE, only template projects are shown; when FALSE, they are hidden
     */
    void setShowTemplates( bool showTemplates );

    /**
     * Sets whether featured projects will be shown on top of the list.
     */
    void setShowFeaturedOnTop( bool showFeaturedOnTop );

    /**
     * Returns TRUE if featured projects will be shown on top of the list.
     */
    bool showFeaturedOnTop() const;

    /**
     * Returns TRUE while an asynchronous projects appending was triggered by a text filter.
     */
    bool isSearching() const;

  signals:

    void projectsModelChanged();
    void filterChanged();
    void showLocalOnlyChanged();
    void textFilterChanged();
    void showInValidProjectsChanged();
    void showFeaturedOnTopChanged();
    void isSearchingChanged();
    void showTemplatesChanged();

  private slots:
    void triggerProjectsAppending();
    void projectsAppended( const QString &owner, const QString &search, const bool hasError = false, const QString &errorString = QString() );

  protected:
    bool lessThan( const QModelIndex &sourceLeft, const QModelIndex &sourceRight ) const override;
    virtual bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;

  private:
    QfCloudProjectsModel *mSourceModel = nullptr;
    bool mShowLocalOnly = false;
    bool mShowInValidProjects = false;
    bool mShowFeaturedOnTop = false;
    QString mTextFilter;
    QStringList mKeywordFilter;
    QString mOwnerFilter;
    bool mIncludePublic = false;
    bool mIsSearching = false;
    bool mShowTemplates = false;

    QTimer mProjectsAppendingTimer;
};

#endif // QFCLOUDPROJECTSMODEL_H
