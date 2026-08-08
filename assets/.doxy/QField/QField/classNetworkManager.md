

# Class NetworkManager



[**ClassList**](annotated.md) **>** [**NetworkManager**](classNetworkManager.md)



[More...](#detailed-description)

* `#include <networkmanager.h>`







































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**NetworkReply**](classNetworkReply.md) \* | [**deleteResource**](#function-deleteresource) (const QNetworkRequest & request, const QByteArray & payload=QByteArray()) <br> |
|  [**NetworkReply**](classNetworkReply.md) \* | [**get**](#function-get) (const QNetworkRequest & request) <br> |
|  [**NetworkReply**](classNetworkReply.md) \* | [**post**](#function-post-12) (const QNetworkRequest & request, const QByteArray & payload=QByteArray()) <br> |
|  [**NetworkReply**](classNetworkReply.md) \* | [**post**](#function-post-22) (const QNetworkRequest & request, QHttpMultiPart \* payload) <br> |
|  [**NetworkReply**](classNetworkReply.md) \* | [**put**](#function-put-12) (const QNetworkRequest & request, const QByteArray & payload=QByteArray()) <br> |
|  [**NetworkReply**](classNetworkReply.md) \* | [**put**](#function-put-22) (const QNetworkRequest & request, QHttpMultiPart \* payload) <br> |


























## Detailed Description


[**NetworkManager**](classNetworkManager.md) is used to make reliable and retriable network requests. It wraps around `QgsNetworkAccessManager`. 

**Note:**

After the request has finished, it is the responsibility of the user to delete the [**NetworkReply**](classNetworkReply.md) object at an appropriate time. 




**Note:**

Do not directly delete it inside the slot connected to finished(). You can use the deleteLater() function. 





    
## Public Static Functions Documentation




### function deleteResource 

```C++
static NetworkReply * NetworkManager::deleteResource (
    const QNetworkRequest & request,
    const QByteArray & payload=QByteArray()
) 
```



makes HTTP DELETE _request_ with an optional _payload_ and returns a reply 


        

<hr>



### function get 

```C++
static NetworkReply * NetworkManager::get (
    const QNetworkRequest & request
) 
```



makes HTTP GET _request_ and returns a reply. 


        

<hr>



### function post [1/2]

```C++
static NetworkReply * NetworkManager::post (
    const QNetworkRequest & request,
    const QByteArray & payload=QByteArray()
) 
```



makes HTTP POST _request_ with an optional _payload_ and returns a reply 


        

<hr>



### function post [2/2]

```C++
static NetworkReply * NetworkManager::post (
    const QNetworkRequest & request,
    QHttpMultiPart * payload
) 
```



makes HTTP POST _request_ with a multipart _payload_ and returns a reply 


        

<hr>



### function put [1/2]

```C++
static NetworkReply * NetworkManager::put (
    const QNetworkRequest & request,
    const QByteArray & payload=QByteArray()
) 
```



makes HTTP PUT _request_ with an optional _payload_ and returns a reply 


        

<hr>



### function put [2/2]

```C++
static NetworkReply * NetworkManager::put (
    const QNetworkRequest & request,
    QHttpMultiPart * payload
) 
```



makes HTTP PUT _request_ with a multipart _payload_ and returns a reply 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/networkmanager.h`

