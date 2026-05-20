

# Struct CloudWhitelabelInformation



[**ClassList**](annotated.md) **>** [**CloudWhitelabelInformation**](structCloudWhitelabelInformation.md)



[More...](#detailed-description)

* `#include <qfieldcloudutils.h>`





























## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**favicon**](structCloudWhitelabelInformation.md#property-favicon)  <br> |
| property QString | [**logoMain**](structCloudWhitelabelInformation.md#property-logomain)  <br> |
| property QString | [**logoNavbar**](structCloudWhitelabelInformation.md#property-logonavbar)  <br> |
| property QString | [**siteTitle**](structCloudWhitelabelInformation.md#property-sitetitle)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CloudWhitelabelInformation**](#function-cloudwhitelabelinformation-12) () = default<br> |
|   | [**CloudWhitelabelInformation**](#function-cloudwhitelabelinformation-22) (const QVariantMap & whitelabel) <br> |
|  bool | [**operator!=**](#function-operator) (const [**CloudWhitelabelInformation**](structCloudWhitelabelInformation.md) & other) const<br> |
|  bool | [**operator==**](#function-operator_1) (const [**CloudWhitelabelInformation**](structCloudWhitelabelInformation.md) & other) const<br> |
|  QVariantMap | [**toVariantMap**](#function-tovariantmap) () const<br> |




























## Detailed Description


Holds whitelabeling information advertised by a QFieldCloud server, such as the displayed site title and branding assets. 


    
## Public Properties Documentation




### property favicon 

```C++
QString CloudWhitelabelInformation::favicon;
```




<hr>



### property logoMain 

```C++
QString CloudWhitelabelInformation::logoMain;
```




<hr>



### property logoNavbar 

```C++
QString CloudWhitelabelInformation::logoNavbar;
```




<hr>



### property siteTitle 

```C++
QString CloudWhitelabelInformation::siteTitle;
```




<hr>
## Public Functions Documentation




### function CloudWhitelabelInformation [1/2]

```C++
CloudWhitelabelInformation::CloudWhitelabelInformation () = default
```




<hr>



### function CloudWhitelabelInformation [2/2]

```C++
inline explicit CloudWhitelabelInformation::CloudWhitelabelInformation (
    const QVariantMap & whitelabel
) 
```




<hr>



### function operator!= 

```C++
inline bool CloudWhitelabelInformation::operator!= (
    const CloudWhitelabelInformation & other
) const
```




<hr>



### function operator== 

```C++
inline bool CloudWhitelabelInformation::operator== (
    const CloudWhitelabelInformation & other
) const
```




<hr>



### function toVariantMap 

```C++
inline QVariantMap CloudWhitelabelInformation::toVariantMap () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qfieldcloudutils.h`

