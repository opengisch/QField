

# Class TcpReceiver



[**ClassList**](annotated.md) **>** [**TcpReceiver**](classTcpReceiver.md)



[More...](#detailed-description)

* `#include <tcpreceiver.h>`



Inherits the following classes: [NmeaGnssReceiver](classNmeaGnssReceiver.md)


















## Public Types inherited from AbstractGnssReceiver

See [AbstractGnssReceiver](classAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
| enum  | [**Capability**](classAbstractGnssReceiver.md#enum-capability)  <br> |














## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  QLatin1String | [**identifier**](#variable-identifier)   = `QLatin1String( "tcp" )`<br> |






## Public Slots

| Type | Name |
| ---: | :--- |
| slot QString | [**socketStateString**](classTcpReceiver.md#slot-socketstatestring)  <br> |




























## Public Signals inherited from AbstractGnssReceiver

See [AbstractGnssReceiver](classAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
| signal void | [**lastErrorChanged**](classAbstractGnssReceiver.md#signal-lasterrorchanged) (const QString & lastError) <br> |
| signal void | [**lastGnssPositionInformationChanged**](classAbstractGnssReceiver.md#signal-lastgnsspositioninformationchanged) (const [**GnssPositionInformation**](classGnssPositionInformation.md) & lastGnssPositionInformation) <br> |
| signal void | [**socketStateChanged**](classAbstractGnssReceiver.md#signal-socketstatechanged) (const QAbstractSocket::SocketState socketState) <br> |
| signal void | [**socketStateStringChanged**](classAbstractGnssReceiver.md#signal-socketstatestringchanged) (const QString & socketStateString) <br> |
| signal void | [**validChanged**](classAbstractGnssReceiver.md#signal-validchanged)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**TcpReceiver**](#function-tcpreceiver) (const QString & address=QString(), const int port=0, QObject \* parent=nullptr) <br> |
|   | [**~TcpReceiver**](#function-tcpreceiver) () override<br> |


## Public Functions inherited from NmeaGnssReceiver

See [NmeaGnssReceiver](classNmeaGnssReceiver.md)

| Type | Name |
| ---: | :--- |
|   | [**NmeaGnssReceiver**](classNmeaGnssReceiver.md#function-nmeagnssreceiver) (QObject \* parent=nullptr) <br> |
|  void | [**initNmeaConnection**](classNmeaGnssReceiver.md#function-initnmeaconnection) (QIODevice \* ioDevice) <br> |
|   | [**~NmeaGnssReceiver**](classNmeaGnssReceiver.md#function-nmeagnssreceiver) () override<br> |


## Public Functions inherited from AbstractGnssReceiver

See [AbstractGnssReceiver](classAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
|   | [**AbstractGnssReceiver**](classAbstractGnssReceiver.md#function-abstractgnssreceiver) (QObject \* parent=nullptr) <br> |
|  void | [**connectDevice**](classAbstractGnssReceiver.md#function-connectdevice) () <br> |
| virtual [**GnssPositionDetails**](classGnssPositionDetails.md) | [**details**](classAbstractGnssReceiver.md#function-details) () const<br> |
|  void | [**disconnectDevice**](classAbstractGnssReceiver.md#function-disconnectdevice) () <br> |
|  QString | [**lastError**](classAbstractGnssReceiver.md#function-lasterror) () const<br> |
|  [**GnssPositionInformation**](classGnssPositionInformation.md) | [**lastGnssPositionInformation**](classAbstractGnssReceiver.md#function-lastgnsspositioninformation) () const<br> |
|  void | [**setValid**](classAbstractGnssReceiver.md#function-setvalid) (bool valid) <br> |
| virtual QAbstractSocket::SocketState | [**socketState**](classAbstractGnssReceiver.md#function-socketstate) () const<br> |
| virtual QString | [**socketStateString**](classAbstractGnssReceiver.md#function-socketstatestring) () <br> |
|  void | [**startLogging**](classAbstractGnssReceiver.md#function-startlogging) (const QString & path) <br> |
|  void | [**stopLogging**](classAbstractGnssReceiver.md#function-stoplogging) () <br> |
|  bool | [**valid**](classAbstractGnssReceiver.md#function-valid) () const<br> |
| virtual  | [**~AbstractGnssReceiver**](classAbstractGnssReceiver.md#function-abstractgnssreceiver) () = default<br> |






















## Protected Attributes inherited from NmeaGnssReceiver

See [NmeaGnssReceiver](classNmeaGnssReceiver.md)

| Type | Name |
| ---: | :--- |
|  bool | [**mLastGnssPositionValid**](classNmeaGnssReceiver.md#variable-mlastgnsspositionvalid)   = `false`<br> |
|  std::unique\_ptr&lt; QgsNmeaConnection &gt; | [**mNmeaConnection**](classNmeaGnssReceiver.md#variable-mnmeaconnection)  <br> |


















































## Protected Functions inherited from AbstractGnssReceiver

See [AbstractGnssReceiver](classAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
|  void | [**setSocketState**](classAbstractGnssReceiver.md#function-setsocketstate) (const QAbstractSocket::SocketState & state) <br> |








## Detailed Description


The [**TcpReceiver**](classTcpReceiver.md) connects to a device and feeds the QgsNmeaConnection over a TCP socket. It receives QgsGpsInformation and converts it to [**GnssPositionInformation**](classGnssPositionInformation.md) 


    
## Public Static Attributes Documentation




### variable identifier 

```C++
QLatin1String TcpReceiver::identifier;
```




<hr>
## Public Slots Documentation




### slot socketStateString 

```C++
QString TcpReceiver::socketStateString;
```




<hr>
## Public Functions Documentation




### function TcpReceiver 

```C++
explicit TcpReceiver::TcpReceiver (
    const QString & address=QString(),
    const int port=0,
    QObject * parent=nullptr
) 
```




<hr>



### function ~TcpReceiver 

```C++
TcpReceiver::~TcpReceiver () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/tcpreceiver.h`

