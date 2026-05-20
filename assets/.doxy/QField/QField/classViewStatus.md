

# Class ViewStatus



[**ClassList**](annotated.md) **>** [**ViewStatus**](classViewStatus.md)








Inherits the following classes: QObject


Inherited by the following classes: [AndroidViewStatus](classAndroidViewStatus.md)




























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**finished**](classViewStatus.md#signal-finished)  <br>_This signal is emitted when a view action is finished._  |
| signal void | [**statusReceived**](classViewStatus.md#signal-statusreceived) (const QString & statusText) <br>_This signal is emitted when a status about the view action has been received._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ViewStatus**](#function-viewstatus) (QObject \* parent=nullptr) <br> |
| virtual  | [**~ViewStatus**](#function-viewstatus) () = default<br> |




























## Public Signals Documentation




### signal finished 

_This signal is emitted when a view action is finished._ 
```C++
void ViewStatus::finished;
```




<hr>



### signal statusReceived 

_This signal is emitted when a status about the view action has been received._ 
```C++
void ViewStatus::statusReceived;
```




<hr>
## Public Functions Documentation




### function ViewStatus 

```C++
explicit ViewStatus::ViewStatus (
    QObject * parent=nullptr
) 
```




<hr>



### function ~ViewStatus 

```C++
virtual ViewStatus::~ViewStatus () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/viewstatus.h`

