

# Class QfViewStatus



[**ClassList**](annotated.md) **>** [**QfViewStatus**](classQfViewStatus.md)








Inherits the following classes: QObject


Inherited by the following classes: [QfAndroidViewStatus](classQfAndroidViewStatus.md)




























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**finished**](classQfViewStatus.md#signal-finished)  <br>_This signal is emitted when a view action is finished._  |
| signal void | [**statusReceived**](classQfViewStatus.md#signal-statusreceived) (const QString & statusText) <br>_This signal is emitted when a status about the view action has been received._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfViewStatus**](#function-qfviewstatus) (QObject \* parent=nullptr) <br> |
| virtual  | [**~QfViewStatus**](#function-qfviewstatus) () = default<br> |




























## Public Signals Documentation




### signal finished 

_This signal is emitted when a view action is finished._ 
```C++
void QfViewStatus::finished;
```




<hr>



### signal statusReceived 

_This signal is emitted when a status about the view action has been received._ 
```C++
void QfViewStatus::statusReceived;
```




<hr>
## Public Functions Documentation




### function QfViewStatus 

```C++
explicit QfViewStatus::QfViewStatus (
    QObject * parent=nullptr
) 
```




<hr>



### function ~QfViewStatus 

```C++
virtual QfViewStatus::~QfViewStatus () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfviewstatus.h`

