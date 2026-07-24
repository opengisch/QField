

# Struct CloudServerInformation



[**ClassList**](annotated.md) **>** [**CloudServerInformation**](structCloudServerInformation.md)



[More...](#detailed-description)

* `#include <qfieldcloudutils.h>`





























## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**signupUrl**](structCloudServerInformation.md#property-signupurl)  <br> |
| property [**CloudWhitelabelInformation**](structCloudWhitelabelInformation.md) | [**whitelabel**](structCloudServerInformation.md#property-whitelabel)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CloudServerInformation**](#function-cloudserverinformation-12) () = default<br> |
|   | [**CloudServerInformation**](#function-cloudserverinformation-22) (const QVariantMap & serverInformation) <br> |
|  bool | [**operator!=**](#function-operator) (const [**CloudServerInformation**](structCloudServerInformation.md) & other) const<br> |
|  bool | [**operator==**](#function-operator_1) (const [**CloudServerInformation**](structCloudServerInformation.md) & other) const<br> |
|  QVariantMap | [**toVariantMap**](#function-tovariantmap) () const<br> |




























## Detailed Description


Public information about a QFieldCloud server, including whitelabel branding and the new-user signup URL. 


    
## Public Properties Documentation




### property signupUrl 

```C++
QString CloudServerInformation::signupUrl;
```




<hr>



### property whitelabel 

```C++
CloudWhitelabelInformation CloudServerInformation::whitelabel;
```




<hr>
## Public Functions Documentation




### function CloudServerInformation [1/2]

```C++
CloudServerInformation::CloudServerInformation () = default
```




<hr>



### function CloudServerInformation [2/2]

```C++
inline explicit CloudServerInformation::CloudServerInformation (
    const QVariantMap & serverInformation
) 
```




<hr>



### function operator!= 

```C++
inline bool CloudServerInformation::operator!= (
    const CloudServerInformation & other
) const
```




<hr>



### function operator== 

```C++
inline bool CloudServerInformation::operator== (
    const CloudServerInformation & other
) const
```




<hr>



### function toVariantMap 

```C++
inline QVariantMap CloudServerInformation::toVariantMap () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qfieldcloudutils.h`

