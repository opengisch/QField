

# Class QfCameraPermission



[**ClassList**](annotated.md) **>** [**QfCameraPermission**](classQfCameraPermission.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property Qt::PermissionStatus | [**status**](classQfCameraPermission.md#property-status-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**statusChanged**](classQfCameraPermission.md#signal-statuschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCameraPermission**](#function-qfcamerapermission) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**request**](#function-request) () <br> |
|  Qt::PermissionStatus | [**status**](#function-status-22) () <br> |




























## Public Properties Documentation




### property status [1/2]

```C++
Qt::PermissionStatus QfCameraPermission::status;
```




<hr>
## Public Signals Documentation




### signal statusChanged 

```C++
void QfCameraPermission::statusChanged;
```



Emitted when the status has changed. 


        

<hr>
## Public Functions Documentation




### function QfCameraPermission 

```C++
explicit QfCameraPermission::QfCameraPermission (
    QObject * parent=nullptr
) 
```




<hr>



### function request 

```C++
Q_INVOKABLE void QfCameraPermission::request () 
```



Requests the permission. 


        

<hr>



### function status [2/2]

```C++
Qt::PermissionStatus QfCameraPermission::status () 
```



Returns the permission status. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfpermissions.h`

