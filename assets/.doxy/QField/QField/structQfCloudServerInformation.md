

# Struct QfCloudServerInformation



[**ClassList**](annotated.md) **>** [**QfCloudServerInformation**](structQfCloudServerInformation.md)



[More...](#detailed-description)

* `#include <qfcloudutils.h>`





























## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**signupUrl**](structQfCloudServerInformation.md#property-signupurl)  <br> |
| property [**QfCloudWhitelabelInformation**](structQfCloudWhitelabelInformation.md) | [**whitelabel**](structQfCloudServerInformation.md#property-whitelabel)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCloudServerInformation**](#function-qfcloudserverinformation-12) () = default<br> |
|   | [**QfCloudServerInformation**](#function-qfcloudserverinformation-22) (const QVariantMap & serverInformation) <br> |
|  bool | [**operator!=**](#function-operator) (const [**QfCloudServerInformation**](structQfCloudServerInformation.md) & other) const<br> |
|  bool | [**operator==**](#function-operator_1) (const [**QfCloudServerInformation**](structQfCloudServerInformation.md) & other) const<br> |
|  QVariantMap | [**toVariantMap**](#function-tovariantmap) () const<br> |




























## Detailed Description


Public information about a QFieldCloud server, including whitelabel branding and the new-user signup URL. 


    
## Public Properties Documentation




### property signupUrl 

```C++
QString QfCloudServerInformation::signupUrl;
```




<hr>



### property whitelabel 

```C++
QfCloudWhitelabelInformation QfCloudServerInformation::whitelabel;
```




<hr>
## Public Functions Documentation




### function QfCloudServerInformation [1/2]

```C++
QfCloudServerInformation::QfCloudServerInformation () = default
```




<hr>



### function QfCloudServerInformation [2/2]

```C++
inline explicit QfCloudServerInformation::QfCloudServerInformation (
    const QVariantMap & serverInformation
) 
```




<hr>



### function operator!= 

```C++
inline bool QfCloudServerInformation::operator!= (
    const QfCloudServerInformation & other
) const
```




<hr>



### function operator== 

```C++
inline bool QfCloudServerInformation::operator== (
    const QfCloudServerInformation & other
) const
```




<hr>



### function toVariantMap 

```C++
inline QVariantMap QfCloudServerInformation::toVariantMap () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qfcloudutils.h`

