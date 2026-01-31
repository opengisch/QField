

# Class QFieldAppAuthRequestHandler



[**ClassList**](annotated.md) **>** [**QFieldAppAuthRequestHandler**](classQFieldAppAuthRequestHandler.md)



[More...](#detailed-description)

* `#include <qfieldappauthrequesthandler.h>`



Inherits the following classes: QObject,  QgsCredentials,  QgsNetworkAuthenticationHandler


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**hasPendingAuthRequest**](classQFieldAppAuthRequestHandler.md#property-haspendingauthrequest-12)  <br> |
| property bool | [**isProjectLoading**](classQFieldAppAuthRequestHandler.md#property-isprojectloading-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**hasPendingAuthRequestChanged**](classQFieldAppAuthRequestHandler.md#signal-haspendingauthrequestchanged)  <br> |
| signal void | [**hideLoginBrowser**](classQFieldAppAuthRequestHandler.md#signal-hideloginbrowser)  <br> |
| signal void | [**isProjectLoadingChanged**](classQFieldAppAuthRequestHandler.md#signal-isprojectloadingchanged)  <br> |
| signal void | [**loginDialogClosed**](classQFieldAppAuthRequestHandler.md#signal-logindialogclosed) (const QString & realm, const bool canceled) <br> |
| signal void | [**reloadEverything**](classQFieldAppAuthRequestHandler.md#signal-reloadeverything)  <br> |
| signal void | [**showLoginBrowser**](classQFieldAppAuthRequestHandler.md#signal-showloginbrowser) (const QString & url) <br> |
| signal void | [**showLoginDialog**](classQFieldAppAuthRequestHandler.md#signal-showlogindialog) (const QString & realm, const QString & title) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QFieldAppAuthRequestHandler**](#function-qfieldappauthrequesthandler) () <br> |
|  Q\_INVOKABLE void | [**abortAuthBrowser**](#function-abortauthbrowser) () <br>_abort an ongoing external browser authentication request_  |
|  Q\_INVOKABLE void | [**clearStoredRealms**](#function-clearstoredrealms) () <br>_clears the stored realms_  |
|  Q\_INVOKABLE void | [**enterCredentials**](#function-entercredentials) (const QString & realm, const QString & username, const QString & password) <br>_stores the credentials after the information is entered in the login dialog_  |
|  void | [**handleAuthRequest**](#function-handleauthrequest) (QNetworkReply \* reply, QAuthenticator \* auth) override<br>_handles the auth request - triggered by the authRequestOccurred signal_  |
|  void | [**handleAuthRequestCloseBrowser**](#function-handleauthrequestclosebrowser) () override<br> |
|  void | [**handleAuthRequestOpenBrowser**](#function-handleauthrequestopenbrowser) (const QUrl & url) override<br> |
|  Q\_INVOKABLE bool | [**handleLayerLogins**](#function-handlelayerlogins) () <br>_handles each realm after the project has been loaded_  |
|  bool | [**hasPendingAuthRequest**](#function-haspendingauthrequest-22) () const<br>_returns the number of pending authentication requests_  |
|  bool | [**isProjectLoading**](#function-isprojectloading-22) () const<br>_returns TRUE is a project is loading_  |
|  void | [**setIsProjectLoading**](#function-setisprojectloading) (bool loading) <br>_sets whether a project is_ _loading_ __ |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  bool | [**request**](#function-request) (const QString & realm, QString & username, QString & password, const QString & message=QString()) override<br> |
|  bool | [**requestMasterPassword**](#function-requestmasterpassword) (QString & password, bool stored=false) override<br> |




## Detailed Description


When a layer requests and authentication, this handler is triggered. When the credentials are found in QgsCredentials (because it's stored before), they are used for the login. If not, QGIS would popup a Login Dialog, but QField does not. The realms (where it should log in to) are just added to the list of unhandled realms. When all layers are handled, the login dialog is diplayed for each of them. On successful login, the credentials are stored in QgsCredentials and removed from the list. When the list is empty (or contains only canceled realms), the project is reloaded. This time the logins are contained in QgsCredentials and no dialog needs to pop up. 


    
## Public Properties Documentation




### property hasPendingAuthRequest [1/2]

```C++
bool QFieldAppAuthRequestHandler::hasPendingAuthRequest;
```




<hr>



### property isProjectLoading [1/2]

```C++
bool QFieldAppAuthRequestHandler::isProjectLoading;
```




<hr>
## Public Signals Documentation




### signal hasPendingAuthRequestChanged 

```C++
void QFieldAppAuthRequestHandler::hasPendingAuthRequestChanged;
```




<hr>



### signal hideLoginBrowser 

```C++
void QFieldAppAuthRequestHandler::hideLoginBrowser;
```




<hr>



### signal isProjectLoadingChanged 

```C++
void QFieldAppAuthRequestHandler::isProjectLoadingChanged;
```




<hr>



### signal loginDialogClosed 

```C++
void QFieldAppAuthRequestHandler::loginDialogClosed;
```




<hr>



### signal reloadEverything 

```C++
void QFieldAppAuthRequestHandler::reloadEverything;
```




<hr>



### signal showLoginBrowser 

```C++
void QFieldAppAuthRequestHandler::showLoginBrowser;
```




<hr>



### signal showLoginDialog 

```C++
void QFieldAppAuthRequestHandler::showLoginDialog;
```




<hr>
## Public Functions Documentation




### function QFieldAppAuthRequestHandler 

```C++
QFieldAppAuthRequestHandler::QFieldAppAuthRequestHandler () 
```




<hr>



### function abortAuthBrowser 

_abort an ongoing external browser authentication request_ 
```C++
Q_INVOKABLE void QFieldAppAuthRequestHandler::abortAuthBrowser () 
```




<hr>



### function clearStoredRealms 

_clears the stored realms_ 
```C++
Q_INVOKABLE void QFieldAppAuthRequestHandler::clearStoredRealms () 
```




<hr>



### function enterCredentials 

_stores the credentials after the information is entered in the login dialog_ 
```C++
Q_INVOKABLE void QFieldAppAuthRequestHandler::enterCredentials (
    const QString & realm,
    const QString & username,
    const QString & password
) 
```




<hr>



### function handleAuthRequest 

_handles the auth request - triggered by the authRequestOccurred signal_ 
```C++
void QFieldAppAuthRequestHandler::handleAuthRequest (
    QNetworkReply * reply,
    QAuthenticator * auth
) override
```




<hr>



### function handleAuthRequestCloseBrowser 

```C++
void QFieldAppAuthRequestHandler::handleAuthRequestCloseBrowser () override
```




<hr>



### function handleAuthRequestOpenBrowser 

```C++
void QFieldAppAuthRequestHandler::handleAuthRequestOpenBrowser (
    const QUrl & url
) override
```




<hr>



### function handleLayerLogins 

_handles each realm after the project has been loaded_ 
```C++
Q_INVOKABLE bool QFieldAppAuthRequestHandler::handleLayerLogins () 
```




<hr>



### function hasPendingAuthRequest [2/2]

_returns the number of pending authentication requests_ 
```C++
bool QFieldAppAuthRequestHandler::hasPendingAuthRequest () const
```




<hr>



### function isProjectLoading [2/2]

_returns TRUE is a project is loading_ 
```C++
bool QFieldAppAuthRequestHandler::isProjectLoading () const
```




<hr>



### function setIsProjectLoading 

_sets whether a project is_ _loading_ __
```C++
void QFieldAppAuthRequestHandler::setIsProjectLoading (
    bool loading
) 
```




<hr>
## Protected Functions Documentation




### function request 

```C++
bool QFieldAppAuthRequestHandler::request (
    const QString & realm,
    QString & username,
    QString & password,
    const QString & message=QString()
) override
```




<hr>



### function requestMasterPassword 

```C++
inline bool QFieldAppAuthRequestHandler::requestMasterPassword (
    QString & password,
    bool stored=false
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldappauthrequesthandler.h`

