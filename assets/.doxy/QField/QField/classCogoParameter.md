

# Class CogoParameter



[**ClassList**](annotated.md) **>** [**CogoParameter**](classCogoParameter.md)



_A COGO operation parameter._ 

* `#include <cogooperation.h>`





























## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**color**](classCogoParameter.md#property-color)  <br> |
| property QVariantMap | [**configuration**](classCogoParameter.md#property-configuration)  <br> |
| property QString | [**label**](classCogoParameter.md#property-label)  <br> |
| property QString | [**name**](classCogoParameter.md#property-name)  <br> |
| property QString | [**type**](classCogoParameter.md#property-type)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CogoParameter**](#function-cogoparameter) (const QString & type=QString(), const QString & name=QString(), const QString & label=QString(), const QColor & color=Qt::transparent, const QVariantMap & configuration=QVariantMap()) <br> |
|  bool | [**operator!=**](#function-operator) (const [**CogoParameter**](classCogoParameter.md) & other) const<br> |
|  bool | [**operator==**](#function-operator_1) (const [**CogoParameter**](classCogoParameter.md) & other) const<br> |




























## Public Properties Documentation




### property color 

```C++
QColor CogoParameter::color;
```



The COGO parameter color. 


        

<hr>



### property configuration 

```C++
QVariantMap CogoParameter::configuration;
```



The COGO parameter's extra configuration details. 


        

<hr>



### property label 

```C++
QString CogoParameter::label;
```



The COGO parameter label. 


        

<hr>



### property name 

```C++
QString CogoParameter::name;
```



The COGO parameter name. 


        

<hr>



### property type 

```C++
QString CogoParameter::type;
```



The COGO parameter type ("point", "distance", "angle, "enum", etc.). 


        

<hr>
## Public Functions Documentation




### function CogoParameter 

```C++
inline explicit CogoParameter::CogoParameter (
    const QString & type=QString(),
    const QString & name=QString(),
    const QString & label=QString(),
    const QColor & color=Qt::transparent,
    const QVariantMap & configuration=QVariantMap()
) 
```




<hr>



### function operator!= 

```C++
inline bool CogoParameter::operator!= (
    const CogoParameter & other
) const
```




<hr>



### function operator== 

```C++
inline bool CogoParameter::operator== (
    const CogoParameter & other
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/cogo/cogooperation.h`

