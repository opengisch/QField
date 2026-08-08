

# Class AuthUtils



[**ClassList**](annotated.md) **>** [**AuthUtils**](classAuthUtils.md)








Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**AuthUtils**](#function-authutils) (QObject \* parent=nullptr) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE QVariantList | [**authenticationConfigurationDetails**](#function-authenticationconfigurationdetails) (const QString & id=QString()) <br> |
|  Q\_INVOKABLE void | [**clearAuthenticationConfigurationCache**](#function-clearauthenticationconfigurationcache) (const QString & id=QString()) <br> |
|  Q\_INVOKABLE bool | [**hasAuthenticationConfigurations**](#function-hasauthenticationconfigurations) () <br> |
|  Q\_INVOKABLE bool | [**isAuthenticationConfigurationAvailable**](#function-isauthenticationconfigurationavailable) (const QString & id) <br> |


























## Public Functions Documentation




### function AuthUtils 

```C++
explicit AuthUtils::AuthUtils (
    QObject * parent=nullptr
) 
```




<hr>
## Public Static Functions Documentation




### function authenticationConfigurationDetails 

```C++
static Q_INVOKABLE QVariantList AuthUtils::authenticationConfigurationDetails (
    const QString & id=QString()
) 
```



Returns authentication configuration _id_ details. If no id is provided, details for all authentication configuration will be returned. 


        

<hr>



### function clearAuthenticationConfigurationCache 

```C++
static Q_INVOKABLE void AuthUtils::clearAuthenticationConfigurationCache (
    const QString & id=QString()
) 
```



Clears any cache associated to the authentication configuration _id_. If no id is provided, cache for all authentication configurations will be cleared. 


        

<hr>



### function hasAuthenticationConfigurations 

```C++
static Q_INVOKABLE bool AuthUtils::hasAuthenticationConfigurations () 
```



Returns TRUE if the authentication manager has one or more stored configurations. 


        

<hr>



### function isAuthenticationConfigurationAvailable 

```C++
static Q_INVOKABLE bool AuthUtils::isAuthenticationConfigurationAvailable (
    const QString & id
) 
```



Returns TRUE if the authentication configuration _id_ is available. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/authutils.h`

