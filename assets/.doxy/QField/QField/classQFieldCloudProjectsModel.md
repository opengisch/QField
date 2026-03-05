

# Class QFieldCloudProjectsModel



[**ClassList**](annotated.md) **>** [**QFieldCloudProjectsModel**](classQFieldCloudProjectsModel.md)








Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ColumnRole**](#enum-columnrole)  <br> |
| enum  | [**ProjectsRequestAttribute**](#enum-projectsrequestattribute)  <br>_Attributes controlling fetching of projects._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QSet&lt; QString &gt; | [**busyProjectIds**](classQFieldCloudProjectsModel.md#property-busyprojectids-12)  <br>_Currently busy project ids._  |
| property [**QFieldCloudConnection**](classQFieldCloudConnection.md) \* | [**cloudConnection**](classQFieldCloudProjectsModel.md#property-cloudconnection-12)  <br>_The current cloud connection._  |
| property [**QFieldCloudProject**](classQFieldCloudProject.md) \* | [**currentProject**](classQFieldCloudProjectsModel.md#property-currentproject-12)  <br>_The current cloud project. (null for non-cloud projects)._  |
| property QString | [**currentProjectId**](classQFieldCloudProjectsModel.md#property-currentprojectid-12)  <br>_The current cloud project id of the currently opened project (empty string for non-cloud projects)._  |
| property [**QgsGpkgFlusher**](classQgsGpkgFlusher.md) \* | [**gpkgFlusher**](classQFieldCloudProjectsModel.md#property-gpkgflusher-12)  <br>_The current geopackage flusher._  |
| property bool | [**isCreating**](classQFieldCloudProjectsModel.md#property-iscreating-12)  <br>_Returns TRUE whether the model is creating a project._  |
| property bool | [**isRefreshing**](classQFieldCloudProjectsModel.md#property-isrefreshing-12)  <br>_Returns TRUE whether the model is being refreshed._  |
| property [**LayerObserver**](classLayerObserver.md) \* | [**layerObserver**](classQFieldCloudProjectsModel.md#property-layerobserver-12)  <br>_The current layer observer._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**busyProjectIdsChanged**](classQFieldCloudProjectsModel.md#signal-busyprojectidschanged)  <br> |
| signal void | [**cloudConnectionChanged**](classQFieldCloudProjectsModel.md#signal-cloudconnectionchanged)  <br> |
| signal void | [**currentProjectChanged**](classQFieldCloudProjectsModel.md#signal-currentprojectchanged)  <br> |
| signal void | [**currentProjectIdChanged**](classQFieldCloudProjectsModel.md#signal-currentprojectidchanged)  <br> |
| signal void | [**deltaListModelChanged**](classQFieldCloudProjectsModel.md#signal-deltalistmodelchanged)  <br> |
| signal void | [**gpkgFlusherChanged**](classQFieldCloudProjectsModel.md#signal-gpkgflusherchanged)  <br> |
| signal void | [**isCreatingChanged**](classQFieldCloudProjectsModel.md#signal-iscreatingchanged)  <br> |
| signal void | [**isRefreshingChanged**](classQFieldCloudProjectsModel.md#signal-isrefreshingchanged)  <br> |
| signal void | [**layerObserverChanged**](classQFieldCloudProjectsModel.md#signal-layerobserverchanged)  <br> |
| signal void | [**projectAppended**](classQFieldCloudProjectsModel.md#signal-projectappended) (const QString & projectId, const bool hasError=false, const QString & errorString=QString()) <br> |
| signal void | [**projectCreated**](classQFieldCloudProjectsModel.md#signal-projectcreated) (const QString & projectId, const bool hasError=false, const QString & errorString=QString()) <br> |
| signal void | [**projectDownloaded**](classQFieldCloudProjectsModel.md#signal-projectdownloaded) (const QString & projectId, const QString & projectName, const bool hasError=false, const QString & errorString=QString()) <br> |
| signal void | [**projectUploaded**](classQFieldCloudProjectsModel.md#signal-projectuploaded) (const QString & projectId) <br> |
| signal void | [**pushFinished**](classQFieldCloudProjectsModel.md#signal-pushfinished) (const QString & projectId, bool isDownloadingProject, bool hasError=false, const QString & errorString=QString()) <br> |
| signal void | [**warning**](classQFieldCloudProjectsModel.md#signal-warning) (const QString & message) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QFieldCloudProjectsModel**](#function-qfieldcloudprojectsmodel) () <br> |
|  Q\_INVOKABLE void | [**appendProject**](#function-appendproject) (const QString & projectId) <br>_Fetches a cloud project for a given_ _projectId_ _and appends it to the model._ |
|  QSet&lt; QString &gt; | [**busyProjectIds**](#function-busyprojectids-22) () const<br>_Returns a set containing the currently busy project ids._  |
|  [**QFieldCloudConnection**](classQFieldCloudConnection.md) \* | [**cloudConnection**](#function-cloudconnection-22) () const<br>_Returns the currently used cloud connection._  |
|  Q\_INVOKABLE void | [**createProject**](#function-createproject) (const QString name) <br> |
|  [**QFieldCloudProject**](classQFieldCloudProject.md) \* | [**currentProject**](#function-currentproject-22) () const<br>_Returns the cloud project of the currently oepened project or NULL for non-cloud projects._  |
|  QString | [**currentProjectId**](#function-currentprojectid-22) () const<br>_Returns the cloud project id of the currently opened project._  |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br>_Returns the data at given_ _index_ _with given__role_ _._ |
|  Q\_INVOKABLE bool | [**discardLocalChangesFromCurrentProject**](#function-discardlocalchangesfromcurrentproject) () <br>_Discards the delta records of the current cloud project._  |
|  Q\_INVOKABLE [**QFieldCloudProject**](classQFieldCloudProject.md) \* | [**findProject**](#function-findproject) (const QString & projectId) const<br>_Return the cloud project for a given_ _projectId_ _._ |
|  [**QgsGpkgFlusher**](classQgsGpkgFlusher.md) \* | [**gpkgFlusher**](#function-gpkgflusher-22) () const<br>_Returns the geopackage flusher._  |
|  bool | [**isCreating**](#function-iscreating-22) () const<br>_Returns TRUE whether the model is being refreshed._  |
|  bool | [**isRefreshing**](#function-isrefreshing-22) () const<br>_Returns TRUE whether the model is being refreshed._  |
|  [**LayerObserver**](classLayerObserver.md) \* | [**layerObserver**](#function-layerobserver-22) () const<br>_Returns the currently used layer observer._  |
|  Q\_INVOKABLE void | [**projectCancelDownload**](#function-projectcanceldownload) (const QString & projectId) <br>_Cancels ongoing cloud project download with_ _projectId_ _._ |
|  Q\_INVOKABLE void | [**projectPackageAndDownload**](#function-projectpackageanddownload) (const QString & projectId) <br>_Downloads a cloud project with given_ _projectId_ _and all of its files._ |
|  Q\_INVOKABLE void | [**projectPush**](#function-projectpush) (const QString & projectId, const bool shouldDownloadUpdates) <br>_Pushes all local deltas for given_ _projectId_ _. If__shouldDownloadUpdates_ _is true, also calls_`downloadProject` _._ |
|  Q\_INVOKABLE void | [**refreshProjectDeltaList**](#function-refreshprojectdeltalist) (const QString & projectId) <br>_Retreives the delta list for a given_ _projectId_ _._ |
|  Q\_INVOKABLE void | [**refreshProjectsList**](#function-refreshprojectslist) (bool shouldResetModel=true, bool shouldFetchPublic=false, int projectFetchOffset=0) <br> |
|  Q\_INVOKABLE void | [**removeLocalProject**](#function-removelocalproject) (const QString & projectId) <br>_Remove local cloud project with given_ _projectId_ _from the device storage._ |
|  Q\_INVOKABLE bool | [**revertLocalChangesFromCurrentProject**](#function-revertlocalchangesfromcurrentproject) () <br>_Reverts the deltas of the current cloud project. The changes would applied in reverse order and opposite methods, e.g. "delete" becomes "create"._  |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br>_Returns the model role names._  |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br>_Returns number of rows._  |
|  void | [**setCloudConnection**](#function-setcloudconnection) ([**QFieldCloudConnection**](classQFieldCloudConnection.md) \* cloudConnection) <br>_Sets the cloud connection._  |
|  void | [**setCurrentProjectId**](#function-setcurrentprojectid) (const QString & currentProjectId) <br>_Sets the cloud project id of the currently opened project._  |
|  void | [**setGpkgFlusher**](#function-setgpkgflusher) ([**QgsGpkgFlusher**](classQgsGpkgFlusher.md) \* flusher) <br>_Sets the geopackage flusher._  |
|  void | [**setLayerObserver**](#function-setlayerobserver) ([**LayerObserver**](classLayerObserver.md) \* layerObserver) <br>_Sets the layer observer._  |
|  Q\_INVOKABLE void | [**updateLocalizedDataPaths**](#function-updatelocalizeddatapaths) (const QString & projectPath) <br>_Configure localized data paths for cloud projects when available._  |




























## Public Types Documentation




### enum ColumnRole 

```C++
enum QFieldCloudProjectsModel::ColumnRole {
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
    DeltaListRole
};
```




<hr>



### enum ProjectsRequestAttribute 

_Attributes controlling fetching of projects._ 
```C++
enum QFieldCloudProjectsModel::ProjectsRequestAttribute {
    FetchPublicProjects = QNetworkRequest::User + 1,
    ProjectsFetchOffset = QNetworkRequest::User + 2,
    ResetModel = QNetworkRequest::User + 3,
    ProjectId = QNetworkRequest::User + 4
};
```




<hr>
## Public Properties Documentation




### property busyProjectIds [1/2]

_Currently busy project ids._ 
```C++
QSet<QString> QFieldCloudProjectsModel::busyProjectIds;
```




<hr>



### property cloudConnection [1/2]

_The current cloud connection._ 
```C++
QFieldCloudConnection * QFieldCloudProjectsModel::cloudConnection;
```




<hr>



### property currentProject [1/2]

_The current cloud project. (null for non-cloud projects)._ 
```C++
QFieldCloudProject * QFieldCloudProjectsModel::currentProject;
```




<hr>



### property currentProjectId [1/2]

_The current cloud project id of the currently opened project (empty string for non-cloud projects)._ 
```C++
QString QFieldCloudProjectsModel::currentProjectId;
```




<hr>



### property gpkgFlusher [1/2]

_The current geopackage flusher._ 
```C++
QgsGpkgFlusher * QFieldCloudProjectsModel::gpkgFlusher;
```




<hr>



### property isCreating [1/2]

_Returns TRUE whether the model is creating a project._ 
```C++
bool QFieldCloudProjectsModel::isCreating;
```




<hr>



### property isRefreshing [1/2]

_Returns TRUE whether the model is being refreshed._ 
```C++
bool QFieldCloudProjectsModel::isRefreshing;
```




<hr>



### property layerObserver [1/2]

_The current layer observer._ 
```C++
LayerObserver * QFieldCloudProjectsModel::layerObserver;
```




<hr>
## Public Signals Documentation




### signal busyProjectIdsChanged 

```C++
void QFieldCloudProjectsModel::busyProjectIdsChanged;
```




<hr>



### signal cloudConnectionChanged 

```C++
void QFieldCloudProjectsModel::cloudConnectionChanged;
```




<hr>



### signal currentProjectChanged 

```C++
void QFieldCloudProjectsModel::currentProjectChanged;
```




<hr>



### signal currentProjectIdChanged 

```C++
void QFieldCloudProjectsModel::currentProjectIdChanged;
```




<hr>



### signal deltaListModelChanged 

```C++
void QFieldCloudProjectsModel::deltaListModelChanged;
```




<hr>



### signal gpkgFlusherChanged 

```C++
void QFieldCloudProjectsModel::gpkgFlusherChanged;
```




<hr>



### signal isCreatingChanged 

```C++
void QFieldCloudProjectsModel::isCreatingChanged;
```




<hr>



### signal isRefreshingChanged 

```C++
void QFieldCloudProjectsModel::isRefreshingChanged;
```




<hr>



### signal layerObserverChanged 

```C++
void QFieldCloudProjectsModel::layerObserverChanged;
```




<hr>



### signal projectAppended 

```C++
void QFieldCloudProjectsModel::projectAppended;
```




<hr>



### signal projectCreated 

```C++
void QFieldCloudProjectsModel::projectCreated;
```




<hr>



### signal projectDownloaded 

```C++
void QFieldCloudProjectsModel::projectDownloaded;
```




<hr>



### signal projectUploaded 

```C++
void QFieldCloudProjectsModel::projectUploaded;
```




<hr>



### signal pushFinished 

```C++
void QFieldCloudProjectsModel::pushFinished;
```




<hr>



### signal warning 

```C++
void QFieldCloudProjectsModel::warning;
```




<hr>
## Public Functions Documentation




### function QFieldCloudProjectsModel 

```C++
QFieldCloudProjectsModel::QFieldCloudProjectsModel () 
```




<hr>



### function appendProject 

_Fetches a cloud project for a given_ _projectId_ _and appends it to the model._
```C++
Q_INVOKABLE void QFieldCloudProjectsModel::appendProject (
    const QString & projectId
) 
```




<hr>



### function busyProjectIds [2/2]

_Returns a set containing the currently busy project ids._ 
```C++
QSet< QString > QFieldCloudProjectsModel::busyProjectIds () const
```




<hr>



### function cloudConnection [2/2]

_Returns the currently used cloud connection._ 
```C++
QFieldCloudConnection * QFieldCloudProjectsModel::cloudConnection () const
```




<hr>



### function createProject 

```C++
Q_INVOKABLE void QFieldCloudProjectsModel::createProject (
    const QString name
) 
```



Transform a locally-stored project into a cloud project by uploading its content to the QFieldCloud server.


The converted project will then be removed from the local storage in favor of a newly packaged cloud project downloaded from the server. 


        

<hr>



### function currentProject [2/2]

_Returns the cloud project of the currently oepened project or NULL for non-cloud projects._ 
```C++
QFieldCloudProject * QFieldCloudProjectsModel::currentProject () const
```




<hr>



### function currentProjectId [2/2]

_Returns the cloud project id of the currently opened project._ 
```C++
QString QFieldCloudProjectsModel::currentProjectId () const
```




<hr>



### function data 

_Returns the data at given_ _index_ _with given__role_ _._
```C++
QVariant QFieldCloudProjectsModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function discardLocalChangesFromCurrentProject 

_Discards the delta records of the current cloud project._ 
```C++
Q_INVOKABLE bool QFieldCloudProjectsModel::discardLocalChangesFromCurrentProject () 
```




<hr>



### function findProject 

_Return the cloud project for a given_ _projectId_ _._
```C++
Q_INVOKABLE QFieldCloudProject * QFieldCloudProjectsModel::findProject (
    const QString & projectId
) const
```




<hr>



### function gpkgFlusher [2/2]

_Returns the geopackage flusher._ 
```C++
inline QgsGpkgFlusher * QFieldCloudProjectsModel::gpkgFlusher () const
```




<hr>



### function isCreating [2/2]

_Returns TRUE whether the model is being refreshed._ 
```C++
inline bool QFieldCloudProjectsModel::isCreating () const
```




<hr>



### function isRefreshing [2/2]

_Returns TRUE whether the model is being refreshed._ 
```C++
inline bool QFieldCloudProjectsModel::isRefreshing () const
```




<hr>



### function layerObserver [2/2]

_Returns the currently used layer observer._ 
```C++
LayerObserver * QFieldCloudProjectsModel::layerObserver () const
```




<hr>



### function projectCancelDownload 

_Cancels ongoing cloud project download with_ _projectId_ _._
```C++
Q_INVOKABLE void QFieldCloudProjectsModel::projectCancelDownload (
    const QString & projectId
) 
```




<hr>



### function projectPackageAndDownload 

_Downloads a cloud project with given_ _projectId_ _and all of its files._
```C++
Q_INVOKABLE void QFieldCloudProjectsModel::projectPackageAndDownload (
    const QString & projectId
) 
```




<hr>



### function projectPush 

_Pushes all local deltas for given_ _projectId_ _. If__shouldDownloadUpdates_ _is true, also calls_`downloadProject` _._
```C++
Q_INVOKABLE void QFieldCloudProjectsModel::projectPush (
    const QString & projectId,
    const bool shouldDownloadUpdates
) 
```




<hr>



### function refreshProjectDeltaList 

_Retreives the delta list for a given_ _projectId_ _._
```C++
Q_INVOKABLE void QFieldCloudProjectsModel::refreshProjectDeltaList (
    const QString & projectId
) 
```




<hr>



### function refreshProjectsList 

```C++
Q_INVOKABLE void QFieldCloudProjectsModel::refreshProjectsList (
    bool shouldResetModel=true,
    bool shouldFetchPublic=false,
    int projectFetchOffset=0
) 
```



Requests the cloud projects list from the server. 

**Parameters:**


* `shouldResetModel` set to TRUE to reset the model 
* `shouldFetchPublic` set to TRUE to refresh public projects 
* `projectFetchOffset` offset for pagination 




        

<hr>



### function removeLocalProject 

_Remove local cloud project with given_ _projectId_ _from the device storage._
```C++
Q_INVOKABLE void QFieldCloudProjectsModel::removeLocalProject (
    const QString & projectId
) 
```




<hr>



### function revertLocalChangesFromCurrentProject 

_Reverts the deltas of the current cloud project. The changes would applied in reverse order and opposite methods, e.g. "delete" becomes "create"._ 
```C++
Q_INVOKABLE bool QFieldCloudProjectsModel::revertLocalChangesFromCurrentProject () 
```




<hr>



### function roleNames 

_Returns the model role names._ 
```C++
QHash< int, QByteArray > QFieldCloudProjectsModel::roleNames () override const
```




<hr>



### function rowCount 

_Returns number of rows._ 
```C++
int QFieldCloudProjectsModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function setCloudConnection 

_Sets the cloud connection._ 
```C++
void QFieldCloudProjectsModel::setCloudConnection (
    QFieldCloudConnection * cloudConnection
) 
```




<hr>



### function setCurrentProjectId 

_Sets the cloud project id of the currently opened project._ 
```C++
void QFieldCloudProjectsModel::setCurrentProjectId (
    const QString & currentProjectId
) 
```




<hr>



### function setGpkgFlusher 

_Sets the geopackage flusher._ 
```C++
void QFieldCloudProjectsModel::setGpkgFlusher (
    QgsGpkgFlusher * flusher
) 
```




<hr>



### function setLayerObserver 

_Sets the layer observer._ 
```C++
void QFieldCloudProjectsModel::setLayerObserver (
    LayerObserver * layerObserver
) 
```




<hr>



### function updateLocalizedDataPaths 

_Configure localized data paths for cloud projects when available._ 
```C++
Q_INVOKABLE void QFieldCloudProjectsModel::updateLocalizedDataPaths (
    const QString & projectPath
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfieldcloudprojectsmodel.h`

