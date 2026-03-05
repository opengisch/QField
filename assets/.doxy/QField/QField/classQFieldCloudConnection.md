

# Class QFieldCloudConnection



[**ClassList**](annotated.md) **>** [**QFieldCloudConnection**](classQFieldCloudConnection.md)








Inherits the following classes: QObject












## Classes

| Type | Name |
| ---: | :--- |
| class | [**CloudError**](classQFieldCloudConnection_1_1CloudError.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ConnectionState**](#enum-connectionstate)  <br> |
| enum  | [**ConnectionStatus**](#enum-connectionstatus)  <br> |






## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  QMap&lt; QString, QString &gt; | [**sErrors**](#variable-serrors)   = `/* multi line expression */`<br>_Contains a map of error codes and translated messages._  |






## Public Properties

| Type | Name |
| ---: | :--- |
| property QList&lt; [**AuthenticationProvider**](classAuthenticationProvider.md) &gt; | [**availableProviders**](classQFieldCloudConnection.md#property-availableproviders-12)  <br> |
| property QString | [**avatarUrl**](classQFieldCloudConnection.md#property-avatarurl-12)  <br> |
| property QString | [**defaultUrl**](classQFieldCloudConnection.md#property-defaulturl-12)  <br> |
| property bool | [**hasProviderConfiguration**](classQFieldCloudConnection.md#property-hasproviderconfiguration-12)  <br> |
| property bool | [**hasToken**](classQFieldCloudConnection.md#property-hastoken-12)  <br> |
| property bool | [**isFetchingAvailableProviders**](classQFieldCloudConnection.md#property-isfetchingavailableproviders-12)  <br> |
| property bool | [**isReachable**](classQFieldCloudConnection.md#property-isreachable-12)  <br> |
| property QString | [**password**](classQFieldCloudConnection.md#property-password-12)  <br> |
| property QString | [**provider**](classQFieldCloudConnection.md#property-provider-12)  <br> |
| property ConnectionState | [**state**](classQFieldCloudConnection.md#property-state-12)  <br> |
| property ConnectionStatus | [**status**](classQFieldCloudConnection.md#property-status-12)  <br> |
| property QString | [**url**](classQFieldCloudConnection.md#property-url-12)  <br> |
| property QStringList | [**urls**](classQFieldCloudConnection.md#property-urls-12)  <br> |
| property [**CloudUserInformation**](structCloudUserInformation.md) | [**userInformation**](classQFieldCloudConnection.md#property-userinformation-12)  <br> |
| property QString | [**username**](classQFieldCloudConnection.md#property-username-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**availableProvidersChanged**](classQFieldCloudConnection.md#signal-availableproviderschanged)  <br> |
| signal void | [**avatarUrlChanged**](classQFieldCloudConnection.md#signal-avatarurlchanged)  <br> |
| signal void | [**error**](classQFieldCloudConnection.md#signal-error)  <br> |
| signal void | [**isFetchingAvailableProvidersChanged**](classQFieldCloudConnection.md#signal-isfetchingavailableproviderschanged)  <br> |
| signal void | [**isReachableChanged**](classQFieldCloudConnection.md#signal-isreachablechanged)  <br> |
| signal void | [**loginFailed**](classQFieldCloudConnection.md#signal-loginfailed) (const QString & reason) <br> |
| signal void | [**passwordChanged**](classQFieldCloudConnection.md#signal-passwordchanged)  <br> |
| signal void | [**pendingAttachmentsAdded**](classQFieldCloudConnection.md#signal-pendingattachmentsadded)  <br> |
| signal void | [**pendingAttachmentsUploadFinished**](classQFieldCloudConnection.md#signal-pendingattachmentsuploadfinished) (const QString & error=QString()) <br> |
| signal void | [**pendingAttachmentsUploadStatus**](classQFieldCloudConnection.md#signal-pendingattachmentsuploadstatus) (const QString & fileName, double fileProgress, qsizetype uploadPending) <br> |
| signal void | [**providerChanged**](classQFieldCloudConnection.md#signal-providerchanged)  <br> |
| signal void | [**providerConfigurationChanged**](classQFieldCloudConnection.md#signal-providerconfigurationchanged)  <br> |
| signal void | [**queuedProjectPushRequested**](classQFieldCloudConnection.md#signal-queuedprojectpushrequested) (const QString & projectId) <br> |
| signal void | [**stateChanged**](classQFieldCloudConnection.md#signal-statechanged)  <br> |
| signal void | [**statusChanged**](classQFieldCloudConnection.md#signal-statuschanged)  <br> |
| signal void | [**tokenChanged**](classQFieldCloudConnection.md#signal-tokenchanged)  <br> |
| signal void | [**urlChanged**](classQFieldCloudConnection.md#signal-urlchanged)  <br> |
| signal void | [**urlsChanged**](classQFieldCloudConnection.md#signal-urlschanged)  <br> |
| signal void | [**userInformationChanged**](classQFieldCloudConnection.md#signal-userinformationchanged)  <br> |
| signal void | [**usernameChanged**](classQFieldCloudConnection.md#signal-usernamechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QFieldCloudConnection**](#function-qfieldcloudconnection) () <br> |
|  QList&lt; [**AuthenticationProvider**](classAuthenticationProvider.md) &gt; | [**availableProviders**](#function-availableproviders-22) () const<br> |
|  QString | [**avatarUrl**](#function-avatarurl-22) () const<br> |
|  [**NetworkReply**](classNetworkReply.md) \* | [**get**](#function-get-13) (const QString & endpoint, const QVariantMap & params=QVariantMap()) <br> |
|  [**NetworkReply**](classNetworkReply.md) \* | [**get**](#function-get-23) (QNetworkRequest & request, const QString & endpoint, const QVariantMap & params=QVariantMap()) <br> |
|  [**NetworkReply**](classNetworkReply.md) \* | [**get**](#function-get-33) (QNetworkRequest & request, const QUrl & url, const QVariantMap & params=QVariantMap()) <br> |
|  Q\_INVOKABLE void | [**getAuthenticationProviders**](#function-getauthenticationproviders) () <br> |
|  bool | [**hasProviderConfiguration**](#function-hasproviderconfiguration-22) () <br> |
|  bool | [**hasToken**](#function-hastoken-22) () <br> |
|  bool | [**isFetchingAvailableProviders**](#function-isfetchingavailableproviders-22) () const<br> |
|  bool | [**isReachable**](#function-isreachable-22) () const<br> |
|  Q\_INVOKABLE void | [**login**](#function-login) (const QString & password=QString()) <br> |
|  Q\_INVOKABLE void | [**logout**](#function-logout) () <br> |
|  QString | [**password**](#function-password-22) () const<br> |
|  [**NetworkReply**](classNetworkReply.md) \* | [**post**](#function-post-12) (const QString & endpoint, const QVariantMap & params=QVariantMap(), const QStringList & fileNames=QStringList()) <br> |
|  [**NetworkReply**](classNetworkReply.md) \* | [**post**](#function-post-22) (QNetworkRequest & request, const QString & endpoint, const QVariantMap & params=QVariantMap(), const QStringList & fileNames=QStringList()) <br> |
|  QString | [**provider**](#function-provider-22) () const<br> |
|  void | [**queueProjectPush**](#function-queueprojectpush) (const QString & projectId) <br> |
|  void | [**setAuthenticationDetails**](#function-setauthenticationdetails) (QNetworkRequest & request) <br> |
|  void | [**setPassword**](#function-setpassword) (const QString & password) <br> |
|  void | [**setProvider**](#function-setprovider) (const QString & provider) <br> |
|  void | [**setUrl**](#function-seturl) (const QString & url) <br> |
|  void | [**setUsername**](#function-setusername) (const QString & username) <br> |
|  ConnectionState | [**state**](#function-state-22) () const<br> |
|  ConnectionStatus | [**status**](#function-status-22) () const<br> |
|  QString | [**token**](#function-token) () const<br> |
|  qsizetype | [**uploadPendingAttachments**](#function-uploadpendingattachments) () <br> |
|  QString | [**url**](#function-url-22) () const<br> |
|  QStringList | [**urls**](#function-urls-22) () const<br> |
|  [**CloudUserInformation**](structCloudUserInformation.md) | [**userInformation**](#function-userinformation-22) () const<br> |
|  QString | [**username**](#function-username-22) () const<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  QString | [**defaultUrl**](#function-defaulturl-22) () <br> |
|  QString | [**errorString**](#function-errorstring) (QNetworkReply \* reply) <br>_Returns an error string to be shown to the user if_ _reply_ _has an error._ |


























## Public Types Documentation




### enum ConnectionState 

```C++
enum QFieldCloudConnection::ConnectionState {
    Idle,
    Busy
};
```




<hr>



### enum ConnectionStatus 

```C++
enum QFieldCloudConnection::ConnectionStatus {
    Disconnected,
    Connecting,
    LoggedIn
};
```




<hr>
## Public Static Attributes Documentation




### variable sErrors 

_Contains a map of error codes and translated messages._ 
```C++
QMap< QString, QString > QFieldCloudConnection::sErrors;
```




<hr>
## Public Properties Documentation




### property availableProviders [1/2]

```C++
QList<AuthenticationProvider> QFieldCloudConnection::availableProviders;
```




<hr>



### property avatarUrl [1/2]

```C++
QString QFieldCloudConnection::avatarUrl;
```




<hr>



### property defaultUrl [1/2]

```C++
QString QFieldCloudConnection::defaultUrl;
```




<hr>



### property hasProviderConfiguration [1/2]

```C++
bool QFieldCloudConnection::hasProviderConfiguration;
```




<hr>



### property hasToken [1/2]

```C++
bool QFieldCloudConnection::hasToken;
```




<hr>



### property isFetchingAvailableProviders [1/2]

```C++
bool QFieldCloudConnection::isFetchingAvailableProviders;
```




<hr>



### property isReachable [1/2]

```C++
bool QFieldCloudConnection::isReachable;
```




<hr>



### property password [1/2]

```C++
QString QFieldCloudConnection::password;
```




<hr>



### property provider [1/2]

```C++
QString QFieldCloudConnection::provider;
```




<hr>



### property state [1/2]

```C++
ConnectionState QFieldCloudConnection::state;
```




<hr>



### property status [1/2]

```C++
ConnectionStatus QFieldCloudConnection::status;
```




<hr>



### property url [1/2]

```C++
QString QFieldCloudConnection::url;
```




<hr>



### property urls [1/2]

```C++
QStringList QFieldCloudConnection::urls;
```




<hr>



### property userInformation [1/2]

```C++
CloudUserInformation QFieldCloudConnection::userInformation;
```




<hr>



### property username [1/2]

```C++
QString QFieldCloudConnection::username;
```




<hr>
## Public Signals Documentation




### signal availableProvidersChanged 

```C++
void QFieldCloudConnection::availableProvidersChanged;
```




<hr>



### signal avatarUrlChanged 

```C++
void QFieldCloudConnection::avatarUrlChanged;
```




<hr>



### signal error 

```C++
void QFieldCloudConnection::error;
```




<hr>



### signal isFetchingAvailableProvidersChanged 

```C++
void QFieldCloudConnection::isFetchingAvailableProvidersChanged;
```




<hr>



### signal isReachableChanged 

```C++
void QFieldCloudConnection::isReachableChanged;
```




<hr>



### signal loginFailed 

```C++
void QFieldCloudConnection::loginFailed;
```




<hr>



### signal passwordChanged 

```C++
void QFieldCloudConnection::passwordChanged;
```




<hr>



### signal pendingAttachmentsAdded 

```C++
void QFieldCloudConnection::pendingAttachmentsAdded;
```




<hr>



### signal pendingAttachmentsUploadFinished 

```C++
void QFieldCloudConnection::pendingAttachmentsUploadFinished;
```




<hr>



### signal pendingAttachmentsUploadStatus 

```C++
void QFieldCloudConnection::pendingAttachmentsUploadStatus;
```




<hr>



### signal providerChanged 

```C++
void QFieldCloudConnection::providerChanged;
```




<hr>



### signal providerConfigurationChanged 

```C++
void QFieldCloudConnection::providerConfigurationChanged;
```




<hr>



### signal queuedProjectPushRequested 

```C++
void QFieldCloudConnection::queuedProjectPushRequested;
```




<hr>



### signal stateChanged 

```C++
void QFieldCloudConnection::stateChanged;
```




<hr>



### signal statusChanged 

```C++
void QFieldCloudConnection::statusChanged;
```




<hr>



### signal tokenChanged 

```C++
void QFieldCloudConnection::tokenChanged;
```




<hr>



### signal urlChanged 

```C++
void QFieldCloudConnection::urlChanged;
```




<hr>



### signal urlsChanged 

```C++
void QFieldCloudConnection::urlsChanged;
```




<hr>



### signal userInformationChanged 

```C++
void QFieldCloudConnection::userInformationChanged;
```




<hr>



### signal usernameChanged 

```C++
void QFieldCloudConnection::usernameChanged;
```




<hr>
## Public Functions Documentation




### function QFieldCloudConnection 

```C++
QFieldCloudConnection::QFieldCloudConnection () 
```




<hr>



### function availableProviders [2/2]

```C++
QList< AuthenticationProvider > QFieldCloudConnection::availableProviders () const
```




<hr>



### function avatarUrl [2/2]

```C++
QString QFieldCloudConnection::avatarUrl () const
```




<hr>



### function get [1/3]

```C++
NetworkReply * QFieldCloudConnection::get (
    const QString & endpoint,
    const QVariantMap & params=QVariantMap()
) 
```



Sends a get request to the given _endpoint_. Query can be passed via _params_, empty by default.


If this connection is not logged in, will return nullptr. The returned reply needs to be deleted by the caller. 


        

<hr>



### function get [2/3]

```C++
NetworkReply * QFieldCloudConnection::get (
    QNetworkRequest & request,
    const QString & endpoint,
    const QVariantMap & params=QVariantMap()
) 
```



Sends a get _request_ to a given _endpoint_. Additional query can be passed via _params_, empty by default.


If this connection is not logged in, will return nullptr. The returned reply needs to be deleted by the caller. 


        

<hr>



### function get [3/3]

```C++
NetworkReply * QFieldCloudConnection::get (
    QNetworkRequest & request,
    const QUrl & url,
    const QVariantMap & params=QVariantMap()
) 
```




<hr>



### function getAuthenticationProviders 

```C++
Q_INVOKABLE void QFieldCloudConnection::getAuthenticationProviders () 
```




<hr>



### function hasProviderConfiguration [2/2]

```C++
inline bool QFieldCloudConnection::hasProviderConfiguration () 
```




<hr>



### function hasToken [2/2]

```C++
inline bool QFieldCloudConnection::hasToken () 
```




<hr>



### function isFetchingAvailableProviders [2/2]

```C++
bool QFieldCloudConnection::isFetchingAvailableProviders () const
```




<hr>



### function isReachable [2/2]

```C++
bool QFieldCloudConnection::isReachable () const
```



Returns whether the network is currently reachable.


If reachability information is not available, this returns true to keep the existing behavior unchanged. 


        

<hr>



### function login 

```C++
Q_INVOKABLE void QFieldCloudConnection::login (
    const QString & password=QString()
) 
```




<hr>



### function logout 

```C++
Q_INVOKABLE void QFieldCloudConnection::logout () 
```




<hr>



### function password [2/2]

```C++
QString QFieldCloudConnection::password () const
```




<hr>



### function post [1/2]

```C++
NetworkReply * QFieldCloudConnection::post (
    const QString & endpoint,
    const QVariantMap & params=QVariantMap(),
    const QStringList & fileNames=QStringList()
) 
```



Sends a post request with the given _parameters_ to the given _endpoint_.


If this connection is not logged in, will return nullptr. The returned reply needs to be deleted by the caller. 


        

<hr>



### function post [2/2]

```C++
NetworkReply * QFieldCloudConnection::post (
    QNetworkRequest & request,
    const QString & endpoint,
    const QVariantMap & params=QVariantMap(),
    const QStringList & fileNames=QStringList()
) 
```



Sends a post _request_ with the given _parameters_ to the given _endpoint_.


If this connection is not logged in, will return nullptr. The returned reply needs to be deleted by the caller. 


        

<hr>



### function provider [2/2]

```C++
QString QFieldCloudConnection::provider () const
```




<hr>



### function queueProjectPush 

```C++
void QFieldCloudConnection::queueProjectPush (
    const QString & projectId
) 
```



Queues a project push request when the network is not reachable.


The push will be automatically triggered once the connection is back online and the user is logged in. 


        

<hr>



### function setAuthenticationDetails 

```C++
void QFieldCloudConnection::setAuthenticationDetails (
    QNetworkRequest & request
) 
```



Sets authentication details on a _request_. 


        

<hr>



### function setPassword 

```C++
void QFieldCloudConnection::setPassword (
    const QString & password
) 
```




<hr>



### function setProvider 

```C++
void QFieldCloudConnection::setProvider (
    const QString & provider
) 
```




<hr>



### function setUrl 

```C++
void QFieldCloudConnection::setUrl (
    const QString & url
) 
```



Sets the current server connection URL and saves it into QSettings. 


        

<hr>



### function setUsername 

```C++
void QFieldCloudConnection::setUsername (
    const QString & username
) 
```




<hr>



### function state [2/2]

```C++
ConnectionState QFieldCloudConnection::state () const
```




<hr>



### function status [2/2]

```C++
ConnectionStatus QFieldCloudConnection::status () const
```




<hr>



### function token 

```C++
QString QFieldCloudConnection::token () const
```




<hr>



### function uploadPendingAttachments 

```C++
qsizetype QFieldCloudConnection::uploadPendingAttachments () 
```



Uploads any pending attachments linked to the logged in user account. 

**Returns:**

the number of attachments to be uploaded. 





        

<hr>



### function url [2/2]

```C++
QString QFieldCloudConnection::url () const
```



Returns the currently set server connection URL. 


        

<hr>



### function urls [2/2]

```C++
QStringList QFieldCloudConnection::urls () const
```



Returns the connections URLs successfully logged in in the past. 


        

<hr>



### function userInformation [2/2]

```C++
CloudUserInformation QFieldCloudConnection::userInformation () const
```




<hr>



### function username [2/2]

```C++
QString QFieldCloudConnection::username () const
```




<hr>
## Public Static Functions Documentation




### function defaultUrl [2/2]

```C++
static QString QFieldCloudConnection::defaultUrl () 
```



Default server connection URL, pointing to the production server. 


        

<hr>



### function errorString 

_Returns an error string to be shown to the user if_ _reply_ _has an error._
```C++
static QString QFieldCloudConnection::errorString (
    QNetworkReply * reply
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfieldcloudconnection.h`

