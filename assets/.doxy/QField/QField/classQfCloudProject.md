

# Class QfCloudProject



[**ClassList**](annotated.md) **>** [**QfCloudProject**](classQfCloudProject.md)








Inherits the following classes: QObject












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**FileTransfer**](structQfCloudProject_1_1FileTransfer.md) <br> |


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
| enum  | [**ProjectType**](#enum-projecttype)  <br>_The type of the cloud project, as reported by the REST endpoint._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**attachmentsOnDemandEnabled**](classQfCloudProject.md#property-attachmentsondemandenabled-12)  <br> |
| property bool | [**autoPushEnabled**](classQfCloudProject.md#property-autopushenabled-12)  <br> |
| property int | [**autoPushIntervalMins**](classQfCloudProject.md#property-autopushintervalmins-12)  <br> |
| property QDateTime | [**createdAt**](classQfCloudProject.md#property-createdat-12)  <br> |
| property QDateTime | [**dataLastUpdatedAt**](classQfCloudProject.md#property-datalastupdatedat-12)  <br> |
| property [**DeltaFileStatus**](classQfCloudProject.md#enum-deltafilestatus) | [**deltaFilePushStatus**](classQfCloudProject.md#property-deltafilepushstatus-12)  <br> |
| property [**QfDeltaFileWrapper**](classQfDeltaFileWrapper.md) \* | [**deltaFileWrapper**](classQfCloudProject.md#property-deltafilewrapper-12)  <br> |
| property int | [**deltasCount**](classQfCloudProject.md#property-deltascount-12)  <br> |
| property QString | [**description**](classQfCloudProject.md#property-description-12)  <br> |
| property qint64 | [**downloadBytesReceived**](classQfCloudProject.md#property-downloadbytesreceived-12)  <br> |
| property qint64 | [**downloadBytesTotal**](classQfCloudProject.md#property-downloadbytestotal-12)  <br> |
| property double | [**downloadProgress**](classQfCloudProject.md#property-downloadprogress-12)  <br> |
| property bool | [**forceAutoPush**](classQfCloudProject.md#property-forceautopush-12)  <br> |
| property QString | [**id**](classQfCloudProject.md#property-id-12)  <br> |
| property bool | [**isFeatured**](classQfCloudProject.md#property-isfeatured-12)  <br> |
| property bool | [**isOutdated**](classQfCloudProject.md#property-isoutdated-12)  <br> |
| property bool | [**isProjectOutdated**](classQfCloudProject.md#property-isprojectoutdated-12)  <br> |
| property bool | [**isPublic**](classQfCloudProject.md#property-ispublic-12)  <br> |
| property QDateTime | [**lastLocalExportedAt**](classQfCloudProject.md#property-lastlocalexportedat-12)  <br> |
| property QDateTime | [**lastLocalPushDeltas**](classQfCloudProject.md#property-lastlocalpushdeltas-12)  <br> |
| property QString | [**localPath**](classQfCloudProject.md#property-localpath-12)  <br> |
| property QString | [**name**](classQfCloudProject.md#property-name-12)  <br> |
| property QString | [**owner**](classQfCloudProject.md#property-owner-12)  <br> |
| property QStringList | [**packagedLayerErrors**](classQfCloudProject.md#property-packagedlayererrors-12)  <br> |
| property [**PackagingStatus**](classQfCloudProject.md#enum-packagingstatus) | [**packagingStatus**](classQfCloudProject.md#property-packagingstatus-12)  <br> |
| property double | [**pushDeltaProgress**](classQfCloudProject.md#property-pushdeltaprogress-12)  <br> |
| property qint64 | [**remoteSizeBytes**](classQfCloudProject.md#property-remotesizebytes-12)  <br> |
| property [**ProjectStatus**](classQfCloudProject.md#enum-projectstatus) | [**status**](classQfCloudProject.md#property-status-12)  <br> |
| property QString | [**thumbnailPath**](classQfCloudProject.md#property-thumbnailpath-12)  <br> |
| property [**ProjectType**](classQfCloudProject.md#enum-projecttype) | [**type**](classQfCloudProject.md#property-type-12)  <br> |
| property QDateTime | [**updatedAt**](classQfCloudProject.md#property-updatedat-12)  <br> |
| property qint64 | [**uploadBytesSent**](classQfCloudProject.md#property-uploadbytessent-12)  <br> |
| property qint64 | [**uploadBytesTotal**](classQfCloudProject.md#property-uploadbytestotal-12)  <br> |
| property double | [**uploadProgress**](classQfCloudProject.md#property-uploadprogress-12)  <br> |
| property QString | [**userRole**](classQfCloudProject.md#property-userrole-12)  <br> |
| property QString | [**userRoleOrigin**](classQfCloudProject.md#property-userroleorigin-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**attachmentsOnDemandEnabledChanged**](classQfCloudProject.md#signal-attachmentsondemandenabledchanged)  <br> |
| signal void | [**autoPushEnabledChanged**](classQfCloudProject.md#signal-autopushenabledchanged)  <br> |
| signal void | [**autoPushIntervalMinsChanged**](classQfCloudProject.md#signal-autopushintervalminschanged)  <br> |
| signal void | [**canRepackageChanged**](classQfCloudProject.md#signal-canrepackagechanged)  <br> |
| signal void | [**checkoutChanged**](classQfCloudProject.md#signal-checkoutchanged)  <br> |
| signal void | [**createdAtChanged**](classQfCloudProject.md#signal-createdatchanged)  <br> |
| signal void | [**dataLastUpdatedAtChanged**](classQfCloudProject.md#signal-datalastupdatedatchanged)  <br> |
| signal void | [**dataRefreshed**](classQfCloudProject.md#signal-datarefreshed) ([**ProjectRefreshReason**](classQfCloudProject.md#enum-projectrefreshreason) reason, const QString & error=QString()) <br> |
| signal void | [**deltaFileIdChanged**](classQfCloudProject.md#signal-deltafileidchanged)  <br> |
| signal void | [**deltaFilePushStatusChanged**](classQfCloudProject.md#signal-deltafilepushstatuschanged)  <br> |
| signal void | [**deltaFilePushStatusStringChanged**](classQfCloudProject.md#signal-deltafilepushstatusstringchanged)  <br> |
| signal void | [**deltaFileWrapperChanged**](classQfCloudProject.md#signal-deltafilewrapperchanged)  <br> |
| signal void | [**deltaLayersToDownloadChanged**](classQfCloudProject.md#signal-deltalayerstodownloadchanged)  <br> |
| signal void | [**deltasCountChanged**](classQfCloudProject.md#signal-deltascountchanged)  <br> |
| signal void | [**descriptionChanged**](classQfCloudProject.md#signal-descriptionchanged)  <br> |
| signal void | [**downloadAttachmentFinished**](classQfCloudProject.md#signal-downloadattachmentfinished) (const QString & fileName, const QString & error=QString()) <br> |
| signal void | [**downloadBytesReceivedChanged**](classQfCloudProject.md#signal-downloadbytesreceivedchanged)  <br> |
| signal void | [**downloadBytesTotalChanged**](classQfCloudProject.md#signal-downloadbytestotalchanged)  <br> |
| signal void | [**downloadFinished**](classQfCloudProject.md#signal-downloadfinished) (const QString & error=QString()) <br> |
| signal void | [**downloadProgressChanged**](classQfCloudProject.md#signal-downloadprogresschanged)  <br> |
| signal void | [**downloaded**](classQfCloudProject.md#signal-downloaded) (const QString & error=QString()) <br> |
| signal void | [**errorStatusChanged**](classQfCloudProject.md#signal-errorstatuschanged)  <br> |
| signal void | [**forceAutoPushChanged**](classQfCloudProject.md#signal-forceautopushchanged)  <br> |
| signal void | [**idChanged**](classQfCloudProject.md#signal-idchanged)  <br> |
| signal void | [**isFeaturedChanged**](classQfCloudProject.md#signal-isfeaturedchanged)  <br> |
| signal void | [**isOutdatedChanged**](classQfCloudProject.md#signal-isoutdatedchanged)  <br> |
| signal void | [**isPackagingActiveChanged**](classQfCloudProject.md#signal-ispackagingactivechanged)  <br> |
| signal void | [**isPackagingFailedChanged**](classQfCloudProject.md#signal-ispackagingfailedchanged)  <br> |
| signal void | [**isProjectOutdatedChanged**](classQfCloudProject.md#signal-isprojectoutdatedchanged)  <br> |
| signal void | [**isPublicChanged**](classQfCloudProject.md#signal-ispublicchanged)  <br> |
| signal void | [**jobFinished**](classQfCloudProject.md#signal-jobfinished) ([**JobType**](classQfCloudProject.md#enum-jobtype) type, const QString & error=QString()) <br> |
| signal void | [**lastDataLastUpdatedAtChanged**](classQfCloudProject.md#signal-lastdatalastupdatedatchanged)  <br> |
| signal void | [**lastLocalDataLastUpdatedAtChanged**](classQfCloudProject.md#signal-lastlocaldatalastupdatedatchanged)  <br> |
| signal void | [**lastLocalExportIdChanged**](classQfCloudProject.md#signal-lastlocalexportidchanged)  <br> |
| signal void | [**lastLocalExportedAtChanged**](classQfCloudProject.md#signal-lastlocalexportedatchanged)  <br> |
| signal void | [**lastLocalPushDeltasChanged**](classQfCloudProject.md#signal-lastlocalpushdeltaschanged)  <br> |
| signal void | [**lastLocalRestrictedDataLastUpdatedAtChanged**](classQfCloudProject.md#signal-lastlocalrestricteddatalastupdatedatchanged)  <br> |
| signal void | [**lastRefreshedAtChanged**](classQfCloudProject.md#signal-lastrefreshedatchanged)  <br> |
| signal void | [**localPathChanged**](classQfCloudProject.md#signal-localpathchanged)  <br> |
| signal void | [**modificationChanged**](classQfCloudProject.md#signal-modificationchanged)  <br> |
| signal void | [**nameChanged**](classQfCloudProject.md#signal-namechanged)  <br> |
| signal void | [**needsRepackagingChanged**](classQfCloudProject.md#signal-needsrepackagingchanged)  <br> |
| signal void | [**networkDeltaPushed**](classQfCloudProject.md#signal-networkdeltapushed)  <br> |
| signal void | [**networkDeltaStatusChecked**](classQfCloudProject.md#signal-networkdeltastatuschecked)  <br> |
| signal void | [**ownerChanged**](classQfCloudProject.md#signal-ownerchanged)  <br> |
| signal void | [**packagedLayerErrorsChanged**](classQfCloudProject.md#signal-packagedlayererrorschanged)  <br> |
| signal void | [**packagingStatusChanged**](classQfCloudProject.md#signal-packagingstatuschanged)  <br> |
| signal void | [**packagingStatusStringChanged**](classQfCloudProject.md#signal-packagingstatusstringchanged)  <br> |
| signal void | [**pushDeltaProgressChanged**](classQfCloudProject.md#signal-pushdeltaprogresschanged)  <br> |
| signal void | [**pushFinished**](classQfCloudProject.md#signal-pushfinished) (bool isDownloading, const QString & error=QString()) <br> |
| signal void | [**remoteSizeBytesChanged**](classQfCloudProject.md#signal-remotesizebyteschanged)  <br> |
| signal void | [**restrictedDataLastUpdatedAtChanged**](classQfCloudProject.md#signal-restricteddatalastupdatedatchanged)  <br> |
| signal void | [**sharedDatasetsProjectIdChanged**](classQfCloudProject.md#signal-shareddatasetsprojectidchanged)  <br> |
| signal void | [**statusChanged**](classQfCloudProject.md#signal-statuschanged)  <br> |
| signal void | [**thumbnailPathChanged**](classQfCloudProject.md#signal-thumbnailpathchanged)  <br> |
| signal void | [**typeChanged**](classQfCloudProject.md#signal-typechanged)  <br> |
| signal void | [**updatedAtChanged**](classQfCloudProject.md#signal-updatedatchanged)  <br> |
| signal void | [**uploadBytesSentChanged**](classQfCloudProject.md#signal-uploadbytessentchanged)  <br> |
| signal void | [**uploadBytesTotalChanged**](classQfCloudProject.md#signal-uploadbytestotalchanged)  <br> |
| signal void | [**uploadFinished**](classQfCloudProject.md#signal-uploadfinished) (const QString & error=QString()) <br> |
| signal void | [**uploadProgressChanged**](classQfCloudProject.md#signal-uploadprogresschanged)  <br> |
| signal void | [**userRoleChanged**](classQfCloudProject.md#signal-userrolechanged)  <br> |
| signal void | [**userRoleOriginChanged**](classQfCloudProject.md#signal-userroleoriginchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCloudProject**](#function-qfcloudproject) (const QString & id=QString(), [**QfCloudConnection**](classQfCloudConnection.md) \* connection=nullptr, [**QgsGpkgFlusher**](classQgsGpkgFlusher.md) \* gpkgFlusher=nullptr) <br> |
|  bool | [**attachmentsOnDemandEnabled**](#function-attachmentsondemandenabled-22) () const<br> |
|  bool | [**autoPushEnabled**](#function-autopushenabled-22) () const<br> |
|  int | [**autoPushIntervalMins**](#function-autopushintervalmins-22) () const<br> |
|  bool | [**canRepackage**](#function-canrepackage) () const<br> |
|  void | [**cancelDownload**](#function-canceldownload) () <br> |
|  void | [**cancelPush**](#function-cancelpush) () <br> |
|  ProjectCheckouts | [**checkout**](#function-checkout) () const<br> |
|  QDateTime | [**createdAt**](#function-createdat-22) () const<br> |
|  QDateTime | [**dataLastUpdatedAt**](#function-datalastupdatedat-22) () const<br> |
|  QString | [**deltaFileId**](#function-deltafileid) () const<br> |
|  [**DeltaFileStatus**](classQfCloudProject.md#enum-deltafilestatus) | [**deltaFilePushStatus**](#function-deltafilepushstatus-22) () const<br> |
|  QString | [**deltaFilePushStatusString**](#function-deltafilepushstatusstring) () const<br> |
|  [**QfDeltaFileWrapper**](classQfDeltaFileWrapper.md) \* | [**deltaFileWrapper**](#function-deltafilewrapper-22) () const<br> |
|  QStringList | [**deltaLayersToDownload**](#function-deltalayerstodownload) () const<br> |
|  int | [**deltasCount**](#function-deltascount-22) () const<br> |
|  QString | [**description**](#function-description-22) () const<br> |
|  Q\_INVOKABLE void | [**downloadAttachment**](#function-downloadattachment) (const QString & fileName) <br> |
|  qint64 | [**downloadBytesReceived**](#function-downloadbytesreceived-22) () const<br> |
|  qint64 | [**downloadBytesTotal**](#function-downloadbytestotal-22) () const<br> |
|  double | [**downloadProgress**](#function-downloadprogress-22) () const<br> |
|  Q\_INVOKABLE void | [**downloadThumbnail**](#function-downloadthumbnail) () <br> |
|  void | [**ensureProjectCreated**](#function-ensureprojectcreated) () <br> |
|  [**ProjectErrorStatus**](classQfCloudProject.md#enum-projecterrorstatus) | [**errorStatus**](#function-errorstatus) () const<br> |
|  bool | [**forceAutoPush**](#function-forceautopush-22) () const<br> |
|  QString | [**id**](#function-id-22) () const<br> |
|  bool | [**isFeatured**](#function-isfeatured-22) () const<br> |
|  bool | [**isOutdated**](#function-isoutdated-22) () const<br> |
|  bool | [**isPackagingActive**](#function-ispackagingactive) () const<br> |
|  bool | [**isPackagingFailed**](#function-ispackagingfailed) () const<br> |
|  bool | [**isProjectOutdated**](#function-isprojectoutdated-22) () const<br> |
|  bool | [**isPublic**](#function-ispublic-22) () const<br> |
|  QDateTime | [**lastLocalDataLastUpdatedAt**](#function-lastlocaldatalastupdatedat) () const<br> |
|  QString | [**lastLocalExportId**](#function-lastlocalexportid) () const<br> |
|  QDateTime | [**lastLocalExportedAt**](#function-lastlocalexportedat-22) () const<br> |
|  QDateTime | [**lastLocalPushDeltas**](#function-lastlocalpushdeltas-22) () const<br> |
|  QDateTime | [**lastLocalRestrictedDataLastUpdatedAt**](#function-lastlocalrestricteddatalastupdatedat) () const<br> |
|  QDateTime | [**lastRefreshedAt**](#function-lastrefreshedat) () const<br> |
|  QString | [**localPath**](#function-localpath-22) () const<br> |
|  ProjectModifications | [**modification**](#function-modification) () const<br> |
|  QString | [**name**](#function-name-22) () const<br> |
|  bool | [**needsRepackaging**](#function-needsrepackaging) () const<br> |
|  QString | [**owner**](#function-owner-22) () const<br> |
|  Q\_INVOKABLE void | [**packageAndDownload**](#function-packageanddownload) () <br> |
|  QStringList | [**packagedLayerErrors**](#function-packagedlayererrors-22) () const<br> |
|  [**PackagingStatus**](classQfCloudProject.md#enum-packagingstatus) | [**packagingStatus**](#function-packagingstatus-22) () const<br> |
|  QString | [**packagingStatusString**](#function-packagingstatusstring) () const<br> |
|  Q\_INVOKABLE void | [**push**](#function-push) (bool shouldDownloadUpdates) <br> |
|  double | [**pushDeltaProgress**](#function-pushdeltaprogress-22) () const<br> |
|  void | [**refreshDeltaList**](#function-refreshdeltalist) () <br> |
|  void | [**refreshModification**](#function-refreshmodification) () <br> |
|  qint64 | [**remoteSizeBytes**](#function-remotesizebytes-22) () const<br> |
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
|  void | [**setDeltaFilePushStatus**](#function-setdeltafilepushstatus) ([**DeltaFileStatus**](classQfCloudProject.md#enum-deltafilestatus) deltaFilePushStatus) <br> |
|  void | [**setDeltaFilePushStatusString**](#function-setdeltafilepushstatusstring) (const QString & deltaFilePushStatusString) <br> |
|  void | [**setDeltaLayersToDownload**](#function-setdeltalayerstodownload) (const QStringList & deltaLayersToDownload) <br> |
|  void | [**setDescription**](#function-setdescription) (const QString & description) <br> |
|  void | [**setErrorStatus**](#function-seterrorstatus) ([**ProjectErrorStatus**](classQfCloudProject.md#enum-projecterrorstatus) errorStatus) <br> |
|  void | [**setForceAutoPush**](#function-setforceautopush) (bool force) <br> |
|  void | [**setIsFeatured**](#function-setisfeatured) (bool isFeatured) <br> |
|  void | [**setIsOutdated**](#function-setisoutdated) (bool isOutdated) <br> |
|  void | [**setIsPackagingActive**](#function-setispackagingactive) (bool isPackagingActive) <br> |
|  void | [**setIsPackagingFailed**](#function-setispackagingfailed) (bool isPackagingFailed) <br> |
|  void | [**setIsProjectOutdated**](#function-setisprojectoutdated) (bool isProjectOutdated) <br> |
|  void | [**setIsPublic**](#function-setispublic) (bool isPublic) <br> |
|  void | [**setLastLocalDataLastUpdatedAt**](#function-setlastlocaldatalastupdatedat) (const QDateTime & lastLocalDataLastUpdatedAt) <br> |
|  void | [**setLastLocalExportId**](#function-setlastlocalexportid) (const QString & lastLocalExportId) <br> |
|  void | [**setLastLocalExportedAt**](#function-setlastlocalexportedat) (const QDateTime & lastLocalExportedAt) <br> |
|  void | [**setLastLocalPushDeltas**](#function-setlastlocalpushdeltas) (const QDateTime & lastLocalPushDeltas) <br> |
|  void | [**setLastLocalRestrictedDataLastUpdatedAt**](#function-setlastlocalrestricteddatalastupdatedat) (const QDateTime & lastLocalRestrictedDataLastUpdatedAt) <br> |
|  void | [**setLastRefreshedAt**](#function-setlastrefreshedat) (const QDateTime & lastRefreshedAt) <br> |
|  void | [**setLocalPath**](#function-setlocalpath) (const QString & localPath) <br> |
|  void | [**setModification**](#function-setmodification) ([**ProjectModification**](classQfCloudProject.md#enum-projectmodification) modification) <br> |
|  void | [**setName**](#function-setname) (const QString & name) <br> |
|  void | [**setNeedsRepackaging**](#function-setneedsrepackaging) (bool needsRepackaging) <br> |
|  void | [**setOwner**](#function-setowner) (const QString & owner) <br> |
|  void | [**setPackagedLayerErrors**](#function-setpackagedlayererrors) (const QStringList & packagedLayerErrors) <br> |
|  void | [**setPackagingStatus**](#function-setpackagingstatus) ([**PackagingStatus**](classQfCloudProject.md#enum-packagingstatus) packagingStatus) <br> |
|  void | [**setPackagingStatusString**](#function-setpackagingstatusstring) (const QString & packagingStatusString) <br> |
|  void | [**setRemoteSizeBytes**](#function-setremotesizebytes) (qint64 remoteSizeBytes) <br> |
|  void | [**setRestrictedDataLastUpdatedAt**](#function-setrestricteddatalastupdatedat) (const QDateTime & restrictedDataLastUpdatedAt) <br> |
|  void | [**setSharedDatasetsProjectId**](#function-setshareddatasetsprojectid) (const QString & id) <br> |
|  void | [**setStatus**](#function-setstatus) ([**ProjectStatus**](classQfCloudProject.md#enum-projectstatus) status) <br> |
|  void | [**setThumbnailPath**](#function-setthumbnailpath) (const QString & thumbnailPath) <br> |
|  void | [**setType**](#function-settype) ([**ProjectType**](classQfCloudProject.md#enum-projecttype) projectType) <br> |
|  void | [**setUpdatedAt**](#function-setupdatedat) (const QDateTime & updatedAt) <br> |
|  void | [**setUserRole**](#function-setuserrole) (const QString & userRole) <br> |
|  void | [**setUserRoleOrigin**](#function-setuserroleorigin) (const QString & userRoleOrigin) <br> |
|  QString | [**sharedDatasetsProjectId**](#function-shareddatasetsprojectid) () const<br> |
|  [**ProjectStatus**](classQfCloudProject.md#enum-projectstatus) | [**status**](#function-status-22) () const<br> |
|  QString | [**thumbnailPath**](#function-thumbnailpath-22) () const<br> |
|  [**ProjectType**](classQfCloudProject.md#enum-projecttype) | [**type**](#function-type-22) () const<br> |
|  QDateTime | [**updatedAt**](#function-updatedat-22) () const<br> |
|  qint64 | [**uploadBytesSent**](#function-uploadbytessent-22) () const<br> |
|  qint64 | [**uploadBytesTotal**](#function-uploadbytestotal-22) () const<br> |
|  Q\_INVOKABLE void | [**uploadLocalPath**](#function-uploadlocalpath) (QString localPath, bool deleteAfterSuccessfulUpload=false) <br> |
|  double | [**uploadProgress**](#function-uploadprogress-22) () const<br> |
|  QString | [**userRole**](#function-userrole-22) () const<br> |
|  QString | [**userRoleOrigin**](#function-userroleorigin-22) () const<br> |
|   | [**~QfCloudProject**](#function-qfcloudproject) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**QfCloudProject**](classQfCloudProject.md) \* | [**fromDetails**](#function-fromdetails) (const QVariantHash & details, [**QfCloudConnection**](classQfCloudConnection.md) \* connection, [**QgsGpkgFlusher**](classQgsGpkgFlusher.md) \* gpkgFlusher=nullptr) <br> |
|  [**QfCloudProject**](classQfCloudProject.md) \* | [**fromLocalSettings**](#function-fromlocalsettings) (const QString & id, [**QfCloudConnection**](classQfCloudConnection.md) \* connection, [**QgsGpkgFlusher**](classQgsGpkgFlusher.md) \* gpkgFlusher=nullptr) <br> |
|  [**QfCloudProject::JobStatus**](classQfCloudProject.md#enum-jobstatus) | [**getJobStatusFromString**](#function-getjobstatusfromstring) (const QString & status) <br> |
|  QString | [**getJobTypeAsString**](#function-getjobtypeasstring) ([**QfCloudProject::JobType**](classQfCloudProject.md#enum-jobtype) jobType) <br> |
|  void | [**restoreLocalSettings**](#function-restorelocalsettings) ([**QfCloudProject**](classQfCloudProject.md) \* project, const QDir & localPath) <br> |
|  [**QfCloudProject::ProjectType**](classQfCloudProject.md#enum-projecttype) | [**typeFromString**](#function-typefromstring) (const QString & type) <br>_Returns the ProjectType matching the given_ _type_ _string; unknown or empty values return ProjectType::Regular._ |


























## Public Types Documentation




### enum DeltaFileStatus 

_The status of the running server job for applying deltas on a project._ 
```C++
enum QfCloudProject::DeltaFileStatus {
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
enum QfCloudProject::JobStatus {
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
enum QfCloudProject::JobType {
    Package
};
```




<hr>



### enum PackagingStatus 

_The status of the running server job for packaging a project._ 
```C++
enum QfCloudProject::PackagingStatus {
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
enum QfCloudProject::ProjectCheckout {
    RemoteCheckout = 2 << 0,
    LocalCheckout = 2 << 1,
    LocalAndRemoteCheckout = RemoteCheckout | LocalCheckout
};
```




<hr>



### enum ProjectErrorStatus 

_Whether the project has experienced an error._ 
```C++
enum QfCloudProject::ProjectErrorStatus {
    NoErrorStatus,
    DownloadErrorStatus,
    PushErrorStatus
};
```




<hr>



### enum ProjectModification 

_Whether the project has no or local and/or remote modification. Indicates wheter can be synced._ 
```C++
enum QfCloudProject::ProjectModification {
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
enum QfCloudProject::ProjectRefreshReason {
    Package,
    DeltaPushed
};
```




<hr>



### enum ProjectStatus 

_Whether the project is busy or idle._ 
```C++
enum QfCloudProject::ProjectStatus {
    Idle,
    Creating,
    Downloading,
    Pushing,
    Uploading,
    Failing
};
```




<hr>



### enum ProjectType 

_The type of the cloud project, as reported by the REST endpoint._ 
```C++
enum QfCloudProject::ProjectType {
    Regular,
    SharedDatasets,
    Template
};
```




<hr>
## Public Properties Documentation




### property attachmentsOnDemandEnabled [1/2]

```C++
bool QfCloudProject::attachmentsOnDemandEnabled;
```




<hr>



### property autoPushEnabled [1/2]

```C++
bool QfCloudProject::autoPushEnabled;
```




<hr>



### property autoPushIntervalMins [1/2]

```C++
int QfCloudProject::autoPushIntervalMins;
```




<hr>



### property createdAt [1/2]

```C++
QDateTime QfCloudProject::createdAt;
```




<hr>



### property dataLastUpdatedAt [1/2]

```C++
QDateTime QfCloudProject::dataLastUpdatedAt;
```




<hr>



### property deltaFilePushStatus [1/2]

```C++
DeltaFileStatus QfCloudProject::deltaFilePushStatus;
```




<hr>



### property deltaFileWrapper [1/2]

```C++
QfDeltaFileWrapper * QfCloudProject::deltaFileWrapper;
```




<hr>



### property deltasCount [1/2]

```C++
int QfCloudProject::deltasCount;
```




<hr>



### property description [1/2]

```C++
QString QfCloudProject::description;
```




<hr>



### property downloadBytesReceived [1/2]

```C++
qint64 QfCloudProject::downloadBytesReceived;
```




<hr>



### property downloadBytesTotal [1/2]

```C++
qint64 QfCloudProject::downloadBytesTotal;
```




<hr>



### property downloadProgress [1/2]

```C++
double QfCloudProject::downloadProgress;
```




<hr>



### property forceAutoPush [1/2]

```C++
bool QfCloudProject::forceAutoPush;
```




<hr>



### property id [1/2]

```C++
QString QfCloudProject::id;
```




<hr>



### property isFeatured [1/2]

```C++
bool QfCloudProject::isFeatured;
```




<hr>



### property isOutdated [1/2]

```C++
bool QfCloudProject::isOutdated;
```




<hr>



### property isProjectOutdated [1/2]

```C++
bool QfCloudProject::isProjectOutdated;
```




<hr>



### property isPublic [1/2]

```C++
bool QfCloudProject::isPublic;
```




<hr>



### property lastLocalExportedAt [1/2]

```C++
QDateTime QfCloudProject::lastLocalExportedAt;
```




<hr>



### property lastLocalPushDeltas [1/2]

```C++
QDateTime QfCloudProject::lastLocalPushDeltas;
```




<hr>



### property localPath [1/2]

```C++
QString QfCloudProject::localPath;
```




<hr>



### property name [1/2]

```C++
QString QfCloudProject::name;
```




<hr>



### property owner [1/2]

```C++
QString QfCloudProject::owner;
```




<hr>



### property packagedLayerErrors [1/2]

```C++
QStringList QfCloudProject::packagedLayerErrors;
```




<hr>



### property packagingStatus [1/2]

```C++
PackagingStatus QfCloudProject::packagingStatus;
```




<hr>



### property pushDeltaProgress [1/2]

```C++
double QfCloudProject::pushDeltaProgress;
```




<hr>



### property remoteSizeBytes [1/2]

```C++
qint64 QfCloudProject::remoteSizeBytes;
```




<hr>



### property status [1/2]

```C++
ProjectStatus QfCloudProject::status;
```




<hr>



### property thumbnailPath [1/2]

```C++
QString QfCloudProject::thumbnailPath;
```




<hr>



### property type [1/2]

```C++
ProjectType QfCloudProject::type;
```




<hr>



### property updatedAt [1/2]

```C++
QDateTime QfCloudProject::updatedAt;
```




<hr>



### property uploadBytesSent [1/2]

```C++
qint64 QfCloudProject::uploadBytesSent;
```




<hr>



### property uploadBytesTotal [1/2]

```C++
qint64 QfCloudProject::uploadBytesTotal;
```




<hr>



### property uploadProgress [1/2]

```C++
double QfCloudProject::uploadProgress;
```




<hr>



### property userRole [1/2]

```C++
QString QfCloudProject::userRole;
```




<hr>



### property userRoleOrigin [1/2]

```C++
QString QfCloudProject::userRoleOrigin;
```




<hr>
## Public Signals Documentation




### signal attachmentsOnDemandEnabledChanged 

```C++
void QfCloudProject::attachmentsOnDemandEnabledChanged;
```




<hr>



### signal autoPushEnabledChanged 

```C++
void QfCloudProject::autoPushEnabledChanged;
```




<hr>



### signal autoPushIntervalMinsChanged 

```C++
void QfCloudProject::autoPushIntervalMinsChanged;
```




<hr>



### signal canRepackageChanged 

```C++
void QfCloudProject::canRepackageChanged;
```




<hr>



### signal checkoutChanged 

```C++
void QfCloudProject::checkoutChanged;
```




<hr>



### signal createdAtChanged 

```C++
void QfCloudProject::createdAtChanged;
```




<hr>



### signal dataLastUpdatedAtChanged 

```C++
void QfCloudProject::dataLastUpdatedAtChanged;
```




<hr>



### signal dataRefreshed 

```C++
void QfCloudProject::dataRefreshed;
```




<hr>



### signal deltaFileIdChanged 

```C++
void QfCloudProject::deltaFileIdChanged;
```




<hr>



### signal deltaFilePushStatusChanged 

```C++
void QfCloudProject::deltaFilePushStatusChanged;
```




<hr>



### signal deltaFilePushStatusStringChanged 

```C++
void QfCloudProject::deltaFilePushStatusStringChanged;
```




<hr>



### signal deltaFileWrapperChanged 

```C++
void QfCloudProject::deltaFileWrapperChanged;
```




<hr>



### signal deltaLayersToDownloadChanged 

```C++
void QfCloudProject::deltaLayersToDownloadChanged;
```




<hr>



### signal deltasCountChanged 

```C++
void QfCloudProject::deltasCountChanged;
```




<hr>



### signal descriptionChanged 

```C++
void QfCloudProject::descriptionChanged;
```




<hr>



### signal downloadAttachmentFinished 

```C++
void QfCloudProject::downloadAttachmentFinished;
```




<hr>



### signal downloadBytesReceivedChanged 

```C++
void QfCloudProject::downloadBytesReceivedChanged;
```




<hr>



### signal downloadBytesTotalChanged 

```C++
void QfCloudProject::downloadBytesTotalChanged;
```




<hr>



### signal downloadFinished 

```C++
void QfCloudProject::downloadFinished;
```




<hr>



### signal downloadProgressChanged 

```C++
void QfCloudProject::downloadProgressChanged;
```




<hr>



### signal downloaded 

```C++
void QfCloudProject::downloaded;
```




<hr>



### signal errorStatusChanged 

```C++
void QfCloudProject::errorStatusChanged;
```




<hr>



### signal forceAutoPushChanged 

```C++
void QfCloudProject::forceAutoPushChanged;
```




<hr>



### signal idChanged 

```C++
void QfCloudProject::idChanged;
```




<hr>



### signal isFeaturedChanged 

```C++
void QfCloudProject::isFeaturedChanged;
```




<hr>



### signal isOutdatedChanged 

```C++
void QfCloudProject::isOutdatedChanged;
```




<hr>



### signal isPackagingActiveChanged 

```C++
void QfCloudProject::isPackagingActiveChanged;
```




<hr>



### signal isPackagingFailedChanged 

```C++
void QfCloudProject::isPackagingFailedChanged;
```




<hr>



### signal isProjectOutdatedChanged 

```C++
void QfCloudProject::isProjectOutdatedChanged;
```




<hr>



### signal isPublicChanged 

```C++
void QfCloudProject::isPublicChanged;
```




<hr>



### signal jobFinished 

```C++
void QfCloudProject::jobFinished;
```




<hr>



### signal lastDataLastUpdatedAtChanged 

```C++
void QfCloudProject::lastDataLastUpdatedAtChanged;
```




<hr>



### signal lastLocalDataLastUpdatedAtChanged 

```C++
void QfCloudProject::lastLocalDataLastUpdatedAtChanged;
```




<hr>



### signal lastLocalExportIdChanged 

```C++
void QfCloudProject::lastLocalExportIdChanged;
```




<hr>



### signal lastLocalExportedAtChanged 

```C++
void QfCloudProject::lastLocalExportedAtChanged;
```




<hr>



### signal lastLocalPushDeltasChanged 

```C++
void QfCloudProject::lastLocalPushDeltasChanged;
```




<hr>



### signal lastLocalRestrictedDataLastUpdatedAtChanged 

```C++
void QfCloudProject::lastLocalRestrictedDataLastUpdatedAtChanged;
```




<hr>



### signal lastRefreshedAtChanged 

```C++
void QfCloudProject::lastRefreshedAtChanged;
```




<hr>



### signal localPathChanged 

```C++
void QfCloudProject::localPathChanged;
```




<hr>



### signal modificationChanged 

```C++
void QfCloudProject::modificationChanged;
```




<hr>



### signal nameChanged 

```C++
void QfCloudProject::nameChanged;
```




<hr>



### signal needsRepackagingChanged 

```C++
void QfCloudProject::needsRepackagingChanged;
```




<hr>



### signal networkDeltaPushed 

```C++
void QfCloudProject::networkDeltaPushed;
```




<hr>



### signal networkDeltaStatusChecked 

```C++
void QfCloudProject::networkDeltaStatusChecked;
```




<hr>



### signal ownerChanged 

```C++
void QfCloudProject::ownerChanged;
```




<hr>



### signal packagedLayerErrorsChanged 

```C++
void QfCloudProject::packagedLayerErrorsChanged;
```




<hr>



### signal packagingStatusChanged 

```C++
void QfCloudProject::packagingStatusChanged;
```




<hr>



### signal packagingStatusStringChanged 

```C++
void QfCloudProject::packagingStatusStringChanged;
```




<hr>



### signal pushDeltaProgressChanged 

```C++
void QfCloudProject::pushDeltaProgressChanged;
```




<hr>



### signal pushFinished 

```C++
void QfCloudProject::pushFinished;
```




<hr>



### signal remoteSizeBytesChanged 

```C++
void QfCloudProject::remoteSizeBytesChanged;
```




<hr>



### signal restrictedDataLastUpdatedAtChanged 

```C++
void QfCloudProject::restrictedDataLastUpdatedAtChanged;
```




<hr>



### signal sharedDatasetsProjectIdChanged 

```C++
void QfCloudProject::sharedDatasetsProjectIdChanged;
```




<hr>



### signal statusChanged 

```C++
void QfCloudProject::statusChanged;
```




<hr>



### signal thumbnailPathChanged 

```C++
void QfCloudProject::thumbnailPathChanged;
```




<hr>



### signal typeChanged 

```C++
void QfCloudProject::typeChanged;
```




<hr>



### signal updatedAtChanged 

```C++
void QfCloudProject::updatedAtChanged;
```




<hr>



### signal uploadBytesSentChanged 

```C++
void QfCloudProject::uploadBytesSentChanged;
```




<hr>



### signal uploadBytesTotalChanged 

```C++
void QfCloudProject::uploadBytesTotalChanged;
```




<hr>



### signal uploadFinished 

```C++
void QfCloudProject::uploadFinished;
```




<hr>



### signal uploadProgressChanged 

```C++
void QfCloudProject::uploadProgressChanged;
```




<hr>



### signal userRoleChanged 

```C++
void QfCloudProject::userRoleChanged;
```




<hr>



### signal userRoleOriginChanged 

```C++
void QfCloudProject::userRoleOriginChanged;
```




<hr>
## Public Functions Documentation




### function QfCloudProject 

```C++
QfCloudProject::QfCloudProject (
    const QString & id=QString(),
    QfCloudConnection * connection=nullptr,
    QgsGpkgFlusher * gpkgFlusher=nullptr
) 
```




<hr>



### function attachmentsOnDemandEnabled [2/2]

```C++
inline bool QfCloudProject::attachmentsOnDemandEnabled () const
```




<hr>



### function autoPushEnabled [2/2]

```C++
inline bool QfCloudProject::autoPushEnabled () const
```




<hr>



### function autoPushIntervalMins [2/2]

```C++
inline int QfCloudProject::autoPushIntervalMins () const
```




<hr>



### function canRepackage 

```C++
inline bool QfCloudProject::canRepackage () const
```




<hr>



### function cancelDownload 

```C++
void QfCloudProject::cancelDownload () 
```




<hr>



### function cancelPush 

```C++
void QfCloudProject::cancelPush () 
```




<hr>



### function checkout 

```C++
inline ProjectCheckouts QfCloudProject::checkout () const
```




<hr>



### function createdAt [2/2]

```C++
inline QDateTime QfCloudProject::createdAt () const
```




<hr>



### function dataLastUpdatedAt [2/2]

```C++
inline QDateTime QfCloudProject::dataLastUpdatedAt () const
```




<hr>



### function deltaFileId 

```C++
inline QString QfCloudProject::deltaFileId () const
```




<hr>



### function deltaFilePushStatus [2/2]

```C++
inline DeltaFileStatus QfCloudProject::deltaFilePushStatus () const
```




<hr>



### function deltaFilePushStatusString 

```C++
inline QString QfCloudProject::deltaFilePushStatusString () const
```




<hr>



### function deltaFileWrapper [2/2]

```C++
inline QfDeltaFileWrapper * QfCloudProject::deltaFileWrapper () const
```




<hr>



### function deltaLayersToDownload 

```C++
inline QStringList QfCloudProject::deltaLayersToDownload () const
```




<hr>



### function deltasCount [2/2]

```C++
inline int QfCloudProject::deltasCount () const
```




<hr>



### function description [2/2]

```C++
inline QString QfCloudProject::description () const
```




<hr>



### function downloadAttachment 

```C++
Q_INVOKABLE void QfCloudProject::downloadAttachment (
    const QString & fileName
) 
```




<hr>



### function downloadBytesReceived [2/2]

```C++
inline qint64 QfCloudProject::downloadBytesReceived () const
```




<hr>



### function downloadBytesTotal [2/2]

```C++
inline qint64 QfCloudProject::downloadBytesTotal () const
```




<hr>



### function downloadProgress [2/2]

```C++
inline double QfCloudProject::downloadProgress () const
```




<hr>



### function downloadThumbnail 

```C++
Q_INVOKABLE void QfCloudProject::downloadThumbnail () 
```




<hr>



### function ensureProjectCreated 

```C++
void QfCloudProject::ensureProjectCreated () 
```




<hr>



### function errorStatus 

```C++
inline ProjectErrorStatus QfCloudProject::errorStatus () const
```




<hr>



### function forceAutoPush [2/2]

```C++
inline bool QfCloudProject::forceAutoPush () const
```




<hr>



### function id [2/2]

```C++
inline QString QfCloudProject::id () const
```




<hr>



### function isFeatured [2/2]

```C++
inline bool QfCloudProject::isFeatured () const
```




<hr>



### function isOutdated [2/2]

```C++
inline bool QfCloudProject::isOutdated () const
```




<hr>



### function isPackagingActive 

```C++
inline bool QfCloudProject::isPackagingActive () const
```




<hr>



### function isPackagingFailed 

```C++
inline bool QfCloudProject::isPackagingFailed () const
```




<hr>



### function isProjectOutdated [2/2]

```C++
inline bool QfCloudProject::isProjectOutdated () const
```




<hr>



### function isPublic [2/2]

```C++
inline bool QfCloudProject::isPublic () const
```




<hr>



### function lastLocalDataLastUpdatedAt 

```C++
inline QDateTime QfCloudProject::lastLocalDataLastUpdatedAt () const
```




<hr>



### function lastLocalExportId 

```C++
inline QString QfCloudProject::lastLocalExportId () const
```




<hr>



### function lastLocalExportedAt [2/2]

```C++
inline QDateTime QfCloudProject::lastLocalExportedAt () const
```




<hr>



### function lastLocalPushDeltas [2/2]

```C++
inline QDateTime QfCloudProject::lastLocalPushDeltas () const
```




<hr>



### function lastLocalRestrictedDataLastUpdatedAt 

```C++
inline QDateTime QfCloudProject::lastLocalRestrictedDataLastUpdatedAt () const
```




<hr>



### function lastRefreshedAt 

```C++
inline QDateTime QfCloudProject::lastRefreshedAt () const
```




<hr>



### function localPath [2/2]

```C++
inline QString QfCloudProject::localPath () const
```




<hr>



### function modification 

```C++
inline ProjectModifications QfCloudProject::modification () const
```




<hr>



### function name [2/2]

```C++
inline QString QfCloudProject::name () const
```




<hr>



### function needsRepackaging 

```C++
inline bool QfCloudProject::needsRepackaging () const
```




<hr>



### function owner [2/2]

```C++
inline QString QfCloudProject::owner () const
```




<hr>



### function packageAndDownload 

```C++
Q_INVOKABLE void QfCloudProject::packageAndDownload () 
```




<hr>



### function packagedLayerErrors [2/2]

```C++
inline QStringList QfCloudProject::packagedLayerErrors () const
```




<hr>



### function packagingStatus [2/2]

```C++
inline PackagingStatus QfCloudProject::packagingStatus () const
```




<hr>



### function packagingStatusString 

```C++
inline QString QfCloudProject::packagingStatusString () const
```




<hr>



### function push 

```C++
Q_INVOKABLE void QfCloudProject::push (
    bool shouldDownloadUpdates
) 
```




<hr>



### function pushDeltaProgress [2/2]

```C++
inline double QfCloudProject::pushDeltaProgress () const
```




<hr>



### function refreshDeltaList 

```C++
void QfCloudProject::refreshDeltaList () 
```




<hr>



### function refreshModification 

```C++
void QfCloudProject::refreshModification () 
```




<hr>



### function remoteSizeBytes [2/2]

```C++
inline qint64 QfCloudProject::remoteSizeBytes () const
```




<hr>



### function removeLocally 

```C++
void QfCloudProject::removeLocally () 
```




<hr>



### function restrictedDataLastUpdatedAt 

```C++
inline QDateTime QfCloudProject::restrictedDataLastUpdatedAt () const
```




<hr>



### function setAttachmentsOnDemandEnabled 

```C++
void QfCloudProject::setAttachmentsOnDemandEnabled (
    bool enabled
) 
```




<hr>



### function setAutoPushEnabled 

```C++
void QfCloudProject::setAutoPushEnabled (
    bool enabled
) 
```




<hr>



### function setAutoPushIntervalMins 

```C++
void QfCloudProject::setAutoPushIntervalMins (
    int minutes
) 
```




<hr>



### function setCanRepackage 

```C++
void QfCloudProject::setCanRepackage (
    bool canRepackage
) 
```




<hr>



### function setCheckout 

```C++
void QfCloudProject::setCheckout (
    ProjectCheckouts checkout
) 
```




<hr>



### function setCreatedAt 

```C++
void QfCloudProject::setCreatedAt (
    const QDateTime & createdAt
) 
```




<hr>



### function setDataLastUpdatedAt 

```C++
void QfCloudProject::setDataLastUpdatedAt (
    const QDateTime & dataLastUpdatedAt
) 
```




<hr>



### function setDeltaFileId 

```C++
void QfCloudProject::setDeltaFileId (
    const QString & deltaFileId
) 
```




<hr>



### function setDeltaFilePushStatus 

```C++
void QfCloudProject::setDeltaFilePushStatus (
    DeltaFileStatus deltaFilePushStatus
) 
```




<hr>



### function setDeltaFilePushStatusString 

```C++
void QfCloudProject::setDeltaFilePushStatusString (
    const QString & deltaFilePushStatusString
) 
```




<hr>



### function setDeltaLayersToDownload 

```C++
void QfCloudProject::setDeltaLayersToDownload (
    const QStringList & deltaLayersToDownload
) 
```




<hr>



### function setDescription 

```C++
void QfCloudProject::setDescription (
    const QString & description
) 
```




<hr>



### function setErrorStatus 

```C++
void QfCloudProject::setErrorStatus (
    ProjectErrorStatus errorStatus
) 
```




<hr>



### function setForceAutoPush 

```C++
void QfCloudProject::setForceAutoPush (
    bool force
) 
```




<hr>



### function setIsFeatured 

```C++
void QfCloudProject::setIsFeatured (
    bool isFeatured
) 
```




<hr>



### function setIsOutdated 

```C++
void QfCloudProject::setIsOutdated (
    bool isOutdated
) 
```




<hr>



### function setIsPackagingActive 

```C++
void QfCloudProject::setIsPackagingActive (
    bool isPackagingActive
) 
```




<hr>



### function setIsPackagingFailed 

```C++
void QfCloudProject::setIsPackagingFailed (
    bool isPackagingFailed
) 
```




<hr>



### function setIsProjectOutdated 

```C++
void QfCloudProject::setIsProjectOutdated (
    bool isProjectOutdated
) 
```




<hr>



### function setIsPublic 

```C++
void QfCloudProject::setIsPublic (
    bool isPublic
) 
```




<hr>



### function setLastLocalDataLastUpdatedAt 

```C++
void QfCloudProject::setLastLocalDataLastUpdatedAt (
    const QDateTime & lastLocalDataLastUpdatedAt
) 
```




<hr>



### function setLastLocalExportId 

```C++
void QfCloudProject::setLastLocalExportId (
    const QString & lastLocalExportId
) 
```




<hr>



### function setLastLocalExportedAt 

```C++
void QfCloudProject::setLastLocalExportedAt (
    const QDateTime & lastLocalExportedAt
) 
```




<hr>



### function setLastLocalPushDeltas 

```C++
void QfCloudProject::setLastLocalPushDeltas (
    const QDateTime & lastLocalPushDeltas
) 
```




<hr>



### function setLastLocalRestrictedDataLastUpdatedAt 

```C++
void QfCloudProject::setLastLocalRestrictedDataLastUpdatedAt (
    const QDateTime & lastLocalRestrictedDataLastUpdatedAt
) 
```




<hr>



### function setLastRefreshedAt 

```C++
void QfCloudProject::setLastRefreshedAt (
    const QDateTime & lastRefreshedAt
) 
```




<hr>



### function setLocalPath 

```C++
void QfCloudProject::setLocalPath (
    const QString & localPath
) 
```




<hr>



### function setModification 

```C++
void QfCloudProject::setModification (
    ProjectModification modification
) 
```




<hr>



### function setName 

```C++
void QfCloudProject::setName (
    const QString & name
) 
```




<hr>



### function setNeedsRepackaging 

```C++
void QfCloudProject::setNeedsRepackaging (
    bool needsRepackaging
) 
```




<hr>



### function setOwner 

```C++
void QfCloudProject::setOwner (
    const QString & owner
) 
```




<hr>



### function setPackagedLayerErrors 

```C++
void QfCloudProject::setPackagedLayerErrors (
    const QStringList & packagedLayerErrors
) 
```




<hr>



### function setPackagingStatus 

```C++
void QfCloudProject::setPackagingStatus (
    PackagingStatus packagingStatus
) 
```




<hr>



### function setPackagingStatusString 

```C++
void QfCloudProject::setPackagingStatusString (
    const QString & packagingStatusString
) 
```




<hr>



### function setRemoteSizeBytes 

```C++
void QfCloudProject::setRemoteSizeBytes (
    qint64 remoteSizeBytes
) 
```




<hr>



### function setRestrictedDataLastUpdatedAt 

```C++
void QfCloudProject::setRestrictedDataLastUpdatedAt (
    const QDateTime & restrictedDataLastUpdatedAt
) 
```




<hr>



### function setSharedDatasetsProjectId 

```C++
void QfCloudProject::setSharedDatasetsProjectId (
    const QString & id
) 
```




<hr>



### function setStatus 

```C++
void QfCloudProject::setStatus (
    ProjectStatus status
) 
```




<hr>



### function setThumbnailPath 

```C++
void QfCloudProject::setThumbnailPath (
    const QString & thumbnailPath
) 
```




<hr>



### function setType 

```C++
void QfCloudProject::setType (
    ProjectType projectType
) 
```




<hr>



### function setUpdatedAt 

```C++
void QfCloudProject::setUpdatedAt (
    const QDateTime & updatedAt
) 
```




<hr>



### function setUserRole 

```C++
void QfCloudProject::setUserRole (
    const QString & userRole
) 
```




<hr>



### function setUserRoleOrigin 

```C++
void QfCloudProject::setUserRoleOrigin (
    const QString & userRoleOrigin
) 
```




<hr>



### function sharedDatasetsProjectId 

```C++
inline QString QfCloudProject::sharedDatasetsProjectId () const
```




<hr>



### function status [2/2]

```C++
inline ProjectStatus QfCloudProject::status () const
```




<hr>



### function thumbnailPath [2/2]

```C++
inline QString QfCloudProject::thumbnailPath () const
```




<hr>



### function type [2/2]

```C++
inline ProjectType QfCloudProject::type () const
```




<hr>



### function updatedAt [2/2]

```C++
inline QDateTime QfCloudProject::updatedAt () const
```




<hr>



### function uploadBytesSent [2/2]

```C++
inline qint64 QfCloudProject::uploadBytesSent () const
```




<hr>



### function uploadBytesTotal [2/2]

```C++
inline qint64 QfCloudProject::uploadBytesTotal () const
```




<hr>



### function uploadLocalPath 

```C++
Q_INVOKABLE void QfCloudProject::uploadLocalPath (
    QString localPath,
    bool deleteAfterSuccessfulUpload=false
) 
```




<hr>



### function uploadProgress [2/2]

```C++
inline double QfCloudProject::uploadProgress () const
```




<hr>



### function userRole [2/2]

```C++
inline QString QfCloudProject::userRole () const
```




<hr>



### function userRoleOrigin [2/2]

```C++
inline QString QfCloudProject::userRoleOrigin () const
```




<hr>



### function ~QfCloudProject 

```C++
QfCloudProject::~QfCloudProject () = default
```




<hr>
## Public Static Functions Documentation




### function fromDetails 

```C++
static QfCloudProject * QfCloudProject::fromDetails (
    const QVariantHash & details,
    QfCloudConnection * connection,
    QgsGpkgFlusher * gpkgFlusher=nullptr
) 
```




<hr>



### function fromLocalSettings 

```C++
static QfCloudProject * QfCloudProject::fromLocalSettings (
    const QString & id,
    QfCloudConnection * connection,
    QgsGpkgFlusher * gpkgFlusher=nullptr
) 
```




<hr>



### function getJobStatusFromString 

```C++
static QfCloudProject::JobStatus QfCloudProject::getJobStatusFromString (
    const QString & status
) 
```




<hr>



### function getJobTypeAsString 

```C++
static QString QfCloudProject::getJobTypeAsString (
    QfCloudProject::JobType jobType
) 
```




<hr>



### function restoreLocalSettings 

```C++
static void QfCloudProject::restoreLocalSettings (
    QfCloudProject * project,
    const QDir & localPath
) 
```




<hr>



### function typeFromString 

_Returns the ProjectType matching the given_ _type_ _string; unknown or empty values return ProjectType::Regular._
```C++
static QfCloudProject::ProjectType QfCloudProject::typeFromString (
    const QString & type
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfcloudproject.h`

