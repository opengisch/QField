

# Class ScreenDimmer



[**ClassList**](annotated.md) **>** [**ScreenDimmer**](classScreenDimmer.md)



_The_ [_**ScreenDimmer**_](classScreenDimmer.md) _class handles dimming of screen brightness._

* `#include <screendimmer.h>`



Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ScreenDimmer**](#function-screendimmer) (QgsApplication \* app) <br> |
|  void | [**setSuspend**](#function-setsuspend) (bool suspend) <br> |
|  void | [**setTimeout**](#function-settimeout) (int timeoutSeconds) <br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  bool | [**eventFilter**](#function-eventfilter) (QObject \* obj, QEvent \* event) override<br> |




## Public Functions Documentation




### function ScreenDimmer 

```C++
explicit ScreenDimmer::ScreenDimmer (
    QgsApplication * app
) 
```




<hr>



### function setSuspend 

```C++
void ScreenDimmer::setSuspend (
    bool suspend
) 
```



Temporarily suspends the screen dimmer when _suspend_ is set to TRUE. 


        

<hr>



### function setTimeout 

```C++
void ScreenDimmer::setTimeout (
    int timeoutSeconds
) 
```



Sets dim timeout as _timeoutSeconds_ in seconds. Disables dim screen if 0. 


        

<hr>
## Protected Functions Documentation




### function eventFilter 

```C++
bool ScreenDimmer::eventFilter (
    QObject * obj,
    QEvent * event
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/screendimmer.h`

