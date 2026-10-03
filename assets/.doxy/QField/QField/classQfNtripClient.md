

# Class QfNtripClient



[**ClassList**](annotated.md) **>** [**QfNtripClient**](classQfNtripClient.md)



_A simple NTRIP client._ 

* `#include <qfntripclient.h>`



Inherits the following classes: QObject






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**bytesReceivedChanged**](classQfNtripClient.md#signal-bytesreceivedchanged)  <br>_Emitted when the received bytes counts have changed._  |
| signal void | [**bytesSentChanged**](classQfNtripClient.md#signal-bytessentchanged)  <br>_Emitted when the sent bytes counts have changed._  |
| signal void | [**correctionDataReceived**](classQfNtripClient.md#signal-correctiondatareceived) (const QByteArray & data) <br>_Emmitted when correction_ _data_ _has been received from an NTRIP server._ |
| signal void | [**errorOccurred**](classQfNtripClient.md#signal-erroroccurred) (const QString & message, bool isPermanent) <br>_Emmitted when an error occured while communicating with an NTRIP server._  |
| signal void | [**streamConnected**](classQfNtripClient.md#signal-streamconnected)  <br>_Emitted when successfully connected to an NTRIP server._  |
| signal void | [**streamDisconnected**](classQfNtripClient.md#signal-streamdisconnected)  <br>_Emitted when successfully disconnected from an NTRIP server._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfNtripClient**](#function-qfntripclient) (QObject \* parent=nullptr) <br>_The NTRIP client constructor._  |
|  qint64 | [**bytesReceived**](#function-bytesreceived) () const<br> |
|  qint64 | [**bytesSent**](#function-bytessent) () const<br> |
|  void | [**sendNmeaSentence**](#function-sendnmeasentence) (const QString & sentence) <br>_Sends an NMEA_ _sentence_ _to the NTRIP server._ |
|  void | [**start**](#function-start) (const [**QfNtripSettings**](classQfNtripSettings.md) & ntripSettings, [**QfAbstractGnssReceiver**](classQfAbstractGnssReceiver.md) \* receiver) <br> |
|  void | [**startLogging**](#function-startlogging) (const QString & path) <br> |
|  void | [**stop**](#function-stop) () <br>_Stops any ongoing connection to an NTRIP server._  |
|  void | [**stopLogging**](#function-stoplogging) () <br>_Stops logging received corrections._  |
|   | [**~QfNtripClient**](#function-qfntripclient) () noexcept override<br>_The NTRIP client destructor._  |




























## Public Signals Documentation




### signal bytesReceivedChanged 

_Emitted when the received bytes counts have changed._ 
```C++
void QfNtripClient::bytesReceivedChanged;
```




<hr>



### signal bytesSentChanged 

_Emitted when the sent bytes counts have changed._ 
```C++
void QfNtripClient::bytesSentChanged;
```




<hr>



### signal correctionDataReceived 

_Emmitted when correction_ _data_ _has been received from an NTRIP server._
```C++
void QfNtripClient::correctionDataReceived;
```




<hr>



### signal errorOccurred 

_Emmitted when an error occured while communicating with an NTRIP server._ 
```C++
void QfNtripClient::errorOccurred;
```




<hr>



### signal streamConnected 

_Emitted when successfully connected to an NTRIP server._ 
```C++
void QfNtripClient::streamConnected;
```




<hr>



### signal streamDisconnected 

_Emitted when successfully disconnected from an NTRIP server._ 
```C++
void QfNtripClient::streamDisconnected;
```




<hr>
## Public Functions Documentation




### function QfNtripClient 

_The NTRIP client constructor._ 
```C++
explicit QfNtripClient::QfNtripClient (
    QObject * parent=nullptr
) 
```




<hr>



### function bytesReceived 

```C++
inline qint64 QfNtripClient::bytesReceived () const
```



Returns the number of bytes received since a connection to an NTRIP server has been started. 


        

<hr>



### function bytesSent 

```C++
inline qint64 QfNtripClient::bytesSent () const
```



Returns the number of bytes sent since a connection to an NTRIP server has been started. 


        

<hr>



### function sendNmeaSentence 

_Sends an NMEA_ _sentence_ _to the NTRIP server._
```C++
void QfNtripClient::sendNmeaSentence (
    const QString & sentence
) 
```




<hr>



### function start 

```C++
void QfNtripClient::start (
    const QfNtripSettings & ntripSettings,
    QfAbstractGnssReceiver * receiver
) 
```



Starts a connection to an NTRIP server 

**Parameters:**


* `ntripSettings` the NTRIP settings object 
* `receiver` the GNSS receiver that will receive corrections and, optionally, forward NMEA sentence from 




        

<hr>



### function startLogging 

```C++
void QfNtripClient::startLogging (
    const QString & path
) 
```



Start logging received correction to a given folder _path_ 

**Note:**

the actual log file name will be generated to include the current date and time 





        

<hr>



### function stop 

_Stops any ongoing connection to an NTRIP server._ 
```C++
void QfNtripClient::stop () 
```




<hr>



### function stopLogging 

_Stops logging received corrections._ 
```C++
void QfNtripClient::stopLogging () 
```




<hr>



### function ~QfNtripClient 

_The NTRIP client destructor._ 
```C++
QfNtripClient::~QfNtripClient () noexcept override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/qfntripclient.h`

