

# Class QfCogoParameter



[**ClassList**](annotated.md) **>** [**QfCogoParameter**](classQfCogoParameter.md)



_A COGO operation parameter._ 

* `#include <qfcogooperation.h>`





























## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**color**](classQfCogoParameter.md#property-color)  <br> |
| property QVariantMap | [**configuration**](classQfCogoParameter.md#property-configuration)  <br> |
| property QString | [**label**](classQfCogoParameter.md#property-label)  <br> |
| property QString | [**name**](classQfCogoParameter.md#property-name)  <br> |
| property QString | [**type**](classQfCogoParameter.md#property-type)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCogoParameter**](#function-qfcogoparameter) (const QString & type=QString(), const QString & name=QString(), const QString & label=QString(), const QColor & color=Qt::transparent, const QVariantMap & configuration=QVariantMap()) <br> |
|  bool | [**operator!=**](#function-operator) (const [**QfCogoParameter**](classQfCogoParameter.md) & other) const<br> |
|  bool | [**operator==**](#function-operator_1) (const [**QfCogoParameter**](classQfCogoParameter.md) & other) const<br> |




























## Public Properties Documentation




### property color 

```C++
QColor QfCogoParameter::color;
```



The COGO parameter color. 


        

<hr>



### property configuration 

```C++
QVariantMap QfCogoParameter::configuration;
```



The COGO parameter's extra configuration details. 


        

<hr>



### property label 

```C++
QString QfCogoParameter::label;
```



The COGO parameter label. 


        

<hr>



### property name 

```C++
QString QfCogoParameter::name;
```



The COGO parameter name. 


        

<hr>



### property type 

```C++
QString QfCogoParameter::type;
```



The COGO parameter type ("point", "distance", "angle, "enum", etc.). 


        

<hr>
## Public Functions Documentation




### function QfCogoParameter 

```C++
inline explicit QfCogoParameter::QfCogoParameter (
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
inline bool QfCogoParameter::operator!= (
    const QfCogoParameter & other
) const
```




<hr>



### function operator== 

```C++
inline bool QfCogoParameter::operator== (
    const QfCogoParameter & other
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/cogo/qfcogooperation.h`

