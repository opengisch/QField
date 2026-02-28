

# Class NmeaGnssReceiver



[**ClassList**](annotated.md) **>** [**NmeaGnssReceiver**](classNmeaGnssReceiver.md)



[More...](#detailed-description)

* `#include <nmeagnssreceiver.h>`



Inherits the following classes: [AbstractGnssReceiver](classAbstractGnssReceiver.md)


Inherited by the following classes: [BluetoothReceiver](classBluetoothReceiver.md),  [FileReceiver](classFileReceiver.md),  [SerialPortReceiver](classSerialPortReceiver.md),  [TcpReceiver](classTcpReceiver.md),  [UdpReceiver](classUdpReceiver.md)














## Public Types inherited from AbstractGnssReceiver

See [AbstractGnssReceiver](classAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
| enum  | [**Capability**](classAbstractGnssReceiver.md#enum-capability)  <br> |
































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
|   | [**NmeaGnssReceiver**](#function-nmeagnssreceiver) (QObject \* parent=nullptr) <br> |
|  void | [**initNmeaConnection**](#function-initnmeaconnection) (QIODevice \* ioDevice) <br> |
|   | [**~NmeaGnssReceiver**](#function-nmeagnssreceiver) () override<br> |


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














## Protected Attributes

| Type | Name |
| ---: | :--- |
|  bool | [**mLastGnssPositionValid**](#variable-mlastgnsspositionvalid)   = `false`<br> |
|  std::unique\_ptr&lt; QgsNmeaConnection &gt; | [**mNmeaConnection**](#variable-mnmeaconnection)  <br> |


































## Protected Functions inherited from AbstractGnssReceiver

See [AbstractGnssReceiver](classAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
|  void | [**setSocketState**](classAbstractGnssReceiver.md#function-setsocketstate) (const QAbstractSocket::SocketState & state) <br> |






## Detailed Description


The nmeareceiver connects to a device and feeds the QgsNmeaConnection. It receives QgsGpsInformation and converts it to [**GnssPositionInformation**](classGnssPositionInformation.md) 


    
## Public Functions Documentation




### function NmeaGnssReceiver 

```C++
explicit NmeaGnssReceiver::NmeaGnssReceiver (
    QObject * parent=nullptr
) 
```




<hr>



### function initNmeaConnection 

```C++
void NmeaGnssReceiver::initNmeaConnection (
    QIODevice * ioDevice
) 
```




<hr>



### function ~NmeaGnssReceiver 

```C++
NmeaGnssReceiver::~NmeaGnssReceiver () override
```




<hr>
## Protected Attributes Documentation




### variable mLastGnssPositionValid 

```C++
bool NmeaGnssReceiver::mLastGnssPositionValid;
```




<hr>



### variable mNmeaConnection 

```C++
std::unique_ptr<QgsNmeaConnection> NmeaGnssReceiver::mNmeaConnection;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/nmeagnssreceiver.h`

