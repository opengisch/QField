

# Class NtripSocket



[**ClassList**](annotated.md) **>** [**NtripSocket**](classNtripSocket.md)



_A simple NTRIP socket._ 

* `#include <ntripclient.h>`



Inherits the following classes: QObject






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**correctionDataReceived**](classNtripSocket.md#signal-correctiondatareceived) (const QByteArray & data) <br>_Emmitted when the socket has received_ _data_ _from an NTRIP server._ |
| signal void | [**errorOccurred**](classNtripSocket.md#signal-erroroccurred) (const QString & message, bool isPermanent) <br>_Emmitted when an error occured while the socket communicates with an NTRIP server._  |
| signal void | [**streamConnected**](classNtripSocket.md#signal-streamconnected)  <br>_Emitted when the socket has successfully connected to an NTRIP server._  |
| signal void | [**streamDisconnected**](classNtripSocket.md#signal-streamdisconnected)  <br>_Emitted when the socket has disconnected from an NTRIP server._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**NtripSocket**](#function-ntripsocket) (QObject \* parent=nullptr) <br>_The NTRIP socket constructor._  |
|  void | [**abort**](#function-abort) () <br>_Abort any socket connection to an NTRIP server._  |
|  qint64 | [**connectToHost**](#function-connecttohost) (const [**NtripSettings**](classNtripSettings.md) & ntripSettings) <br> |
|  qint64 | [**writeNmeaSentence**](#function-writenmeasentence) (const QByteArray & sentence) <br>_Write an NMEA_ _sentence_ _to the NTRIP server._ |
|   | [**~NtripSocket**](#function-ntripsocket) () noexcept override<br>_The NTRIP socket destructor._  |




























## Public Signals Documentation




### signal correctionDataReceived 

_Emmitted when the socket has received_ _data_ _from an NTRIP server._
```C++
void NtripSocket::correctionDataReceived;
```




<hr>



### signal errorOccurred 

_Emmitted when an error occured while the socket communicates with an NTRIP server._ 
```C++
void NtripSocket::errorOccurred;
```




<hr>



### signal streamConnected 

_Emitted when the socket has successfully connected to an NTRIP server._ 
```C++
void NtripSocket::streamConnected;
```




<hr>



### signal streamDisconnected 

_Emitted when the socket has disconnected from an NTRIP server._ 
```C++
void NtripSocket::streamDisconnected;
```




<hr>
## Public Functions Documentation




### function NtripSocket 

_The NTRIP socket constructor._ 
```C++
explicit NtripSocket::NtripSocket (
    QObject * parent=nullptr
) 
```




<hr>



### function abort 

_Abort any socket connection to an NTRIP server._ 
```C++
void NtripSocket::abort () 
```




<hr>



### function connectToHost 

```C++
qint64 NtripSocket::connectToHost (
    const NtripSettings & ntripSettings
) 
```



Connects the socket to an NTRIP server 

**Parameters:**


* `ntripSettings` the NTRIP settings object 




        

<hr>



### function writeNmeaSentence 

_Write an NMEA_ _sentence_ _to the NTRIP server._
```C++
qint64 NtripSocket::writeNmeaSentence (
    const QByteArray & sentence
) 
```




<hr>



### function ~NtripSocket 

_The NTRIP socket destructor._ 
```C++
NtripSocket::~NtripSocket () noexcept override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/ntripclient.h`

