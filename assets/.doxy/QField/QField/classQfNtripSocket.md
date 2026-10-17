

# Class QfNtripSocket



[**ClassList**](annotated.md) **>** [**QfNtripSocket**](classQfNtripSocket.md)



_A simple NTRIP socket._ 

* `#include <qfntripclient.h>`



Inherits the following classes: QObject






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**correctionDataReceived**](classQfNtripSocket.md#signal-correctiondatareceived) (const QByteArray & data) <br>_Emmitted when the socket has received_ _data_ _from an NTRIP server._ |
| signal void | [**errorOccurred**](classQfNtripSocket.md#signal-erroroccurred) (const QString & message, bool isPermanent) <br>_Emmitted when an error occured while the socket communicates with an NTRIP server._  |
| signal void | [**streamConnected**](classQfNtripSocket.md#signal-streamconnected)  <br>_Emitted when the socket has successfully connected to an NTRIP server._  |
| signal void | [**streamDisconnected**](classQfNtripSocket.md#signal-streamdisconnected)  <br>_Emitted when the socket has disconnected from an NTRIP server._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfNtripSocket**](#function-qfntripsocket) (QObject \* parent=nullptr) <br>_The NTRIP socket constructor._  |
|  void | [**abort**](#function-abort) () <br>_Abort any socket connection to an NTRIP server._  |
|  qint64 | [**connectToHost**](#function-connecttohost) (const [**QfNtripSettings**](classQfNtripSettings.md) & ntripSettings) <br> |
|  qint64 | [**writeNmeaSentence**](#function-writenmeasentence) (const QByteArray & sentence) <br>_Write an NMEA_ _sentence_ _to the NTRIP server._ |
|   | [**~QfNtripSocket**](#function-qfntripsocket) () noexcept override<br>_The NTRIP socket destructor._  |




























## Public Signals Documentation




### signal correctionDataReceived 

_Emmitted when the socket has received_ _data_ _from an NTRIP server._
```C++
void QfNtripSocket::correctionDataReceived;
```




<hr>



### signal errorOccurred 

_Emmitted when an error occured while the socket communicates with an NTRIP server._ 
```C++
void QfNtripSocket::errorOccurred;
```




<hr>



### signal streamConnected 

_Emitted when the socket has successfully connected to an NTRIP server._ 
```C++
void QfNtripSocket::streamConnected;
```




<hr>



### signal streamDisconnected 

_Emitted when the socket has disconnected from an NTRIP server._ 
```C++
void QfNtripSocket::streamDisconnected;
```




<hr>
## Public Functions Documentation




### function QfNtripSocket 

_The NTRIP socket constructor._ 
```C++
explicit QfNtripSocket::QfNtripSocket (
    QObject * parent=nullptr
) 
```




<hr>



### function abort 

_Abort any socket connection to an NTRIP server._ 
```C++
void QfNtripSocket::abort () 
```




<hr>



### function connectToHost 

```C++
qint64 QfNtripSocket::connectToHost (
    const QfNtripSettings & ntripSettings
) 
```



Connects the socket to an NTRIP server 

**Parameters:**


* `ntripSettings` the NTRIP settings object 




        

<hr>



### function writeNmeaSentence 

_Write an NMEA_ _sentence_ _to the NTRIP server._
```C++
qint64 QfNtripSocket::writeNmeaSentence (
    const QByteArray & sentence
) 
```




<hr>



### function ~QfNtripSocket 

_The NTRIP socket destructor._ 
```C++
QfNtripSocket::~QfNtripSocket () noexcept override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/qfntripclient.h`

