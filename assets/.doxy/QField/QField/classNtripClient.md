

# Class NtripClient



[**ClassList**](annotated.md) **>** [**NtripClient**](classNtripClient.md)



_A simple NTRIP client._ 

* `#include <ntripclient.h>`



Inherits the following classes: QObject






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**bytesCountersChanged**](classNtripClient.md#signal-bytescounterschanged)  <br>_Emitted when the bytes sent and received counts have changed._  |
| signal void | [**correctionDataReceived**](classNtripClient.md#signal-correctiondatareceived) (const QByteArray & data) <br>_Emmitted when correction_ _data_ _has been received from an NTRIP server._ |
| signal void | [**errorOccurred**](classNtripClient.md#signal-erroroccurred) (const QString & message, bool isPermanent) <br>_Emmitted when an error occured while communicating with an NTRIP server._  |
| signal void | [**streamConnected**](classNtripClient.md#signal-streamconnected)  <br>_Emitted when successfully connected to an NTRIP server._  |
| signal void | [**streamDisconnected**](classNtripClient.md#signal-streamdisconnected)  <br>_Emitted when successfully disconnected from an NTRIP server._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**NtripClient**](#function-ntripclient) (QObject \* parent=nullptr) <br>_The NTRIP client constructor._  |
|  qint64 | [**bytesReceived**](#function-bytesreceived) () const<br> |
|  qint64 | [**bytesSent**](#function-bytessent) () const<br> |
|  void | [**sendNmeaSentence**](#function-sendnmeasentence) (const QString & sentence) <br>_Sends an NMEA_ _sentence_ _to the NTRIP server._ |
|  void | [**start**](#function-start) (const [**NtripSettings**](classNtripSettings.md) & ntripSettings, [**AbstractGnssReceiver**](classAbstractGnssReceiver.md) \* receiver) <br> |
|  void | [**startLogging**](#function-startlogging) (const QString & path) <br> |
|  void | [**stop**](#function-stop) () <br>_Stops any ongoing connection to an NTRIP server._  |
|  void | [**stopLogging**](#function-stoplogging) () <br>_Stops logging received corrections._  |
|   | [**~NtripClient**](#function-ntripclient) () noexcept override<br>_The NTRIP client destructor._  |




























## Public Signals Documentation




### signal bytesCountersChanged 

_Emitted when the bytes sent and received counts have changed._ 
```C++
void NtripClient::bytesCountersChanged;
```




<hr>



### signal correctionDataReceived 

_Emmitted when correction_ _data_ _has been received from an NTRIP server._
```C++
void NtripClient::correctionDataReceived;
```




<hr>



### signal errorOccurred 

_Emmitted when an error occured while communicating with an NTRIP server._ 
```C++
void NtripClient::errorOccurred;
```




<hr>



### signal streamConnected 

_Emitted when successfully connected to an NTRIP server._ 
```C++
void NtripClient::streamConnected;
```




<hr>



### signal streamDisconnected 

_Emitted when successfully disconnected from an NTRIP server._ 
```C++
void NtripClient::streamDisconnected;
```




<hr>
## Public Functions Documentation




### function NtripClient 

_The NTRIP client constructor._ 
```C++
explicit NtripClient::NtripClient (
    QObject * parent=nullptr
) 
```




<hr>



### function bytesReceived 

```C++
inline qint64 NtripClient::bytesReceived () const
```



Returns the number of bytes received since a connection to an NTRIP server has been started. 


        

<hr>



### function bytesSent 

```C++
inline qint64 NtripClient::bytesSent () const
```



Returns the number of bytes sent since a connection to an NTRIP server has been started. 


        

<hr>



### function sendNmeaSentence 

_Sends an NMEA_ _sentence_ _to the NTRIP server._
```C++
void NtripClient::sendNmeaSentence (
    const QString & sentence
) 
```




<hr>



### function start 

```C++
void NtripClient::start (
    const NtripSettings & ntripSettings,
    AbstractGnssReceiver * receiver
) 
```



Starts a connection to an NTRIP server 

**Parameters:**


* `ntripSettings` the NTRIP settings object 
* `receiver` the GNSS receiver that will receive corrections and, optionally, forward NMEA sentence from 




        

<hr>



### function startLogging 

```C++
void NtripClient::startLogging (
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
void NtripClient::stop () 
```




<hr>



### function stopLogging 

_Stops logging received corrections._ 
```C++
void NtripClient::stopLogging () 
```




<hr>



### function ~NtripClient 

_The NTRIP client destructor._ 
```C++
NtripClient::~NtripClient () noexcept override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/ntripclient.h`

