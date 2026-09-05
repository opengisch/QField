

# Struct QfCloudWhitelabelInformation



[**ClassList**](annotated.md) **>** [**QfCloudWhitelabelInformation**](structQfCloudWhitelabelInformation.md)



[More...](#detailed-description)

* `#include <qfcloudutils.h>`





























## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**favicon**](structQfCloudWhitelabelInformation.md#property-favicon)  <br> |
| property QString | [**logoMain**](structQfCloudWhitelabelInformation.md#property-logomain)  <br> |
| property QString | [**logoNavbar**](structQfCloudWhitelabelInformation.md#property-logonavbar)  <br> |
| property QString | [**siteTitle**](structQfCloudWhitelabelInformation.md#property-sitetitle)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCloudWhitelabelInformation**](#function-qfcloudwhitelabelinformation-12) () = default<br> |
|   | [**QfCloudWhitelabelInformation**](#function-qfcloudwhitelabelinformation-22) (const QVariantMap & whitelabel) <br> |
|  bool | [**operator!=**](#function-operator) (const [**QfCloudWhitelabelInformation**](structQfCloudWhitelabelInformation.md) & other) const<br> |
|  bool | [**operator==**](#function-operator_1) (const [**QfCloudWhitelabelInformation**](structQfCloudWhitelabelInformation.md) & other) const<br> |
|  QVariantMap | [**toVariantMap**](#function-tovariantmap) () const<br> |




























## Detailed Description


Holds whitelabeling information advertised by a QFieldCloud server, such as the displayed site title and branding assets. 


    
## Public Properties Documentation




### property favicon 

```C++
QString QfCloudWhitelabelInformation::favicon;
```




<hr>



### property logoMain 

```C++
QString QfCloudWhitelabelInformation::logoMain;
```




<hr>



### property logoNavbar 

```C++
QString QfCloudWhitelabelInformation::logoNavbar;
```




<hr>



### property siteTitle 

```C++
QString QfCloudWhitelabelInformation::siteTitle;
```




<hr>
## Public Functions Documentation




### function QfCloudWhitelabelInformation [1/2]

```C++
QfCloudWhitelabelInformation::QfCloudWhitelabelInformation () = default
```




<hr>



### function QfCloudWhitelabelInformation [2/2]

```C++
inline explicit QfCloudWhitelabelInformation::QfCloudWhitelabelInformation (
    const QVariantMap & whitelabel
) 
```




<hr>



### function operator!= 

```C++
inline bool QfCloudWhitelabelInformation::operator!= (
    const QfCloudWhitelabelInformation & other
) const
```




<hr>



### function operator== 

```C++
inline bool QfCloudWhitelabelInformation::operator== (
    const QfCloudWhitelabelInformation & other
) const
```




<hr>



### function toVariantMap 

```C++
inline QVariantMap QfCloudWhitelabelInformation::toVariantMap () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qfcloudutils.h`

