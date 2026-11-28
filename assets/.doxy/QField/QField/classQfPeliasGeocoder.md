

# Class QfPeliasGeocoder



[**ClassList**](annotated.md) **>** [**QfPeliasGeocoder**](classQfPeliasGeocoder.md)



_A geocoder which uses the Pelias geocoding API to retrieve results._ [More...](#detailed-description)

* `#include <qfpeliasgeocoder.h>`



Inherits the following classes: QgsGeocoderInterface


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfPeliasGeocoder**](#function-qfpeliasgeocoder) (const QString & endpoint=QString()) <br> |
|  QgsFields | [**appendedFields**](#function-appendedfields) () override const<br> |
|  QString | [**endpoint**](#function-endpoint) () const<br> |
|  Flags | [**flags**](#function-flags) () override const<br> |
|  QList&lt; QgsGeocoderResult &gt; | [**geocodeString**](#function-geocodestring) (const QString & string, const QgsGeocoderContext & context, QgsFeedback \* feedback=nullptr) override const<br> |
|  QgsGeocoderResult | [**jsonToResult**](#function-jsontoresult) (const QVariantMap & json) const<br> |
|  QUrl | [**requestUrl**](#function-requesturl) (const QString & address, const QgsRectangle & bounds=QgsRectangle()) const<br> |
|  double | [**requestsPerSecond**](#function-requestspersecond) () const<br> |
|  void | [**setEndpoint**](#function-setendpoint) (const QString & endpoint) <br> |
|  void | [**setRequestsPerSecond**](#function-setrequestspersecond) (double number) <br> |
|  Qgis::WkbType | [**wkbType**](#function-wkbtype) () override const<br> |




























## Detailed Description


This geocoder utilizes the Pelias geocoding API in order to geocode strings from an endpoint server. 


    
## Public Functions Documentation




### function QfPeliasGeocoder 

```C++
explicit QfPeliasGeocoder::QfPeliasGeocoder (
    const QString & endpoint=QString()
) 
```



Constructor for [**QfPeliasGeocoder**](classQfPeliasGeocoder.md).


The _endpoint_ argument is used to specify an endpoint to use for request. 


        

<hr>



### function appendedFields 

```C++
QgsFields QfPeliasGeocoder::appendedFields () override const
```




<hr>



### function endpoint 

```C++
QString QfPeliasGeocoder::endpoint () const
```



Returns the API endpoint used for requests.




**See also:** [**setEndpoint()**](classQfPeliasGeocoder.md#function-setendpoint) 



        

<hr>



### function flags 

```C++
Flags QfPeliasGeocoder::flags () override const
```




<hr>



### function geocodeString 

```C++
QList< QgsGeocoderResult > QfPeliasGeocoder::geocodeString (
    const QString & string,
    const QgsGeocoderContext & context,
    QgsFeedback * feedback=nullptr
) override const
```




<hr>



### function jsonToResult 

```C++
QgsGeocoderResult QfPeliasGeocoder::jsonToResult (
    const QVariantMap & json
) const
```



Converts a JSON result returned from the Pelias service to a geocoder result object. 


        

<hr>



### function requestUrl 

```C++
QUrl QfPeliasGeocoder::requestUrl (
    const QString & address,
    const QgsRectangle & bounds=QgsRectangle()
) const
```



Returns the URL generated for geocoding the specified _address_. 


        

<hr>



### function requestsPerSecond 

```C++
inline double QfPeliasGeocoder::requestsPerSecond () const
```



Returns the number of requests per seconds to the endpoint.




**See also:** [**setRequestsPerSecond()**](classQfPeliasGeocoder.md#function-setrequestspersecond) 



        

<hr>



### function setEndpoint 

```C++
void QfPeliasGeocoder::setEndpoint (
    const QString & endpoint
) 
```



Sets a specific API _endpoint_ to use for requests. This is for internal testing purposes only.




**See also:** [**endpoint()**](classQfPeliasGeocoder.md#function-endpoint) 



        

<hr>



### function setRequestsPerSecond 

```C++
inline void QfPeliasGeocoder::setRequestsPerSecond (
    double number
) 
```



Sets the _number_ of request per seconds to the endpoint.




**See also:** [**requestsPerSecond()**](classQfPeliasGeocoder.md#function-requestspersecond) 



        

<hr>



### function wkbType 

```C++
Qgis::WkbType QfPeliasGeocoder::wkbType () override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfpeliasgeocoder.h`

