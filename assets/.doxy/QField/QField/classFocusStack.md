

# Class FocusStack



[**ClassList**](annotated.md) **>** [**FocusStack**](classFocusStack.md)








Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**FocusStack**](#function-focusstack) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**addFocusTaker**](#function-addfocustaker) (QObject \* object) <br> |
|  Q\_INVOKABLE void | [**forceActiveFocusOnLastTaker**](#function-forceactivefocusonlasttaker) () <br> |
|   | [**~FocusStack**](#function-focusstack) () = default<br> |




























## Public Functions Documentation




### function FocusStack 

```C++
explicit FocusStack::FocusStack (
    QObject * parent=nullptr
) 
```




<hr>



### function addFocusTaker 

```C++
Q_INVOKABLE void FocusStack::addFocusTaker (
    QObject * object
) 
```




<hr>



### function forceActiveFocusOnLastTaker 

```C++
Q_INVOKABLE void FocusStack::forceActiveFocusOnLastTaker () 
```




<hr>



### function ~FocusStack 

```C++
FocusStack::~FocusStack () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/focusstack.h`

