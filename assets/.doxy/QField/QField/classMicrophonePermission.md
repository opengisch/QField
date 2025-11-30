

# Class MicrophonePermission



[**ClassList**](annotated.md) **>** [**MicrophonePermission**](classMicrophonePermission.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property Qt::PermissionStatus | [**status**](classMicrophonePermission.md#property-status-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**statusChanged**](classMicrophonePermission.md#signal-statuschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**MicrophonePermission**](#function-microphonepermission) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**request**](#function-request) () <br> |
|  Qt::PermissionStatus | [**status**](#function-status-22) () <br> |




























## Public Properties Documentation




### property status [1/2]

```C++
Qt::PermissionStatus MicrophonePermission::status;
```




<hr>
## Public Signals Documentation




### signal statusChanged 

```C++
void MicrophonePermission::statusChanged;
```



Emitted when the status has changed. 


        

<hr>
## Public Functions Documentation




### function MicrophonePermission 

```C++
explicit MicrophonePermission::MicrophonePermission (
    QObject * parent=nullptr
) 
```




<hr>



### function request 

```C++
Q_INVOKABLE void MicrophonePermission::request () 
```



Requests the permission. 


        

<hr>



### function status [2/2]

```C++
Qt::PermissionStatus MicrophonePermission::status () 
```



Returns the permission status. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/permissions.h`

