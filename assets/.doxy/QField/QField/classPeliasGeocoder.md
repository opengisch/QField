

# Class PeliasGeocoder



[**ClassList**](annotated.md) **>** [**PeliasGeocoder**](classPeliasGeocoder.md)



_A geocoder which uses the Pelias geocoding API to retrieve results._ [More...](#detailed-description)

* `#include <peliasgeocoder.h>`



Inherits the following classes: QgsGeocoderInterface


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PeliasGeocoder**](#function-peliasgeocoder) (const QString & endpoint=QString()) <br> |
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




**Since:**

QField 1.9 





    
## Public Functions Documentation




### function PeliasGeocoder 

```C++
explicit PeliasGeocoder::PeliasGeocoder (
    const QString & endpoint=QString()
) 
```



Constructor for [**PeliasGeocoder**](classPeliasGeocoder.md).


The _endpoint_ argument is used to specify an endpoint to use for request. 


        

<hr>



### function appendedFields 

```C++
QgsFields PeliasGeocoder::appendedFields () override const
```




<hr>



### function endpoint 

```C++
QString PeliasGeocoder::endpoint () const
```



Returns the API endpoint used for requests.




**See also:** [**setEndpoint()**](classPeliasGeocoder.md#function-setendpoint) 



        

<hr>



### function flags 

```C++
Flags PeliasGeocoder::flags () override const
```




<hr>



### function geocodeString 

```C++
QList< QgsGeocoderResult > PeliasGeocoder::geocodeString (
    const QString & string,
    const QgsGeocoderContext & context,
    QgsFeedback * feedback=nullptr
) override const
```




<hr>



### function jsonToResult 

```C++
QgsGeocoderResult PeliasGeocoder::jsonToResult (
    const QVariantMap & json
) const
```



Converts a JSON result returned from the Pelias service to a geocoder result object. 


        

<hr>



### function requestUrl 

```C++
QUrl PeliasGeocoder::requestUrl (
    const QString & address,
    const QgsRectangle & bounds=QgsRectangle()
) const
```



Returns the URL generated for geocoding the specified _address_. 


        

<hr>



### function requestsPerSecond 

```C++
inline double PeliasGeocoder::requestsPerSecond () const
```



Returns the number of requests per seconds to the endpoint.




**See also:** [**setRequestsPerSecond()**](classPeliasGeocoder.md#function-setrequestspersecond) 



        

<hr>



### function setEndpoint 

```C++
void PeliasGeocoder::setEndpoint (
    const QString & endpoint
) 
```



Sets a specific API _endpoint_ to use for requests. This is for internal testing purposes only.




**See also:** [**endpoint()**](classPeliasGeocoder.md#function-endpoint) 



        

<hr>



### function setRequestsPerSecond 

```C++
inline void PeliasGeocoder::setRequestsPerSecond (
    double number
) 
```



Sets the _number_ of request per seconds to the endpoint.




**See also:** [**requestsPerSecond()**](classPeliasGeocoder.md#function-requestspersecond) 



        

<hr>



### function wkbType 

```C++
Qgis::WkbType PeliasGeocoder::wkbType () override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/peliasgeocoder.h`

