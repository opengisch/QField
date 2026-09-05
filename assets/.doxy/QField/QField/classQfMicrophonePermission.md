

# Class QfMicrophonePermission



[**ClassList**](annotated.md) **>** [**QfMicrophonePermission**](classQfMicrophonePermission.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property Qt::PermissionStatus | [**status**](classQfMicrophonePermission.md#property-status-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**statusChanged**](classQfMicrophonePermission.md#signal-statuschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfMicrophonePermission**](#function-qfmicrophonepermission) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**request**](#function-request) () <br> |
|  Qt::PermissionStatus | [**status**](#function-status-22) () <br> |




























## Public Properties Documentation




### property status [1/2]

```C++
Qt::PermissionStatus QfMicrophonePermission::status;
```




<hr>
## Public Signals Documentation




### signal statusChanged 

```C++
void QfMicrophonePermission::statusChanged;
```



Emitted when the status has changed. 


        

<hr>
## Public Functions Documentation




### function QfMicrophonePermission 

```C++
explicit QfMicrophonePermission::QfMicrophonePermission (
    QObject * parent=nullptr
) 
```




<hr>



### function request 

```C++
Q_INVOKABLE void QfMicrophonePermission::request () 
```



Requests the permission. 


        

<hr>



### function status [2/2]

```C++
Qt::PermissionStatus QfMicrophonePermission::status () 
```



Returns the permission status. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfpermissions.h`

