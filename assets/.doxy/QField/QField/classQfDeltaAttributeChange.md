

# Class QfDeltaAttributeChange



[**ClassList**](annotated.md) **>** [**QfDeltaAttributeChange**](classQfDeltaAttributeChange.md)



_A single attribute modified by a delta, with the values on both sides of the change._ 

* `#include <qfdeltachangesmodel.h>`





























## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**name**](classQfDeltaAttributeChange.md#property-name)  <br> |
| property QVariant | [**newValue**](classQfDeltaAttributeChange.md#property-newvalue)  <br> |
| property QVariant | [**oldValue**](classQfDeltaAttributeChange.md#property-oldvalue)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfDeltaAttributeChange**](#function-qfdeltaattributechange) (const QString & name=QString(), const QVariant & oldValue=QVariant(), const QVariant & newValue=QVariant()) <br> |




























## Public Properties Documentation




### property name 

```C++
QString QfDeltaAttributeChange::name;
```




<hr>



### property newValue 

```C++
QVariant QfDeltaAttributeChange::newValue;
```




<hr>



### property oldValue 

```C++
QVariant QfDeltaAttributeChange::oldValue;
```




<hr>
## Public Functions Documentation




### function QfDeltaAttributeChange 

```C++
inline explicit QfDeltaAttributeChange::QfDeltaAttributeChange (
    const QString & name=QString(),
    const QVariant & oldValue=QVariant(),
    const QVariant & newValue=QVariant()
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfdeltachangesmodel.h`

