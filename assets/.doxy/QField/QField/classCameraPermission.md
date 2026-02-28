

# Class CameraPermission



[**ClassList**](annotated.md) **>** [**CameraPermission**](classCameraPermission.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property Qt::PermissionStatus | [**status**](classCameraPermission.md#property-status-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**statusChanged**](classCameraPermission.md#signal-statuschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CameraPermission**](#function-camerapermission) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**request**](#function-request) () <br> |
|  Qt::PermissionStatus | [**status**](#function-status-22) () <br> |




























## Public Properties Documentation




### property status [1/2]

```C++
Qt::PermissionStatus CameraPermission::status;
```




<hr>
## Public Signals Documentation




### signal statusChanged 

```C++
void CameraPermission::statusChanged;
```



Emitted when the status has changed. 


        

<hr>
## Public Functions Documentation




### function CameraPermission 

```C++
explicit CameraPermission::CameraPermission (
    QObject * parent=nullptr
) 
```




<hr>



### function request 

```C++
Q_INVOKABLE void CameraPermission::request () 
```



Requests the permission. 


        

<hr>



### function status [2/2]

```C++
Qt::PermissionStatus CameraPermission::status () 
```



Returns the permission status. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/permissions.h`

