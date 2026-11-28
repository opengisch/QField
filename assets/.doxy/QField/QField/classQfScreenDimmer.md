

# Class QfScreenDimmer



[**ClassList**](annotated.md) **>** [**QfScreenDimmer**](classQfScreenDimmer.md)



_The_ [_**QfScreenDimmer**_](classQfScreenDimmer.md) _class handles dimming of screen brightness._

* `#include <qfscreendimmer.h>`



Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfScreenDimmer**](#function-qfscreendimmer) (QgsApplication \* app) <br> |
|  void | [**setSuspend**](#function-setsuspend) (bool suspend) <br> |
|  void | [**setTimeout**](#function-settimeout) (int timeoutSeconds) <br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  bool | [**eventFilter**](#function-eventfilter) (QObject \* obj, QEvent \* event) override<br> |




## Public Functions Documentation




### function QfScreenDimmer 

```C++
explicit QfScreenDimmer::QfScreenDimmer (
    QgsApplication * app
) 
```




<hr>



### function setSuspend 

```C++
void QfScreenDimmer::setSuspend (
    bool suspend
) 
```



Temporarily suspends the screen dimmer when _suspend_ is set to TRUE. 


        

<hr>



### function setTimeout 

```C++
void QfScreenDimmer::setTimeout (
    int timeoutSeconds
) 
```



Sets dim timeout as _timeoutSeconds_ in seconds. Disables dim screen if 0. 


        

<hr>
## Protected Functions Documentation




### function eventFilter 

```C++
bool QfScreenDimmer::eventFilter (
    QObject * obj,
    QEvent * event
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/app/qfscreendimmer.h`

