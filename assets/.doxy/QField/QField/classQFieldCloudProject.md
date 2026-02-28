

# Class QFieldCloudProject



[**ClassList**](annotated.md) **>** [**QFieldCloudProject**](classQFieldCloudProject.md)








Inherits the following classes: QObject












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**FileTransfer**](structQFieldCloudProject_1_1FileTransfer.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**DeltaFileStatus**](#enum-deltafilestatus)  <br>_The status of the running server job for applying deltas on a project._  |
| enum  | [**JobStatus**](#enum-jobstatus)  <br>_The status of the running server job._  |
| enum  | [**JobType**](#enum-jobtype)  <br>_The status of the running server job._  |
| enum  | [**PackagingStatus**](#enum-packagingstatus)  <br>_The status of the running server job for packaging a project._  |
| enum  | [**ProjectCheckout**](#enum-projectcheckout)  <br>_Whether the project has been available locally and/or remotely._  |
| enum  | [**ProjectErrorStatus**](#enum-projecterrorstatus)  <br>_Whether the project has experienced an error._  |
| enum  | [**ProjectModification**](#enum-projectmodification)  <br>_Whether the project has no or local and/or remote modification. Indicates wheter can be synced._  |
| enum  | [**ProjectRefreshReason**](#enum-projectrefreshreason)  <br>_The reason why projectRefreshData was called._  |
| enum  | [**ProjectStatus**](#enum-projectstatus)  <br>_Whether the project is busy or idle._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**attachmentsOnDemandEnabled**](classQFieldCloudProject.md#property-attachmentsondemandenabled-12)  <br> |
| property bool | [**autoPushEnabled**](classQFieldCloudProject.md#property-autopushenabled-12)  <br> |
| property int | [**autoPushIntervalMins**](classQFieldCloudProject.md#property-autopushintervalmins-12)  <br> |
| property QDateTime | [**createdAt**](classQFieldCloudProject.md#property-createdat-12)  <br> |
| property [**DeltaFileStatus**](classQFieldCloudProject.md#enum-deltafilestatus) | [**deltaFilePushStatus**](classQFieldCloudProject.md#property-deltafilepushstatus-12)  <br> |
| property [**DeltaFileWrapper**](classDeltaFileWrapper.md) \* | [**deltaFileWrapper**](classQFieldCloudProject.md#property-deltafilewrapper-12)  <br> |
| property [**DeltaListModel**](classDeltaListModel.md) \* | [**deltaListModel**](classQFieldCloudProject.md#property-deltalistmodel-12)  <br> |
| property int | [**deltasCount**](classQFieldCloudProject.md#property-deltascount-12)  <br> |
| property QString | [**description**](classQFieldCloudProject.md#property-description-12)  <br> |
| property qint64 | [**downloadBytesReceived**](classQFieldCloudProject.md#property-downloadbytesreceived-12)  <br> |
| property qint64 | [**downloadBytesTotal**](classQFieldCloudProject.md#property-downloadbytestotal-12)  <br> |
| property double | [**downloadProgress**](classQFieldCloudProject.md#property-downloadprogress-12)  <br> |
| property bool | [**forceAutoPush**](classQFieldCloudProject.md#property-forceautopush-12)  <br> |
| property QString | [**id**](classQFieldCloudProject.md#property-id-12)  <br> |
| property bool | [**isFeatured**](classQFieldCloudProject.md#property-isfeatured-12)  <br> |
| property bool | [**isOutdated**](classQFieldCloudProject.md#property-isoutdated-12)  <br> |
| property bool | [**isProjectOutdated**](classQFieldCloudProject.md#property-isprojectoutdated-12)  <br> |
| property bool | [**isPublic**](classQFieldCloudProject.md#property-ispublic-12)  <br> |
| property QString | [**lastLocalExportedAt**](classQFieldCloudProject.md#property-lastlocalexportedat-12)  <br> |
| property QString | [**lastLocalPushDeltas**](classQFieldCloudProject.md#property-lastlocalpushdeltas-12)  <br> |
| property QString | [**localPath**](classQFieldCloudProject.md#property-localpath-12)  <br> |
| property QString | [**name**](classQFieldCloudProject.md#property-name-12)  <br> |
| property QString | [**owner**](classQFieldCloudProject.md#property-owner-12)  <br> |
| property QStringList | [**packagedLayerErrors**](classQFieldCloudProject.md#property-packagedlayererrors-12)  <br> |
| property [**PackagingStatus**](classQFieldCloudProject.md#enum-packagingstatus) | [**packagingStatus**](classQFieldCloudProject.md#property-packagingstatus-12)  <br> |
| property double | [**pushDeltaProgress**](classQFieldCloudProject.md#property-pushdeltaprogress-12)  <br> |
| property [**ProjectStatus**](classQFieldCloudProject.md#enum-projectstatus) | [**status**](classQFieldCloudProject.md#property-status-12)  <br> |
| property QString | [**thumbnailPath**](classQFieldCloudProject.md#property-thumbnailpath-12)  <br> |
| property QDateTime | [**updatedAt**](classQFieldCloudProject.md#property-updatedat-12)  <br> |
| property qint64 | [**uploadBytesSent**](classQFieldCloudProject.md#property-uploadbytessent-12)  <br> |
| property qint64 | [**uploadBytesTotal**](classQFieldCloudProject.md#property-uploadbytestotal-12)  <br> |
| property double | [**uploadProgress**](classQFieldCloudProject.md#property-uploadprogress-12)  <br> |
| property QString | [**userRole**](classQFieldCloudProject.md#property-userrole-12)  <br> |
| property QString | [**userRoleOrigin**](classQFieldCloudProject.md#property-userroleorigin-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**attachmentsOnDemandEnabledChanged**](classQFieldCloudProject.md#signal-attachmentsondemandenabledchanged)  <br> |
| signal void | [**autoPushEnabledChanged**](classQFieldCloudProject.md#signal-autopushenabledchanged)  <br> |
| signal void | [**autoPushIntervalMinsChanged**](classQFieldCloudProject.md#signal-autopushintervalminschanged)  <br> |
| signal void | [**canRepackageChanged**](classQFieldCloudProject.md#signal-canrepackagechanged)  <br> |
| signal void | [**checkoutChanged**](classQFieldCloudProject.md#signal-checkoutchanged)  <br> |
| signal void | [**createdAtChanged**](classQFieldCloudProject.md#signal-createdatchanged)  <br> |
| signal void | [**dataLastUpdatedAtChanged**](classQFieldCloudProject.md#signal-datalastupdatedatchanged)  <br> |
| signal void | [**dataRefreshed**](classQFieldCloudProject.md#signal-datarefreshed) ([**ProjectRefreshReason**](classQFieldCloudProject.md#enum-projectrefreshreason) reason, const QString & error=QString()) <br> |
| signal void | [**deltaFileIdChanged**](classQFieldCloudProject.md#signal-deltafileidchanged)  <br> |
| signal void | [**deltaFilePushStatusChanged**](classQFieldCloudProject.md#signal-deltafilepushstatuschanged)  <br> |
| signal void | [**deltaFilePushStatusStringChanged**](classQFieldCloudProject.md#signal-deltafilepushstatusstringchanged)  <br> |
| signal void | [**deltaFileWrapperChanged**](classQFieldCloudProject.md#signal-deltafilewrapperchanged)  <br> |
| signal void | [**deltaLayersToDownloadChanged**](classQFieldCloudProject.md#signal-deltalayerstodownloadchanged)  <br> |
| signal void | [**deltaListModelChanged**](classQFieldCloudProject.md#signal-deltalistmodelchanged)  <br> |
| signal void | [**deltasCountChanged**](classQFieldCloudProject.md#signal-deltascountchanged)  <br> |
| signal void | [**descriptionChanged**](classQFieldCloudProject.md#signal-descriptionchanged)  <br> |
| signal void | [**downloadAttachmentFinished**](classQFieldCloudProject.md#signal-downloadattachmentfinished) (const QString & fileName, const QString & error=QString()) <br> |
| signal void | [**downloadBytesReceivedChanged**](classQFieldCloudProject.md#signal-downloadbytesreceivedchanged)  <br> |
| signal void | [**downloadBytesTotalChanged**](classQFieldCloudProject.md#signal-downloadbytestotalchanged)  <br> |
| signal void | [**downloadFinished**](classQFieldCloudProject.md#signal-downloadfinished) (const QString & error=QString()) <br> |
| signal void | [**downloadProgressChanged**](classQFieldCloudProject.md#signal-downloadprogresschanged)  <br> |
| signal void | [**downloaded**](classQFieldCloudProject.md#signal-downloaded) (const QString & name, const QString & error=QString()) <br> |
| signal void | [**errorStatusChanged**](classQFieldCloudProject.md#signal-errorstatuschanged)  <br> |
| signal void | [**forceAutoPushChanged**](classQFieldCloudProject.md#signal-forceautopushchanged)  <br> |
| signal void | [**idChanged**](classQFieldCloudProject.md#signal-idchanged)  <br> |
| signal void | [**isFeaturedChanged**](classQFieldCloudProject.md#signal-isfeaturedchanged)  <br> |
| signal void | [**isOutdatedChanged**](classQFieldCloudProject.md#signal-isoutdatedchanged)  <br> |
| signal void | [**isPackagingActiveChanged**](classQFieldCloudProject.md#signal-ispackagingactivechanged)  <br> |
| signal void | [**isPackagingFailedChanged**](classQFieldCloudProject.md#signal-ispackagingfailedchanged)  <br> |
| signal void | [**isProjectOutdatedChanged**](classQFieldCloudProject.md#signal-isprojectoutdatedchanged)  <br> |
| signal void | [**isPublicChanged**](classQFieldCloudProject.md#signal-ispublicchanged)  <br> |
| signal void | [**isSharedDatasetsProjectChanged**](classQFieldCloudProject.md#signal-isshareddatasetsprojectchanged)  <br> |
| signal void | [**jobFinished**](classQFieldCloudProject.md#signal-jobfinished) ([**JobType**](classQFieldCloudProject.md#enum-jobtype) type, const QString & error=QString()) <br> |
| signal void | [**lastDataLastUpdatedAtChanged**](classQFieldCloudProject.md#signal-lastdatalastupdatedatchanged)  <br> |
| signal void | [**lastLocalDataLastUpdatedAtChanged**](classQFieldCloudProject.md#signal-lastlocaldatalastupdatedatchanged)  <br> |
| signal void | [**lastLocalExportIdChanged**](classQFieldCloudProject.md#signal-lastlocalexportidchanged)  <br> |
| signal void | [**lastLocalExportedAtChanged**](classQFieldCloudProject.md#signal-lastlocalexportedatchanged)  <br> |
| signal void | [**lastLocalPushDeltasChanged**](classQFieldCloudProject.md#signal-lastlocalpushdeltaschanged)  <br> |
| signal void | [**lastLocalRestrictedDataLastUpdatedAtChanged**](classQFieldCloudProject.md#signal-lastlocalrestricteddatalastupdatedatchanged)  <br> |
| signal void | [**lastRefreshedAtChanged**](classQFieldCloudProject.md#signal-lastrefreshedatchanged)  <br> |
| signal void | [**localPathChanged**](classQFieldCloudProject.md#signal-localpathchanged)  <br> |
| signal void | [**modificationChanged**](classQFieldCloudProject.md#signal-modificationchanged)  <br> |
| signal void | [**nameChanged**](classQFieldCloudProject.md#signal-namechanged)  <br> |
| signal void | [**needsRepackagingChanged**](classQFieldCloudProject.md#signal-needsrepackagingchanged)  <br> |
| signal void | [**networkDeltaPushed**](classQFieldCloudProject.md#signal-networkdeltapushed)  <br> |
| signal void | [**networkDeltaStatusChecked**](classQFieldCloudProject.md#signal-networkdeltastatuschecked)  <br> |
| signal void | [**ownerChanged**](classQFieldCloudProject.md#signal-ownerchanged)  <br> |
| signal void | [**packagedLayerErrorsChanged**](classQFieldCloudProject.md#signal-packagedlayererrorschanged)  <br> |
| signal void | [**packagingStatusChanged**](classQFieldCloudProject.md#signal-packagingstatuschanged)  <br> |
| signal void | [**packagingStatusStringChanged**](classQFieldCloudProject.md#signal-packagingstatusstringchanged)  <br> |
| signal void | [**pushDeltaProgressChanged**](classQFieldCloudProject.md#signal-pushdeltaprogresschanged)  <br> |
| signal void | [**pushFinished**](classQFieldCloudProject.md#signal-pushfinished) (bool isDownloading, const QString & error=QString()) <br> |
| signal void | [**restrictedDataLastUpdatedAtChanged**](classQFieldCloudProject.md#signal-restricteddatalastupdatedatchanged)  <br> |
| signal void | [**sharedDatasetsProjectIdChanged**](classQFieldCloudProject.md#signal-shareddatasetsprojectidchanged)  <br> |
| signal void | [**statusChanged**](classQFieldCloudProject.md#signal-statuschanged)  <br> |
| signal void | [**thumbnailPathChanged**](classQFieldCloudProject.md#signal-thumbnailpathchanged)  <br> |
| signal void | [**updatedAtChanged**](classQFieldCloudProject.md#signal-updatedatchanged)  <br> |
| signal void | [**uploadBytesSentChanged**](classQFieldCloudProject.md#signal-uploadbytessentchanged)  <br> |
| signal void | [**uploadBytesTotalChanged**](classQFieldCloudProject.md#signal-uploadbytestotalchanged)  <br> |
| signal void | [**uploadFinished**](classQFieldCloudProject.md#signal-uploadfinished) (const QString & error=QString()) <br> |
| signal void | [**uploadProgressChanged**](classQFieldCloudProject.md#signal-uploadprogresschanged)  <br> |
| signal void | [**userRoleChanged**](classQFieldCloudProject.md#signal-userrolechanged)  <br> |
| signal void | [**userRoleOriginChanged**](classQFieldCloudProject.md#signal-userroleoriginchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QFieldCloudProject**](#function-qfieldcloudproject) (const QString & id=QString(), [**QFieldCloudConnection**](classQFieldCloudConnection.md) \* connection=nullptr, [**QgsGpkgFlusher**](classQgsGpkgFlusher.md) \* gpkgFlusher=nullptr) <br> |
|  bool | [**attachmentsOnDemandEnabled**](#function-attachmentsondemandenabled-22) () const<br> |
|  bool | [**autoPushEnabled**](#function-autopushenabled-22) () const<br> |
|  int | [**autoPushIntervalMins**](#function-autopushintervalmins-22) () const<br> |
|  bool | [**canRepackage**](#function-canrepackage) () const<br> |
|  void | [**cancelDownload**](#function-canceldownload) () <br> |
|  void | [**cancelPush**](#function-cancelpush) () <br> |
|  ProjectCheckouts | [**checkout**](#function-checkout) () const<br> |
|  QDateTime | [**createdAt**](#function-createdat-22) () const<br> |
|  QDateTime | [**dataLastUpdatedAt**](#function-datalastupdatedat) () const<br> |
|  QString | [**deltaFileId**](#function-deltafileid) () const<br> |
|  [**DeltaFileStatus**](classQFieldCloudProject.md#enum-deltafilestatus) | [**deltaFilePushStatus**](#function-deltafilepushstatus-22) () const<br> |
|  QString | [**deltaFilePushStatusString**](#function-deltafilepushstatusstring) () const<br> |
|  [**DeltaFileWrapper**](classDeltaFileWrapper.md) \* | [**deltaFileWrapper**](#function-deltafilewrapper-22) () const<br> |
|  QStringList | [**deltaLayersToDownload**](#function-deltalayerstodownload) () const<br> |
|  [**DeltaListModel**](classDeltaListModel.md) \* | [**deltaListModel**](#function-deltalistmodel-22) () const<br> |
|  int | [**deltasCount**](#function-deltascount-22) () const<br> |
|  QString | [**description**](#function-description-22) () const<br> |
|  Q\_INVOKABLE void | [**downloadAttachment**](#function-downloadattachment) (const QString & fileName) <br> |
|  qint64 | [**downloadBytesReceived**](#function-downloadbytesreceived-22) () const<br> |
|  qint64 | [**downloadBytesTotal**](#function-downloadbytestotal-22) () const<br> |
|  double | [**downloadProgress**](#function-downloadprogress-22) () const<br> |
|  Q\_INVOKABLE void | [**downloadThumbnail**](#function-downloadthumbnail) () <br> |
|  [**ProjectErrorStatus**](classQFieldCloudProject.md#enum-projecterrorstatus) | [**errorStatus**](#function-errorstatus) () const<br> |
|  bool | [**forceAutoPush**](#function-forceautopush-22) () const<br> |
|  QString | [**id**](#function-id-22) () const<br> |
|  bool | [**isFeatured**](#function-isfeatured-22) () const<br> |
|  bool | [**isOutdated**](#function-isoutdated-22) () const<br> |
|  bool | [**isPackagingActive**](#function-ispackagingactive) () const<br> |
|  bool | [**isPackagingFailed**](#function-ispackagingfailed) () const<br> |
|  bool | [**isProjectOutdated**](#function-isprojectoutdated-22) () const<br> |
|  bool | [**isPublic**](#function-ispublic-22) () const<br> |
|  bool | [**isSharedDatasetsProject**](#function-isshareddatasetsproject) () const<br> |
|  QDateTime | [**lastLocalDataLastUpdatedAt**](#function-lastlocaldatalastupdatedat) () const<br> |
|  QString | [**lastLocalExportId**](#function-lastlocalexportid) () const<br> |
|  QString | [**lastLocalExportedAt**](#function-lastlocalexportedat-22) () const<br> |
|  QString | [**lastLocalPushDeltas**](#function-lastlocalpushdeltas-22) () const<br> |
|  QDateTime | [**lastLocalRestrictedDataLastUpdatedAt**](#function-lastlocalrestricteddatalastupdatedat) () const<br> |
|  QDateTime | [**lastRefreshedAt**](#function-lastrefreshedat) () const<br> |
|  QString | [**localPath**](#function-localpath-22) () const<br> |
|  ProjectModifications | [**modification**](#function-modification) () const<br> |
|  QString | [**name**](#function-name-22) () const<br> |
|  bool | [**needsRepackaging**](#function-needsrepackaging) () const<br> |
|  QString | [**owner**](#function-owner-22) () const<br> |
|  Q\_INVOKABLE void | [**packageAndDownload**](#function-packageanddownload) () <br> |
|  QStringList | [**packagedLayerErrors**](#function-packagedlayererrors-22) () const<br> |
|  [**PackagingStatus**](classQFieldCloudProject.md#enum-packagingstatus) | [**packagingStatus**](#function-packagingstatus-22) () const<br> |
|  QString | [**packagingStatusString**](#function-packagingstatusstring) () const<br> |
|  Q\_INVOKABLE void | [**push**](#function-push) (bool shouldDownloadUpdates) <br> |
|  double | [**pushDeltaProgress**](#function-pushdeltaprogress-22) () const<br> |
|  void | [**refreshDeltaList**](#function-refreshdeltalist) () <br> |
|  void | [**refreshModification**](#function-refreshmodification) () <br> |
|  void | [**removeLocally**](#function-removelocally) () <br> |
|  QDateTime | [**restrictedDataLastUpdatedAt**](#function-restricteddatalastupdatedat) () const<br> |
|  void | [**setAttachmentsOnDemandEnabled**](#function-setattachmentsondemandenabled) (bool enabled) <br> |
|  void | [**setAutoPushEnabled**](#function-setautopushenabled) (bool enabled) <br> |
|  void | [**setAutoPushIntervalMins**](#function-setautopushintervalmins) (int minutes) <br> |
|  void | [**setCanRepackage**](#function-setcanrepackage) (bool canRepackage) <br> |
|  void | [**setCheckout**](#function-setcheckout) (ProjectCheckouts checkout) <br> |
|  void | [**setCreatedAt**](#function-setcreatedat) (const QDateTime & createdAt) <br> |
|  void | [**setDataLastUpdatedAt**](#function-setdatalastupdatedat) (const QDateTime & dataLastUpdatedAt) <br> |
|  void | [**setDeltaFileId**](#function-setdeltafileid) (const QString & deltaFileId) <br> |
|  void | [**setDeltaFilePushStatus**](#function-setdeltafilepushstatus) ([**DeltaFileStatus**](classQFieldCloudProject.md#enum-deltafilestatus) deltaFilePushStatus) <br> |
|  void | [**setDeltaFilePushStatusString**](#function-setdeltafilepushstatusstring) (const QString & deltaFilePushStatusString) <br> |
|  void | [**setDeltaLayersToDownload**](#function-setdeltalayerstodownload) (const QStringList & deltaLayersToDownload) <br> |
|  void | [**setDescription**](#function-setdescription) (const QString & description) <br> |
|  void | [**setErrorStatus**](#function-seterrorstatus) ([**ProjectErrorStatus**](classQFieldCloudProject.md#enum-projecterrorstatus) errorStatus) <br> |
|  void | [**setForceAutoPush**](#function-setforceautopush) (bool force) <br> |
|  void | [**setIsFeatured**](#function-setisfeatured) (bool isFeatured) <br> |
|  void | [**setIsOutdated**](#function-setisoutdated) (bool isOutdated) <br> |
|  void | [**setIsPackagingActive**](#function-setispackagingactive) (bool isPackagingActive) <br> |
|  void | [**setIsPackagingFailed**](#function-setispackagingfailed) (bool isPackagingFailed) <br> |
|  void | [**setIsProjectOutdated**](#function-setisprojectoutdated) (bool isProjectOutdated) <br> |
|  void | [**setIsPublic**](#function-setispublic) (bool isPublic) <br> |
|  void | [**setIsSharedDatasetsProject**](#function-setisshareddatasetsproject) (bool isSharedDatasetsProject) <br> |
|  void | [**setLastLocalDataLastUpdatedAt**](#function-setlastlocaldatalastupdatedat) (const QDateTime & lastLocalDataLastUpdatedAt) <br> |
|  void | [**setLastLocalExportId**](#function-setlastlocalexportid) (const QString & lastLocalExportId) <br> |
|  void | [**setLastLocalExportedAt**](#function-setlastlocalexportedat) (const QString & lastLocalExportedAt) <br> |
|  void | [**setLastLocalPushDeltas**](#function-setlastlocalpushdeltas) (const QString & lastLocalPushDeltas) <br> |
|  void | [**setLastLocalRestrictedDataLastUpdatedAt**](#function-setlastlocalrestricteddatalastupdatedat) (const QDateTime & lastLocalRestrictedDataLastUpdatedAt) <br> |
|  void | [**setLastRefreshedAt**](#function-setlastrefreshedat) (const QDateTime & lastRefreshedAt) <br> |
|  void | [**setLocalPath**](#function-setlocalpath) (const QString & localPath) <br> |
|  void | [**setModification**](#function-setmodification) ([**ProjectModification**](classQFieldCloudProject.md#enum-projectmodification) modification) <br> |
|  void | [**setName**](#function-setname) (const QString & name) <br> |
|  void | [**setNeedsRepackaging**](#function-setneedsrepackaging) (bool needsRepackaging) <br> |
|  void | [**setOwner**](#function-setowner) (const QString & owner) <br> |
|  void | [**setPackagedLayerErrors**](#function-setpackagedlayererrors) (const QStringList & packagedLayerErrors) <br> |
|  void | [**setPackagingStatus**](#function-setpackagingstatus) ([**PackagingStatus**](classQFieldCloudProject.md#enum-packagingstatus) packagingStatus) <br> |
|  void | [**setPackagingStatusString**](#function-setpackagingstatusstring) (const QString & packagingStatusString) <br> |
|  void | [**setRestrictedDataLastUpdatedAt**](#function-setrestricteddatalastupdatedat) (const QDateTime & restrictedDataLastUpdatedAt) <br> |
|  void | [**setSharedDatasetsProjectId**](#function-setshareddatasetsprojectid) (const QString & id) <br> |
|  void | [**setStatus**](#function-setstatus) ([**ProjectStatus**](classQFieldCloudProject.md#enum-projectstatus) status) <br> |
|  void | [**setThumbnailPath**](#function-setthumbnailpath) (const QString & thumbnailPath) <br> |
|  void | [**setUpdatedAt**](#function-setupdatedat) (const QDateTime & updatedAt) <br> |
|  void | [**setUserRole**](#function-setuserrole) (const QString & userRole) <br> |
|  void | [**setUserRoleOrigin**](#function-setuserroleorigin) (const QString & userRoleOrigin) <br> |
|  QString | [**sharedDatasetsProjectId**](#function-shareddatasetsprojectid) () const<br> |
|  [**ProjectStatus**](classQFieldCloudProject.md#enum-projectstatus) | [**status**](#function-status-22) () const<br> |
|  QString | [**thumbnailPath**](#function-thumbnailpath-22) () const<br> |
|  QDateTime | [**updatedAt**](#function-updatedat-22) () const<br> |
|  qint64 | [**uploadBytesSent**](#function-uploadbytessent-22) () const<br> |
|  qint64 | [**uploadBytesTotal**](#function-uploadbytestotal-22) () const<br> |
|  Q\_INVOKABLE void | [**uploadLocalPath**](#function-uploadlocalpath) (QString localPath, bool deleteAfterSuccessfulUpload=false) <br> |
|  double | [**uploadProgress**](#function-uploadprogress-22) () const<br> |
|  QString | [**userRole**](#function-userrole-22) () const<br> |
|  QString | [**userRoleOrigin**](#function-userroleorigin-22) () const<br> |
|   | [**~QFieldCloudProject**](#function-qfieldcloudproject) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**QFieldCloudProject**](classQFieldCloudProject.md) \* | [**fromDetails**](#function-fromdetails) (const QVariantHash & details, [**QFieldCloudConnection**](classQFieldCloudConnection.md) \* connection, [**QgsGpkgFlusher**](classQgsGpkgFlusher.md) \* gpkgFlusher=nullptr) <br> |
|  [**QFieldCloudProject**](classQFieldCloudProject.md) \* | [**fromLocalSettings**](#function-fromlocalsettings) (const QString & id, [**QFieldCloudConnection**](classQFieldCloudConnection.md) \* connection, [**QgsGpkgFlusher**](classQgsGpkgFlusher.md) \* gpkgFlusher=nullptr) <br> |
|  [**QFieldCloudProject::JobStatus**](classQFieldCloudProject.md#enum-jobstatus) | [**getJobStatusFromString**](#function-getjobstatusfromstring) (const QString & status) <br> |
|  QString | [**getJobTypeAsString**](#function-getjobtypeasstring) ([**QFieldCloudProject::JobType**](classQFieldCloudProject.md#enum-jobtype) jobType) <br> |
|  void | [**restoreLocalSettings**](#function-restorelocalsettings) ([**QFieldCloudProject**](classQFieldCloudProject.md) \* project, const QDir & localPath) <br> |


























## Public Types Documentation




### enum DeltaFileStatus 

_The status of the running server job for applying deltas on a project._ 
```C++
enum QFieldCloudProject::DeltaFileStatus {
    DeltaErrorStatus,
    DeltaLocalStatus,
    DeltaPendingStatus,
    DeltaBusyStatus,
    DeltaConflictStatus,
    DeltaNotAppliedStatus,
    DeltaAppliedStatus
};
```




<hr>



### enum JobStatus 

_The status of the running server job._ 
```C++
enum QFieldCloudProject::JobStatus {
    JobPendingStatus,
    JobQueuedStatus,
    JobStartedStatus,
    JobFinishedStatus,
    JobStoppedStatus,
    JobFailedStatus
};
```




<hr>



### enum JobType 

_The status of the running server job._ 
```C++
enum QFieldCloudProject::JobType {
    Package
};
```




<hr>



### enum PackagingStatus 

_The status of the running server job for packaging a project._ 
```C++
enum QFieldCloudProject::PackagingStatus {
    PackagingUnstartedStatus,
    PackagingErrorStatus,
    PackagingBusyStatus,
    PackagingFinishedStatus,
    PackagingAbortStatus
};
```




<hr>



### enum ProjectCheckout 

_Whether the project has been available locally and/or remotely._ 
```C++
enum QFieldCloudProject::ProjectCheckout {
    RemoteCheckout = 2 << 0,
    LocalCheckout = 2 << 1,
    LocalAndRemoteCheckout = RemoteCheckout | LocalCheckout
};
```




<hr>



### enum ProjectErrorStatus 

_Whether the project has experienced an error._ 
```C++
enum QFieldCloudProject::ProjectErrorStatus {
    NoErrorStatus,
    DownloadErrorStatus,
    PushErrorStatus
};
```




<hr>



### enum ProjectModification 

_Whether the project has no or local and/or remote modification. Indicates wheter can be synced._ 
```C++
enum QFieldCloudProject::ProjectModification {
    NoModification = 0,
    LocalModification = 2 << 0,
    RemoteModification = 2 << 1,
    LocalAndRemoteModification = RemoteModification | LocalModification
};
```




<hr>



### enum ProjectRefreshReason 

_The reason why projectRefreshData was called._ 
```C++
enum QFieldCloudProject::ProjectRefreshReason {
    Package,
    DeltaPushed
};
```




<hr>



### enum ProjectStatus 

_Whether the project is busy or idle._ 
```C++
enum QFieldCloudProject::ProjectStatus {
    Idle,
    Downloading,
    Pushing,
    Uploading,
    Failing
};
```




<hr>
## Public Properties Documentation




### property attachmentsOnDemandEnabled [1/2]

```C++
bool QFieldCloudProject::attachmentsOnDemandEnabled;
```




<hr>



### property autoPushEnabled [1/2]

```C++
bool QFieldCloudProject::autoPushEnabled;
```




<hr>



### property autoPushIntervalMins [1/2]

```C++
int QFieldCloudProject::autoPushIntervalMins;
```




<hr>



### property createdAt [1/2]

```C++
QDateTime QFieldCloudProject::createdAt;
```




<hr>



### property deltaFilePushStatus [1/2]

```C++
DeltaFileStatus QFieldCloudProject::deltaFilePushStatus;
```




<hr>



### property deltaFileWrapper [1/2]

```C++
DeltaFileWrapper * QFieldCloudProject::deltaFileWrapper;
```




<hr>



### property deltaListModel [1/2]

```C++
DeltaListModel * QFieldCloudProject::deltaListModel;
```




<hr>



### property deltasCount [1/2]

```C++
int QFieldCloudProject::deltasCount;
```




<hr>



### property description [1/2]

```C++
QString QFieldCloudProject::description;
```




<hr>



### property downloadBytesReceived [1/2]

```C++
qint64 QFieldCloudProject::downloadBytesReceived;
```




<hr>



### property downloadBytesTotal [1/2]

```C++
qint64 QFieldCloudProject::downloadBytesTotal;
```




<hr>



### property downloadProgress [1/2]

```C++
double QFieldCloudProject::downloadProgress;
```




<hr>



### property forceAutoPush [1/2]

```C++
bool QFieldCloudProject::forceAutoPush;
```




<hr>



### property id [1/2]

```C++
QString QFieldCloudProject::id;
```




<hr>



### property isFeatured [1/2]

```C++
bool QFieldCloudProject::isFeatured;
```




<hr>



### property isOutdated [1/2]

```C++
bool QFieldCloudProject::isOutdated;
```




<hr>



### property isProjectOutdated [1/2]

```C++
bool QFieldCloudProject::isProjectOutdated;
```




<hr>



### property isPublic [1/2]

```C++
bool QFieldCloudProject::isPublic;
```




<hr>



### property lastLocalExportedAt [1/2]

```C++
QString QFieldCloudProject::lastLocalExportedAt;
```




<hr>



### property lastLocalPushDeltas [1/2]

```C++
QString QFieldCloudProject::lastLocalPushDeltas;
```




<hr>



### property localPath [1/2]

```C++
QString QFieldCloudProject::localPath;
```




<hr>



### property name [1/2]

```C++
QString QFieldCloudProject::name;
```




<hr>



### property owner [1/2]

```C++
QString QFieldCloudProject::owner;
```




<hr>



### property packagedLayerErrors [1/2]

```C++
QStringList QFieldCloudProject::packagedLayerErrors;
```




<hr>



### property packagingStatus [1/2]

```C++
PackagingStatus QFieldCloudProject::packagingStatus;
```




<hr>



### property pushDeltaProgress [1/2]

```C++
double QFieldCloudProject::pushDeltaProgress;
```




<hr>



### property status [1/2]

```C++
ProjectStatus QFieldCloudProject::status;
```




<hr>



### property thumbnailPath [1/2]

```C++
QString QFieldCloudProject::thumbnailPath;
```




<hr>



### property updatedAt [1/2]

```C++
QDateTime QFieldCloudProject::updatedAt;
```




<hr>



### property uploadBytesSent [1/2]

```C++
qint64 QFieldCloudProject::uploadBytesSent;
```




<hr>



### property uploadBytesTotal [1/2]

```C++
qint64 QFieldCloudProject::uploadBytesTotal;
```




<hr>



### property uploadProgress [1/2]

```C++
double QFieldCloudProject::uploadProgress;
```




<hr>



### property userRole [1/2]

```C++
QString QFieldCloudProject::userRole;
```




<hr>



### property userRoleOrigin [1/2]

```C++
QString QFieldCloudProject::userRoleOrigin;
```




<hr>
## Public Signals Documentation




### signal attachmentsOnDemandEnabledChanged 

```C++
void QFieldCloudProject::attachmentsOnDemandEnabledChanged;
```




<hr>



### signal autoPushEnabledChanged 

```C++
void QFieldCloudProject::autoPushEnabledChanged;
```




<hr>



### signal autoPushIntervalMinsChanged 

```C++
void QFieldCloudProject::autoPushIntervalMinsChanged;
```




<hr>



### signal canRepackageChanged 

```C++
void QFieldCloudProject::canRepackageChanged;
```




<hr>



### signal checkoutChanged 

```C++
void QFieldCloudProject::checkoutChanged;
```




<hr>



### signal createdAtChanged 

```C++
void QFieldCloudProject::createdAtChanged;
```




<hr>



### signal dataLastUpdatedAtChanged 

```C++
void QFieldCloudProject::dataLastUpdatedAtChanged;
```




<hr>



### signal dataRefreshed 

```C++
void QFieldCloudProject::dataRefreshed;
```




<hr>



### signal deltaFileIdChanged 

```C++
void QFieldCloudProject::deltaFileIdChanged;
```




<hr>



### signal deltaFilePushStatusChanged 

```C++
void QFieldCloudProject::deltaFilePushStatusChanged;
```




<hr>



### signal deltaFilePushStatusStringChanged 

```C++
void QFieldCloudProject::deltaFilePushStatusStringChanged;
```




<hr>



### signal deltaFileWrapperChanged 

```C++
void QFieldCloudProject::deltaFileWrapperChanged;
```




<hr>



### signal deltaLayersToDownloadChanged 

```C++
void QFieldCloudProject::deltaLayersToDownloadChanged;
```




<hr>



### signal deltaListModelChanged 

```C++
void QFieldCloudProject::deltaListModelChanged;
```




<hr>



### signal deltasCountChanged 

```C++
void QFieldCloudProject::deltasCountChanged;
```




<hr>



### signal descriptionChanged 

```C++
void QFieldCloudProject::descriptionChanged;
```




<hr>



### signal downloadAttachmentFinished 

```C++
void QFieldCloudProject::downloadAttachmentFinished;
```




<hr>



### signal downloadBytesReceivedChanged 

```C++
void QFieldCloudProject::downloadBytesReceivedChanged;
```




<hr>



### signal downloadBytesTotalChanged 

```C++
void QFieldCloudProject::downloadBytesTotalChanged;
```




<hr>



### signal downloadFinished 

```C++
void QFieldCloudProject::downloadFinished;
```




<hr>



### signal downloadProgressChanged 

```C++
void QFieldCloudProject::downloadProgressChanged;
```




<hr>



### signal downloaded 

```C++
void QFieldCloudProject::downloaded;
```




<hr>



### signal errorStatusChanged 

```C++
void QFieldCloudProject::errorStatusChanged;
```




<hr>



### signal forceAutoPushChanged 

```C++
void QFieldCloudProject::forceAutoPushChanged;
```




<hr>



### signal idChanged 

```C++
void QFieldCloudProject::idChanged;
```




<hr>



### signal isFeaturedChanged 

```C++
void QFieldCloudProject::isFeaturedChanged;
```




<hr>



### signal isOutdatedChanged 

```C++
void QFieldCloudProject::isOutdatedChanged;
```




<hr>



### signal isPackagingActiveChanged 

```C++
void QFieldCloudProject::isPackagingActiveChanged;
```




<hr>



### signal isPackagingFailedChanged 

```C++
void QFieldCloudProject::isPackagingFailedChanged;
```




<hr>



### signal isProjectOutdatedChanged 

```C++
void QFieldCloudProject::isProjectOutdatedChanged;
```




<hr>



### signal isPublicChanged 

```C++
void QFieldCloudProject::isPublicChanged;
```




<hr>



### signal isSharedDatasetsProjectChanged 

```C++
void QFieldCloudProject::isSharedDatasetsProjectChanged;
```




<hr>



### signal jobFinished 

```C++
void QFieldCloudProject::jobFinished;
```




<hr>



### signal lastDataLastUpdatedAtChanged 

```C++
void QFieldCloudProject::lastDataLastUpdatedAtChanged;
```




<hr>



### signal lastLocalDataLastUpdatedAtChanged 

```C++
void QFieldCloudProject::lastLocalDataLastUpdatedAtChanged;
```




<hr>



### signal lastLocalExportIdChanged 

```C++
void QFieldCloudProject::lastLocalExportIdChanged;
```




<hr>



### signal lastLocalExportedAtChanged 

```C++
void QFieldCloudProject::lastLocalExportedAtChanged;
```




<hr>



### signal lastLocalPushDeltasChanged 

```C++
void QFieldCloudProject::lastLocalPushDeltasChanged;
```




<hr>



### signal lastLocalRestrictedDataLastUpdatedAtChanged 

```C++
void QFieldCloudProject::lastLocalRestrictedDataLastUpdatedAtChanged;
```




<hr>



### signal lastRefreshedAtChanged 

```C++
void QFieldCloudProject::lastRefreshedAtChanged;
```




<hr>



### signal localPathChanged 

```C++
void QFieldCloudProject::localPathChanged;
```




<hr>



### signal modificationChanged 

```C++
void QFieldCloudProject::modificationChanged;
```




<hr>



### signal nameChanged 

```C++
void QFieldCloudProject::nameChanged;
```




<hr>



### signal needsRepackagingChanged 

```C++
void QFieldCloudProject::needsRepackagingChanged;
```




<hr>



### signal networkDeltaPushed 

```C++
void QFieldCloudProject::networkDeltaPushed;
```




<hr>



### signal networkDeltaStatusChecked 

```C++
void QFieldCloudProject::networkDeltaStatusChecked;
```




<hr>



### signal ownerChanged 

```C++
void QFieldCloudProject::ownerChanged;
```




<hr>



### signal packagedLayerErrorsChanged 

```C++
void QFieldCloudProject::packagedLayerErrorsChanged;
```




<hr>



### signal packagingStatusChanged 

```C++
void QFieldCloudProject::packagingStatusChanged;
```




<hr>



### signal packagingStatusStringChanged 

```C++
void QFieldCloudProject::packagingStatusStringChanged;
```




<hr>



### signal pushDeltaProgressChanged 

```C++
void QFieldCloudProject::pushDeltaProgressChanged;
```




<hr>



### signal pushFinished 

```C++
void QFieldCloudProject::pushFinished;
```




<hr>



### signal restrictedDataLastUpdatedAtChanged 

```C++
void QFieldCloudProject::restrictedDataLastUpdatedAtChanged;
```




<hr>



### signal sharedDatasetsProjectIdChanged 

```C++
void QFieldCloudProject::sharedDatasetsProjectIdChanged;
```




<hr>



### signal statusChanged 

```C++
void QFieldCloudProject::statusChanged;
```




<hr>



### signal thumbnailPathChanged 

```C++
void QFieldCloudProject::thumbnailPathChanged;
```




<hr>



### signal updatedAtChanged 

```C++
void QFieldCloudProject::updatedAtChanged;
```




<hr>



### signal uploadBytesSentChanged 

```C++
void QFieldCloudProject::uploadBytesSentChanged;
```




<hr>



### signal uploadBytesTotalChanged 

```C++
void QFieldCloudProject::uploadBytesTotalChanged;
```




<hr>



### signal uploadFinished 

```C++
void QFieldCloudProject::uploadFinished;
```




<hr>



### signal uploadProgressChanged 

```C++
void QFieldCloudProject::uploadProgressChanged;
```




<hr>



### signal userRoleChanged 

```C++
void QFieldCloudProject::userRoleChanged;
```




<hr>



### signal userRoleOriginChanged 

```C++
void QFieldCloudProject::userRoleOriginChanged;
```




<hr>
## Public Functions Documentation




### function QFieldCloudProject 

```C++
QFieldCloudProject::QFieldCloudProject (
    const QString & id=QString(),
    QFieldCloudConnection * connection=nullptr,
    QgsGpkgFlusher * gpkgFlusher=nullptr
) 
```




<hr>



### function attachmentsOnDemandEnabled [2/2]

```C++
inline bool QFieldCloudProject::attachmentsOnDemandEnabled () const
```




<hr>



### function autoPushEnabled [2/2]

```C++
inline bool QFieldCloudProject::autoPushEnabled () const
```




<hr>



### function autoPushIntervalMins [2/2]

```C++
inline int QFieldCloudProject::autoPushIntervalMins () const
```




<hr>



### function canRepackage 

```C++
inline bool QFieldCloudProject::canRepackage () const
```




<hr>



### function cancelDownload 

```C++
void QFieldCloudProject::cancelDownload () 
```




<hr>



### function cancelPush 

```C++
void QFieldCloudProject::cancelPush () 
```




<hr>



### function checkout 

```C++
inline ProjectCheckouts QFieldCloudProject::checkout () const
```




<hr>



### function createdAt [2/2]

```C++
inline QDateTime QFieldCloudProject::createdAt () const
```




<hr>



### function dataLastUpdatedAt 

```C++
inline QDateTime QFieldCloudProject::dataLastUpdatedAt () const
```




<hr>



### function deltaFileId 

```C++
inline QString QFieldCloudProject::deltaFileId () const
```




<hr>



### function deltaFilePushStatus [2/2]

```C++
inline DeltaFileStatus QFieldCloudProject::deltaFilePushStatus () const
```




<hr>



### function deltaFilePushStatusString 

```C++
inline QString QFieldCloudProject::deltaFilePushStatusString () const
```




<hr>



### function deltaFileWrapper [2/2]

```C++
inline DeltaFileWrapper * QFieldCloudProject::deltaFileWrapper () const
```




<hr>



### function deltaLayersToDownload 

```C++
inline QStringList QFieldCloudProject::deltaLayersToDownload () const
```




<hr>



### function deltaListModel [2/2]

```C++
inline DeltaListModel * QFieldCloudProject::deltaListModel () const
```




<hr>



### function deltasCount [2/2]

```C++
inline int QFieldCloudProject::deltasCount () const
```




<hr>



### function description [2/2]

```C++
inline QString QFieldCloudProject::description () const
```




<hr>



### function downloadAttachment 

```C++
Q_INVOKABLE void QFieldCloudProject::downloadAttachment (
    const QString & fileName
) 
```




<hr>



### function downloadBytesReceived [2/2]

```C++
inline qint64 QFieldCloudProject::downloadBytesReceived () const
```




<hr>



### function downloadBytesTotal [2/2]

```C++
inline qint64 QFieldCloudProject::downloadBytesTotal () const
```




<hr>



### function downloadProgress [2/2]

```C++
inline double QFieldCloudProject::downloadProgress () const
```




<hr>



### function downloadThumbnail 

```C++
Q_INVOKABLE void QFieldCloudProject::downloadThumbnail () 
```




<hr>



### function errorStatus 

```C++
inline ProjectErrorStatus QFieldCloudProject::errorStatus () const
```




<hr>



### function forceAutoPush [2/2]

```C++
inline bool QFieldCloudProject::forceAutoPush () const
```




<hr>



### function id [2/2]

```C++
inline QString QFieldCloudProject::id () const
```




<hr>



### function isFeatured [2/2]

```C++
inline bool QFieldCloudProject::isFeatured () const
```




<hr>



### function isOutdated [2/2]

```C++
inline bool QFieldCloudProject::isOutdated () const
```




<hr>



### function isPackagingActive 

```C++
inline bool QFieldCloudProject::isPackagingActive () const
```




<hr>



### function isPackagingFailed 

```C++
inline bool QFieldCloudProject::isPackagingFailed () const
```




<hr>



### function isProjectOutdated [2/2]

```C++
inline bool QFieldCloudProject::isProjectOutdated () const
```




<hr>



### function isPublic [2/2]

```C++
inline bool QFieldCloudProject::isPublic () const
```




<hr>



### function isSharedDatasetsProject 

```C++
inline bool QFieldCloudProject::isSharedDatasetsProject () const
```




<hr>



### function lastLocalDataLastUpdatedAt 

```C++
inline QDateTime QFieldCloudProject::lastLocalDataLastUpdatedAt () const
```




<hr>



### function lastLocalExportId 

```C++
inline QString QFieldCloudProject::lastLocalExportId () const
```




<hr>



### function lastLocalExportedAt [2/2]

```C++
inline QString QFieldCloudProject::lastLocalExportedAt () const
```




<hr>



### function lastLocalPushDeltas [2/2]

```C++
inline QString QFieldCloudProject::lastLocalPushDeltas () const
```




<hr>



### function lastLocalRestrictedDataLastUpdatedAt 

```C++
inline QDateTime QFieldCloudProject::lastLocalRestrictedDataLastUpdatedAt () const
```




<hr>



### function lastRefreshedAt 

```C++
inline QDateTime QFieldCloudProject::lastRefreshedAt () const
```




<hr>



### function localPath [2/2]

```C++
inline QString QFieldCloudProject::localPath () const
```




<hr>



### function modification 

```C++
inline ProjectModifications QFieldCloudProject::modification () const
```




<hr>



### function name [2/2]

```C++
inline QString QFieldCloudProject::name () const
```




<hr>



### function needsRepackaging 

```C++
inline bool QFieldCloudProject::needsRepackaging () const
```




<hr>



### function owner [2/2]

```C++
inline QString QFieldCloudProject::owner () const
```




<hr>



### function packageAndDownload 

```C++
Q_INVOKABLE void QFieldCloudProject::packageAndDownload () 
```




<hr>



### function packagedLayerErrors [2/2]

```C++
inline QStringList QFieldCloudProject::packagedLayerErrors () const
```




<hr>



### function packagingStatus [2/2]

```C++
inline PackagingStatus QFieldCloudProject::packagingStatus () const
```




<hr>



### function packagingStatusString 

```C++
inline QString QFieldCloudProject::packagingStatusString () const
```




<hr>



### function push 

```C++
Q_INVOKABLE void QFieldCloudProject::push (
    bool shouldDownloadUpdates
) 
```




<hr>



### function pushDeltaProgress [2/2]

```C++
inline double QFieldCloudProject::pushDeltaProgress () const
```




<hr>



### function refreshDeltaList 

```C++
void QFieldCloudProject::refreshDeltaList () 
```




<hr>



### function refreshModification 

```C++
void QFieldCloudProject::refreshModification () 
```




<hr>



### function removeLocally 

```C++
void QFieldCloudProject::removeLocally () 
```




<hr>



### function restrictedDataLastUpdatedAt 

```C++
inline QDateTime QFieldCloudProject::restrictedDataLastUpdatedAt () const
```




<hr>



### function setAttachmentsOnDemandEnabled 

```C++
void QFieldCloudProject::setAttachmentsOnDemandEnabled (
    bool enabled
) 
```




<hr>



### function setAutoPushEnabled 

```C++
void QFieldCloudProject::setAutoPushEnabled (
    bool enabled
) 
```




<hr>



### function setAutoPushIntervalMins 

```C++
void QFieldCloudProject::setAutoPushIntervalMins (
    int minutes
) 
```




<hr>



### function setCanRepackage 

```C++
void QFieldCloudProject::setCanRepackage (
    bool canRepackage
) 
```




<hr>



### function setCheckout 

```C++
void QFieldCloudProject::setCheckout (
    ProjectCheckouts checkout
) 
```




<hr>



### function setCreatedAt 

```C++
void QFieldCloudProject::setCreatedAt (
    const QDateTime & createdAt
) 
```




<hr>



### function setDataLastUpdatedAt 

```C++
void QFieldCloudProject::setDataLastUpdatedAt (
    const QDateTime & dataLastUpdatedAt
) 
```




<hr>



### function setDeltaFileId 

```C++
void QFieldCloudProject::setDeltaFileId (
    const QString & deltaFileId
) 
```




<hr>



### function setDeltaFilePushStatus 

```C++
void QFieldCloudProject::setDeltaFilePushStatus (
    DeltaFileStatus deltaFilePushStatus
) 
```




<hr>



### function setDeltaFilePushStatusString 

```C++
void QFieldCloudProject::setDeltaFilePushStatusString (
    const QString & deltaFilePushStatusString
) 
```




<hr>



### function setDeltaLayersToDownload 

```C++
void QFieldCloudProject::setDeltaLayersToDownload (
    const QStringList & deltaLayersToDownload
) 
```




<hr>



### function setDescription 

```C++
void QFieldCloudProject::setDescription (
    const QString & description
) 
```




<hr>



### function setErrorStatus 

```C++
void QFieldCloudProject::setErrorStatus (
    ProjectErrorStatus errorStatus
) 
```




<hr>



### function setForceAutoPush 

```C++
void QFieldCloudProject::setForceAutoPush (
    bool force
) 
```




<hr>



### function setIsFeatured 

```C++
void QFieldCloudProject::setIsFeatured (
    bool isFeatured
) 
```




<hr>



### function setIsOutdated 

```C++
void QFieldCloudProject::setIsOutdated (
    bool isOutdated
) 
```




<hr>



### function setIsPackagingActive 

```C++
void QFieldCloudProject::setIsPackagingActive (
    bool isPackagingActive
) 
```




<hr>



### function setIsPackagingFailed 

```C++
void QFieldCloudProject::setIsPackagingFailed (
    bool isPackagingFailed
) 
```




<hr>



### function setIsProjectOutdated 

```C++
void QFieldCloudProject::setIsProjectOutdated (
    bool isProjectOutdated
) 
```




<hr>



### function setIsPublic 

```C++
void QFieldCloudProject::setIsPublic (
    bool isPublic
) 
```




<hr>



### function setIsSharedDatasetsProject 

```C++
void QFieldCloudProject::setIsSharedDatasetsProject (
    bool isSharedDatasetsProject
) 
```




<hr>



### function setLastLocalDataLastUpdatedAt 

```C++
void QFieldCloudProject::setLastLocalDataLastUpdatedAt (
    const QDateTime & lastLocalDataLastUpdatedAt
) 
```




<hr>



### function setLastLocalExportId 

```C++
void QFieldCloudProject::setLastLocalExportId (
    const QString & lastLocalExportId
) 
```




<hr>



### function setLastLocalExportedAt 

```C++
void QFieldCloudProject::setLastLocalExportedAt (
    const QString & lastLocalExportedAt
) 
```




<hr>



### function setLastLocalPushDeltas 

```C++
void QFieldCloudProject::setLastLocalPushDeltas (
    const QString & lastLocalPushDeltas
) 
```




<hr>



### function setLastLocalRestrictedDataLastUpdatedAt 

```C++
void QFieldCloudProject::setLastLocalRestrictedDataLastUpdatedAt (
    const QDateTime & lastLocalRestrictedDataLastUpdatedAt
) 
```




<hr>



### function setLastRefreshedAt 

```C++
void QFieldCloudProject::setLastRefreshedAt (
    const QDateTime & lastRefreshedAt
) 
```




<hr>



### function setLocalPath 

```C++
void QFieldCloudProject::setLocalPath (
    const QString & localPath
) 
```




<hr>



### function setModification 

```C++
void QFieldCloudProject::setModification (
    ProjectModification modification
) 
```




<hr>



### function setName 

```C++
void QFieldCloudProject::setName (
    const QString & name
) 
```




<hr>



### function setNeedsRepackaging 

```C++
void QFieldCloudProject::setNeedsRepackaging (
    bool needsRepackaging
) 
```




<hr>



### function setOwner 

```C++
void QFieldCloudProject::setOwner (
    const QString & owner
) 
```




<hr>



### function setPackagedLayerErrors 

```C++
void QFieldCloudProject::setPackagedLayerErrors (
    const QStringList & packagedLayerErrors
) 
```




<hr>



### function setPackagingStatus 

```C++
void QFieldCloudProject::setPackagingStatus (
    PackagingStatus packagingStatus
) 
```




<hr>



### function setPackagingStatusString 

```C++
void QFieldCloudProject::setPackagingStatusString (
    const QString & packagingStatusString
) 
```




<hr>



### function setRestrictedDataLastUpdatedAt 

```C++
void QFieldCloudProject::setRestrictedDataLastUpdatedAt (
    const QDateTime & restrictedDataLastUpdatedAt
) 
```




<hr>



### function setSharedDatasetsProjectId 

```C++
void QFieldCloudProject::setSharedDatasetsProjectId (
    const QString & id
) 
```




<hr>



### function setStatus 

```C++
void QFieldCloudProject::setStatus (
    ProjectStatus status
) 
```




<hr>



### function setThumbnailPath 

```C++
void QFieldCloudProject::setThumbnailPath (
    const QString & thumbnailPath
) 
```




<hr>



### function setUpdatedAt 

```C++
void QFieldCloudProject::setUpdatedAt (
    const QDateTime & updatedAt
) 
```




<hr>



### function setUserRole 

```C++
void QFieldCloudProject::setUserRole (
    const QString & userRole
) 
```




<hr>



### function setUserRoleOrigin 

```C++
void QFieldCloudProject::setUserRoleOrigin (
    const QString & userRoleOrigin
) 
```




<hr>



### function sharedDatasetsProjectId 

```C++
inline QString QFieldCloudProject::sharedDatasetsProjectId () const
```




<hr>



### function status [2/2]

```C++
inline ProjectStatus QFieldCloudProject::status () const
```




<hr>



### function thumbnailPath [2/2]

```C++
inline QString QFieldCloudProject::thumbnailPath () const
```




<hr>



### function updatedAt [2/2]

```C++
inline QDateTime QFieldCloudProject::updatedAt () const
```




<hr>



### function uploadBytesSent [2/2]

```C++
inline qint64 QFieldCloudProject::uploadBytesSent () const
```




<hr>



### function uploadBytesTotal [2/2]

```C++
inline qint64 QFieldCloudProject::uploadBytesTotal () const
```




<hr>



### function uploadLocalPath 

```C++
Q_INVOKABLE void QFieldCloudProject::uploadLocalPath (
    QString localPath,
    bool deleteAfterSuccessfulUpload=false
) 
```




<hr>



### function uploadProgress [2/2]

```C++
inline double QFieldCloudProject::uploadProgress () const
```




<hr>



### function userRole [2/2]

```C++
inline QString QFieldCloudProject::userRole () const
```




<hr>



### function userRoleOrigin [2/2]

```C++
inline QString QFieldCloudProject::userRoleOrigin () const
```




<hr>



### function ~QFieldCloudProject 

```C++
QFieldCloudProject::~QFieldCloudProject () = default
```




<hr>
## Public Static Functions Documentation




### function fromDetails 

```C++
static QFieldCloudProject * QFieldCloudProject::fromDetails (
    const QVariantHash & details,
    QFieldCloudConnection * connection,
    QgsGpkgFlusher * gpkgFlusher=nullptr
) 
```




<hr>



### function fromLocalSettings 

```C++
static QFieldCloudProject * QFieldCloudProject::fromLocalSettings (
    const QString & id,
    QFieldCloudConnection * connection,
    QgsGpkgFlusher * gpkgFlusher=nullptr
) 
```




<hr>



### function getJobStatusFromString 

```C++
static QFieldCloudProject::JobStatus QFieldCloudProject::getJobStatusFromString (
    const QString & status
) 
```




<hr>



### function getJobTypeAsString 

```C++
static QString QFieldCloudProject::getJobTypeAsString (
    QFieldCloudProject::JobType jobType
) 
```




<hr>



### function restoreLocalSettings 

```C++
static void QFieldCloudProject::restoreLocalSettings (
    QFieldCloudProject * project,
    const QDir & localPath
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfieldcloudproject.h`

