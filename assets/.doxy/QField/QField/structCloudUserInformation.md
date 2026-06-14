

# Struct CloudUserInformation



[**ClassList**](annotated.md) **>** [**CloudUserInformation**](structCloudUserInformation.md)


































## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**email**](structCloudUserInformation.md#property-email)  <br> |
| property QString | [**username**](structCloudUserInformation.md#property-username)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CloudUserInformation**](#function-clouduserinformation-13) () = default<br> |
|   | [**CloudUserInformation**](#function-clouduserinformation-23) (const QString & username, const QString & email) <br> |
|   | [**CloudUserInformation**](#function-clouduserinformation-33) (const QJsonObject cloudUserInformation) <br> |
|  bool | [**isEmpty**](#function-isempty) () const<br> |
|  bool | [**operator==**](#function-operator) (const [**CloudUserInformation**](structCloudUserInformation.md) & other) const<br> |
|  QJsonObject | [**toJson**](#function-tojson) () const<br> |




























## Public Properties Documentation




### property email 

```C++
QString CloudUserInformation::email;
```




<hr>



### property username 

```C++
QString CloudUserInformation::username;
```




<hr>
## Public Functions Documentation




### function CloudUserInformation [1/3]

```C++
CloudUserInformation::CloudUserInformation () = default
```




<hr>



### function CloudUserInformation [2/3]

```C++
inline CloudUserInformation::CloudUserInformation (
    const QString & username,
    const QString & email
) 
```




<hr>



### function CloudUserInformation [3/3]

```C++
inline explicit CloudUserInformation::CloudUserInformation (
    const QJsonObject cloudUserInformation
) 
```




<hr>



### function isEmpty 

```C++
inline bool CloudUserInformation::isEmpty () const
```




<hr>



### function operator== 

```C++
inline bool CloudUserInformation::operator== (
    const CloudUserInformation & other
) const
```




<hr>



### function toJson 

```C++
inline QJsonObject CloudUserInformation::toJson () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qfieldcloudutils.h`

