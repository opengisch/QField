

# Class QfSettings



[**ClassList**](annotated.md) **>** [**QfSettings**](classQfSettings.md)








Inherits the following classes: QSettings






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**settingChanged**](classQfSettings.md#signal-settingchanged) (const QString & key) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfSettings**](#function-qfsettings) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**remove**](#function-remove) (const QString & key) <br> |
|  Q\_INVOKABLE void | [**setValue**](#function-setvalue) (const QString & key, const QVariant & value) <br> |
|  Q\_INVOKABLE void | [**sync**](#function-sync) () <br> |
|  Q\_INVOKABLE QVariant | [**value**](#function-value) (const QString & key, const QVariant & defaultValue) <br> |
|  Q\_INVOKABLE bool | [**valueBool**](#function-valuebool) (const QString & key, bool defaultValue) <br> |
|  Q\_INVOKABLE int | [**valueInt**](#function-valueint) (const QString & key, int defaultValue) <br> |




























## Public Signals Documentation




### signal settingChanged 

```C++
void QfSettings::settingChanged;
```




<hr>
## Public Functions Documentation




### function QfSettings 

```C++
explicit QfSettings::QfSettings (
    QObject * parent=nullptr
) 
```




<hr>



### function remove 

```C++
Q_INVOKABLE void QfSettings::remove (
    const QString & key
) 
```



Removes the given _key_ from settings. 


        

<hr>



### function setValue 

```C++
Q_INVOKABLE void QfSettings::setValue (
    const QString & key,
    const QVariant & value
) 
```




<hr>



### function sync 

```C++
Q_INVOKABLE void QfSettings::sync () 
```



Writes any unsaved changes to permanent storage, and reloads the settings. 


        

<hr>



### function value 

```C++
Q_INVOKABLE QVariant QfSettings::value (
    const QString & key,
    const QVariant & defaultValue
) 
```




<hr>



### function valueBool 

```C++
Q_INVOKABLE bool QfSettings::valueBool (
    const QString & key,
    bool defaultValue
) 
```



Properly evaluates the returned value to be boolean. If the normal value() is used instead, a string "true" or "false" will be returned which will be evaluated to true either way by JS. 


        

<hr>



### function valueInt 

```C++
Q_INVOKABLE int QfSettings::valueInt (
    const QString & key,
    int defaultValue
) 
```



Properly evaluates the returned value to be int. If the normal value() is used instead, a string "1" or "-456" will be returned. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfsettings.h`

