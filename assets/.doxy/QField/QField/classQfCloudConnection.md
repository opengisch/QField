

# Class QfCloudConnection



[**ClassList**](annotated.md) **>** [**QfCloudConnection**](classQfCloudConnection.md)








Inherits the following classes: QObject












## Classes

| Type | Name |
| ---: | :--- |
| class | [**CloudError**](classQfCloudConnection_1_1CloudError.md) <br> |


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
| property QList&lt; [**QfAuthenticationProvider**](classQfAuthenticationProvider.md) &gt; | [**availableProviders**](classQfCloudConnection.md#property-availableproviders-12)  <br> |
| property QString | [**avatarUrl**](classQfCloudConnection.md#property-avatarurl-12)  <br> |
| property QString | [**defaultUrl**](classQfCloudConnection.md#property-defaulturl-12)  <br> |
| property bool | [**hasProviderConfiguration**](classQfCloudConnection.md#property-hasproviderconfiguration-12)  <br> |
| property bool | [**hasToken**](classQfCloudConnection.md#property-hastoken-12)  <br> |
| property bool | [**isFetchingAvailableProviders**](classQfCloudConnection.md#property-isfetchingavailableproviders-12)  <br> |
| property bool | [**isReachable**](classQfCloudConnection.md#property-isreachable-12)  <br> |
| property QString | [**password**](classQfCloudConnection.md#property-password-12)  <br> |
| property QString | [**provider**](classQfCloudConnection.md#property-provider-12)  <br> |
| property [**QfCloudServerInformation**](structQfCloudServerInformation.md) | [**serverInformation**](classQfCloudConnection.md#property-serverinformation-12)  <br> |
| property ConnectionState | [**state**](classQfCloudConnection.md#property-state-12)  <br> |
| property ConnectionStatus | [**status**](classQfCloudConnection.md#property-status-12)  <br> |
| property QString | [**url**](classQfCloudConnection.md#property-url-12)  <br> |
| property QStringList | [**urls**](classQfCloudConnection.md#property-urls-12)  <br> |
| property [**QfCloudUserInformation**](structQfCloudUserInformation.md) | [**userInformation**](classQfCloudConnection.md#property-userinformation-12)  <br> |
| property QString | [**username**](classQfCloudConnection.md#property-username-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**availableProvidersChanged**](classQfCloudConnection.md#signal-availableproviderschanged)  <br> |
| signal void | [**avatarUrlChanged**](classQfCloudConnection.md#signal-avatarurlchanged)  <br> |
| signal void | [**error**](classQfCloudConnection.md#signal-error)  <br> |
| signal void | [**isFetchingAvailableProvidersChanged**](classQfCloudConnection.md#signal-isfetchingavailableproviderschanged)  <br> |
| signal void | [**isReachableChanged**](classQfCloudConnection.md#signal-isreachablechanged)  <br> |
| signal void | [**loginFailed**](classQfCloudConnection.md#signal-loginfailed) (const QString & reason) <br> |
| signal void | [**passwordChanged**](classQfCloudConnection.md#signal-passwordchanged)  <br> |
| signal void | [**pendingAttachmentsAdded**](classQfCloudConnection.md#signal-pendingattachmentsadded)  <br> |
| signal void | [**pendingAttachmentsUploadFinished**](classQfCloudConnection.md#signal-pendingattachmentsuploadfinished) (const QString & error=QString()) <br> |
| signal void | [**pendingAttachmentsUploadStatus**](classQfCloudConnection.md#signal-pendingattachmentsuploadstatus) (const QString & fileName, double fileProgress, qsizetype uploadPending) <br> |
| signal void | [**providerChanged**](classQfCloudConnection.md#signal-providerchanged)  <br> |
| signal void | [**providerConfigurationChanged**](classQfCloudConnection.md#signal-providerconfigurationchanged)  <br> |
| signal void | [**queuedProjectPushRequested**](classQfCloudConnection.md#signal-queuedprojectpushrequested) (const QString & projectId) <br> |
| signal void | [**serverInformationChanged**](classQfCloudConnection.md#signal-serverinformationchanged)  <br> |
| signal void | [**stateChanged**](classQfCloudConnection.md#signal-statechanged)  <br> |
| signal void | [**statusChanged**](classQfCloudConnection.md#signal-statuschanged)  <br> |
| signal void | [**subscriptionInformationReceived**](classQfCloudConnection.md#signal-subscriptioninformationreceived) (const [**QfCloudSubscriptionInformation**](structQfCloudSubscriptionInformation.md) & subscriptionInformation) <br> |
| signal void | [**tokenChanged**](classQfCloudConnection.md#signal-tokenchanged)  <br> |
| signal void | [**urlChanged**](classQfCloudConnection.md#signal-urlchanged)  <br> |
| signal void | [**urlsChanged**](classQfCloudConnection.md#signal-urlschanged)  <br> |
| signal void | [**userInformationChanged**](classQfCloudConnection.md#signal-userinformationchanged)  <br> |
| signal void | [**userOrganizationsReceived**](classQfCloudConnection.md#signal-userorganizationsreceived) (const QStringList & organizations) <br> |
| signal void | [**usernameChanged**](classQfCloudConnection.md#signal-usernamechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCloudConnection**](#function-qfcloudconnection) () <br> |
|  QList&lt; [**QfAuthenticationProvider**](classQfAuthenticationProvider.md) &gt; | [**availableProviders**](#function-availableproviders-22) () const<br> |
|  QString | [**avatarUrl**](#function-avatarurl-22) () const<br> |
|  [**QfNetworkReply**](classQfNetworkReply.md) \* | [**get**](#function-get-13) (const QString & endpoint, const QVariantMap & params=QVariantMap()) <br> |
|  [**QfNetworkReply**](classQfNetworkReply.md) \* | [**get**](#function-get-23) (QNetworkRequest & request, const QString & endpoint, const QVariantMap & params=QVariantMap()) <br> |
|  [**QfNetworkReply**](classQfNetworkReply.md) \* | [**get**](#function-get-33) (QNetworkRequest & request, const QUrl & url, const QVariantMap & params=QVariantMap()) <br> |
|  Q\_INVOKABLE void | [**getServerInformation**](#function-getserverinformation) () <br> |
|  Q\_INVOKABLE void | [**getSubscriptionInformation**](#function-getsubscriptioninformation) (const QString & user) <br> |
|  Q\_INVOKABLE void | [**getUserOrganizations**](#function-getuserorganizations) (const QString & user) <br> |
|  bool | [**hasProviderConfiguration**](#function-hasproviderconfiguration-22) () <br> |
|  bool | [**hasToken**](#function-hastoken-22) () <br> |
|  bool | [**isFetchingAvailableProviders**](#function-isfetchingavailableproviders-22) () const<br> |
|  bool | [**isReachable**](#function-isreachable-22) () const<br> |
|  Q\_INVOKABLE void | [**login**](#function-login) (const QString & password=QString()) <br> |
|  Q\_INVOKABLE void | [**logout**](#function-logout) () <br> |
|  QString | [**password**](#function-password-22) () const<br> |
|  [**QfNetworkReply**](classQfNetworkReply.md) \* | [**post**](#function-post-12) (const QString & endpoint, const QVariantMap & params=QVariantMap(), const QStringList & fileNames=QStringList()) <br> |
|  [**QfNetworkReply**](classQfNetworkReply.md) \* | [**post**](#function-post-22) (QNetworkRequest & request, const QString & endpoint, const QVariantMap & params=QVariantMap(), const QStringList & fileNames=QStringList()) <br> |
|  QString | [**provider**](#function-provider-22) () const<br> |
|  void | [**queueProjectPush**](#function-queueprojectpush) (const QString & projectId) <br> |
|  [**QfCloudServerInformation**](structQfCloudServerInformation.md) | [**serverInformation**](#function-serverinformation-22) () const<br> |
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
|  [**QfCloudUserInformation**](structQfCloudUserInformation.md) | [**userInformation**](#function-userinformation-22) () const<br> |
|  QString | [**username**](#function-username-22) () const<br> |
|   | [**~QfCloudConnection**](#function-qfcloudconnection) () <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  QString | [**defaultUrl**](#function-defaulturl-22) () <br> |
|  QString | [**errorString**](#function-errorstring) (QNetworkReply \* reply) <br>_Returns an error string to be shown to the user if_ _reply_ _has an error._ |


























## Public Types Documentation




### enum ConnectionState 

```C++
enum QfCloudConnection::ConnectionState {
    Idle,
    Busy
};
```




<hr>



### enum ConnectionStatus 

```C++
enum QfCloudConnection::ConnectionStatus {
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
QMap< QString, QString > QfCloudConnection::sErrors;
```




<hr>
## Public Properties Documentation




### property availableProviders [1/2]

```C++
QList<QfAuthenticationProvider> QfCloudConnection::availableProviders;
```




<hr>



### property avatarUrl [1/2]

```C++
QString QfCloudConnection::avatarUrl;
```




<hr>



### property defaultUrl [1/2]

```C++
QString QfCloudConnection::defaultUrl;
```




<hr>



### property hasProviderConfiguration [1/2]

```C++
bool QfCloudConnection::hasProviderConfiguration;
```




<hr>



### property hasToken [1/2]

```C++
bool QfCloudConnection::hasToken;
```




<hr>



### property isFetchingAvailableProviders [1/2]

```C++
bool QfCloudConnection::isFetchingAvailableProviders;
```




<hr>



### property isReachable [1/2]

```C++
bool QfCloudConnection::isReachable;
```




<hr>



### property password [1/2]

```C++
QString QfCloudConnection::password;
```




<hr>



### property provider [1/2]

```C++
QString QfCloudConnection::provider;
```




<hr>



### property serverInformation [1/2]

```C++
QfCloudServerInformation QfCloudConnection::serverInformation;
```




<hr>



### property state [1/2]

```C++
ConnectionState QfCloudConnection::state;
```




<hr>



### property status [1/2]

```C++
ConnectionStatus QfCloudConnection::status;
```




<hr>



### property url [1/2]

```C++
QString QfCloudConnection::url;
```




<hr>



### property urls [1/2]

```C++
QStringList QfCloudConnection::urls;
```




<hr>



### property userInformation [1/2]

```C++
QfCloudUserInformation QfCloudConnection::userInformation;
```




<hr>



### property username [1/2]

```C++
QString QfCloudConnection::username;
```




<hr>
## Public Signals Documentation




### signal availableProvidersChanged 

```C++
void QfCloudConnection::availableProvidersChanged;
```




<hr>



### signal avatarUrlChanged 

```C++
void QfCloudConnection::avatarUrlChanged;
```




<hr>



### signal error 

```C++
void QfCloudConnection::error;
```




<hr>



### signal isFetchingAvailableProvidersChanged 

```C++
void QfCloudConnection::isFetchingAvailableProvidersChanged;
```




<hr>



### signal isReachableChanged 

```C++
void QfCloudConnection::isReachableChanged;
```




<hr>



### signal loginFailed 

```C++
void QfCloudConnection::loginFailed;
```




<hr>



### signal passwordChanged 

```C++
void QfCloudConnection::passwordChanged;
```




<hr>



### signal pendingAttachmentsAdded 

```C++
void QfCloudConnection::pendingAttachmentsAdded;
```




<hr>



### signal pendingAttachmentsUploadFinished 

```C++
void QfCloudConnection::pendingAttachmentsUploadFinished;
```




<hr>



### signal pendingAttachmentsUploadStatus 

```C++
void QfCloudConnection::pendingAttachmentsUploadStatus;
```




<hr>



### signal providerChanged 

```C++
void QfCloudConnection::providerChanged;
```




<hr>



### signal providerConfigurationChanged 

```C++
void QfCloudConnection::providerConfigurationChanged;
```




<hr>



### signal queuedProjectPushRequested 

```C++
void QfCloudConnection::queuedProjectPushRequested;
```




<hr>



### signal serverInformationChanged 

```C++
void QfCloudConnection::serverInformationChanged;
```




<hr>



### signal stateChanged 

```C++
void QfCloudConnection::stateChanged;
```




<hr>



### signal statusChanged 

```C++
void QfCloudConnection::statusChanged;
```




<hr>



### signal subscriptionInformationReceived 

```C++
void QfCloudConnection::subscriptionInformationReceived;
```




<hr>



### signal tokenChanged 

```C++
void QfCloudConnection::tokenChanged;
```




<hr>



### signal urlChanged 

```C++
void QfCloudConnection::urlChanged;
```




<hr>



### signal urlsChanged 

```C++
void QfCloudConnection::urlsChanged;
```




<hr>



### signal userInformationChanged 

```C++
void QfCloudConnection::userInformationChanged;
```




<hr>



### signal userOrganizationsReceived 

```C++
void QfCloudConnection::userOrganizationsReceived;
```




<hr>



### signal usernameChanged 

```C++
void QfCloudConnection::usernameChanged;
```




<hr>
## Public Functions Documentation




### function QfCloudConnection 

```C++
QfCloudConnection::QfCloudConnection () 
```




<hr>



### function availableProviders [2/2]

```C++
QList< QfAuthenticationProvider > QfCloudConnection::availableProviders () const
```




<hr>



### function avatarUrl [2/2]

```C++
QString QfCloudConnection::avatarUrl () const
```




<hr>



### function get [1/3]

```C++
QfNetworkReply * QfCloudConnection::get (
    const QString & endpoint,
    const QVariantMap & params=QVariantMap()
) 
```



Sends a get request to the given _endpoint_. Query can be passed via _params_, empty by default.


If this connection is not logged in, will return nullptr. The returned reply needs to be deleted by the caller. 


        

<hr>



### function get [2/3]

```C++
QfNetworkReply * QfCloudConnection::get (
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
QfNetworkReply * QfCloudConnection::get (
    QNetworkRequest & request,
    const QUrl & url,
    const QVariantMap & params=QVariantMap()
) 
```




<hr>



### function getServerInformation 

```C++
Q_INVOKABLE void QfCloudConnection::getServerInformation () 
```




<hr>



### function getSubscriptionInformation 

```C++
Q_INVOKABLE void QfCloudConnection::getSubscriptionInformation (
    const QString & user
) 
```




<hr>



### function getUserOrganizations 

```C++
Q_INVOKABLE void QfCloudConnection::getUserOrganizations (
    const QString & user
) 
```




<hr>



### function hasProviderConfiguration [2/2]

```C++
inline bool QfCloudConnection::hasProviderConfiguration () 
```




<hr>



### function hasToken [2/2]

```C++
inline bool QfCloudConnection::hasToken () 
```




<hr>



### function isFetchingAvailableProviders [2/2]

```C++
bool QfCloudConnection::isFetchingAvailableProviders () const
```




<hr>



### function isReachable [2/2]

```C++
bool QfCloudConnection::isReachable () const
```



Returns whether the network is currently reachable.


If reachability information is not available, this returns true to keep the existing behavior unchanged. 


        

<hr>



### function login 

```C++
Q_INVOKABLE void QfCloudConnection::login (
    const QString & password=QString()
) 
```




<hr>



### function logout 

```C++
Q_INVOKABLE void QfCloudConnection::logout () 
```




<hr>



### function password [2/2]

```C++
QString QfCloudConnection::password () const
```




<hr>



### function post [1/2]

```C++
QfNetworkReply * QfCloudConnection::post (
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
QfNetworkReply * QfCloudConnection::post (
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
QString QfCloudConnection::provider () const
```




<hr>



### function queueProjectPush 

```C++
void QfCloudConnection::queueProjectPush (
    const QString & projectId
) 
```



Queues a project push request when the network is not reachable.


The push will be automatically triggered once the connection is back online and the user is logged in. 


        

<hr>



### function serverInformation [2/2]

```C++
inline QfCloudServerInformation QfCloudConnection::serverInformation () const
```




<hr>



### function setAuthenticationDetails 

```C++
void QfCloudConnection::setAuthenticationDetails (
    QNetworkRequest & request
) 
```



Sets authentication details on a _request_. 


        

<hr>



### function setPassword 

```C++
void QfCloudConnection::setPassword (
    const QString & password
) 
```




<hr>



### function setProvider 

```C++
void QfCloudConnection::setProvider (
    const QString & provider
) 
```




<hr>



### function setUrl 

```C++
void QfCloudConnection::setUrl (
    const QString & url
) 
```



Sets the current server connection URL and saves it into QSettings. 


        

<hr>



### function setUsername 

```C++
void QfCloudConnection::setUsername (
    const QString & username
) 
```




<hr>



### function state [2/2]

```C++
ConnectionState QfCloudConnection::state () const
```




<hr>



### function status [2/2]

```C++
ConnectionStatus QfCloudConnection::status () const
```




<hr>



### function token 

```C++
QString QfCloudConnection::token () const
```




<hr>



### function uploadPendingAttachments 

```C++
qsizetype QfCloudConnection::uploadPendingAttachments () 
```



Uploads any pending attachments linked to the logged in user account. 

**Returns:**

the number of attachments to be uploaded. 





        

<hr>



### function url [2/2]

```C++
QString QfCloudConnection::url () const
```



Returns the currently set server connection URL. 


        

<hr>



### function urls [2/2]

```C++
QStringList QfCloudConnection::urls () const
```



Returns the connections URLs successfully logged in in the past. 


        

<hr>



### function userInformation [2/2]

```C++
QfCloudUserInformation QfCloudConnection::userInformation () const
```




<hr>



### function username [2/2]

```C++
QString QfCloudConnection::username () const
```




<hr>



### function ~QfCloudConnection 

```C++
QfCloudConnection::~QfCloudConnection () 
```




<hr>
## Public Static Functions Documentation




### function defaultUrl [2/2]

```C++
static QString QfCloudConnection::defaultUrl () 
```



Default server connection URL, pointing to the production server. 


        

<hr>



### function errorString 

_Returns an error string to be shown to the user if_ _reply_ _has an error._
```C++
static QString QfCloudConnection::errorString (
    QNetworkReply * reply
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfcloudconnection.h`

