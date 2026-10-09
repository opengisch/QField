

# Class QfFileReceiver



[**ClassList**](annotated.md) **>** [**QfFileReceiver**](classQfFileReceiver.md)



[More...](#detailed-description)

* `#include <qffilereceiver.h>`



Inherits the following classes: [QfNmeaGnssReceiver](classQfNmeaGnssReceiver.md)


















## Public Types inherited from QfAbstractGnssReceiver

See [QfAbstractGnssReceiver](classQfAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
| enum  | [**Capability**](classQfAbstractGnssReceiver.md#enum-capability)  <br> |














## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  QLatin1String | [**identifier**](#variable-identifier)   = `QLatin1String( "file" )`<br> |








## Public Slots inherited from QfNmeaGnssReceiver

See [QfNmeaGnssReceiver](classQfNmeaGnssReceiver.md)

| Type | Name |
| ---: | :--- |
| slot void | [**onCorrectionDataReceived**](classQfNmeaGnssReceiver.md#slot-oncorrectiondatareceived) (const QByteArray & data) <br> |


## Public Slots inherited from QfAbstractGnssReceiver

See [QfAbstractGnssReceiver](classQfAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
| slot void | [**onCorrectionDataReceived**](classQfAbstractGnssReceiver.md#slot-oncorrectiondatareceived) (const QByteArray & data) <br> |






















## Public Signals inherited from QfNmeaGnssReceiver

See [QfNmeaGnssReceiver](classQfNmeaGnssReceiver.md)

| Type | Name |
| ---: | :--- |
| signal void | [**nmeaSentenceReceived**](classQfNmeaGnssReceiver.md#signal-nmeasentencereceived) (const QString & sentence) <br> |


## Public Signals inherited from QfAbstractGnssReceiver

See [QfAbstractGnssReceiver](classQfAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
| signal void | [**batteryLevelChanged**](classQfAbstractGnssReceiver.md#signal-batterylevelchanged) (const double batteryLevel) <br> |
| signal void | [**lastErrorChanged**](classQfAbstractGnssReceiver.md#signal-lasterrorchanged) (const QString & lastError) <br> |
| signal void | [**lastGnssPositionInformationChanged**](classQfAbstractGnssReceiver.md#signal-lastgnsspositioninformationchanged) (const [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) & lastGnssPositionInformation) <br> |
| signal void | [**socketStateChanged**](classQfAbstractGnssReceiver.md#signal-socketstatechanged) (const QAbstractSocket::SocketState socketState) <br> |
| signal void | [**socketStateStringChanged**](classQfAbstractGnssReceiver.md#signal-socketstatestringchanged) (const QString & socketStateString) <br> |
| signal void | [**validChanged**](classQfAbstractGnssReceiver.md#signal-validchanged)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfFileReceiver**](#function-qffilereceiver) (const QString & filePath=QString(), const int interval=0, QObject \* parent=nullptr) <br> |
|   | [**~QfFileReceiver**](#function-qffilereceiver) () <br> |


## Public Functions inherited from QfNmeaGnssReceiver

See [QfNmeaGnssReceiver](classQfNmeaGnssReceiver.md)

| Type | Name |
| ---: | :--- |
|   | [**QfNmeaGnssReceiver**](classQfNmeaGnssReceiver.md#function-qfnmeagnssreceiver) (QObject \* parent=nullptr) <br> |
| virtual QfAbstractGnssReceiver::Capabilities | [**capabilities**](classQfNmeaGnssReceiver.md#function-capabilities) () override const<br> |
|  void | [**initNmeaConnection**](classQfNmeaGnssReceiver.md#function-initnmeaconnection) (QIODevice \* ioDevice) <br> |
|   | [**~QfNmeaGnssReceiver**](classQfNmeaGnssReceiver.md#function-qfnmeagnssreceiver) () override<br> |


## Public Functions inherited from QfAbstractGnssReceiver

See [QfAbstractGnssReceiver](classQfAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
|   | [**QfAbstractGnssReceiver**](classQfAbstractGnssReceiver.md#function-qfabstractgnssreceiver) (QObject \* parent=nullptr) <br> |
|  double | [**batteryLevel**](classQfAbstractGnssReceiver.md#function-batterylevel) () const<br> |
| virtual QfAbstractGnssReceiver::Capabilities | [**capabilities**](classQfAbstractGnssReceiver.md#function-capabilities) () const<br> |
|  void | [**connectDevice**](classQfAbstractGnssReceiver.md#function-connectdevice) () <br> |
| virtual [**QfGnssPositionDetails**](classQfGnssPositionDetails.md) | [**details**](classQfAbstractGnssReceiver.md#function-details) () const<br> |
|  void | [**disconnectDevice**](classQfAbstractGnssReceiver.md#function-disconnectdevice) () <br> |
|  QString | [**lastError**](classQfAbstractGnssReceiver.md#function-lasterror) () const<br> |
|  [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) | [**lastGnssPositionInformation**](classQfAbstractGnssReceiver.md#function-lastgnsspositioninformation) () const<br> |
|  void | [**setValid**](classQfAbstractGnssReceiver.md#function-setvalid) (bool valid) <br> |
| virtual QAbstractSocket::SocketState | [**socketState**](classQfAbstractGnssReceiver.md#function-socketstate) () const<br> |
| virtual QString | [**socketStateString**](classQfAbstractGnssReceiver.md#function-socketstatestring) () <br> |
|  void | [**startLogging**](classQfAbstractGnssReceiver.md#function-startlogging) (const QString & path) <br> |
|  void | [**stopLogging**](classQfAbstractGnssReceiver.md#function-stoplogging) () <br> |
|  bool | [**valid**](classQfAbstractGnssReceiver.md#function-valid) () const<br> |
| virtual  | [**~QfAbstractGnssReceiver**](classQfAbstractGnssReceiver.md#function-qfabstractgnssreceiver) () = default<br> |






















## Protected Attributes inherited from QfNmeaGnssReceiver

See [QfNmeaGnssReceiver](classQfNmeaGnssReceiver.md)

| Type | Name |
| ---: | :--- |
|  bool | [**mLastGnssPositionValid**](classQfNmeaGnssReceiver.md#variable-mlastgnsspositionvalid)   = `false`<br> |
|  std::unique\_ptr&lt; QgsNmeaConnection &gt; | [**mNmeaConnection**](classQfNmeaGnssReceiver.md#variable-mnmeaconnection)  <br> |


















































## Protected Functions inherited from QfAbstractGnssReceiver

See [QfAbstractGnssReceiver](classQfAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
|  void | [**setSocketState**](classQfAbstractGnssReceiver.md#function-setsocketstate) (const QAbstractSocket::SocketState & state) <br> |








## Detailed Description


The [**QfFileReceiver**](classQfFileReceiver.md) replays a provided NMEA log file in a loop. Useful to debug external GNSS devices. 


    
## Public Static Attributes Documentation




### variable identifier 

```C++
QLatin1String QfFileReceiver::identifier;
```




<hr>
## Public Functions Documentation




### function QfFileReceiver 

```C++
explicit QfFileReceiver::QfFileReceiver (
    const QString & filePath=QString(),
    const int interval=0,
    QObject * parent=nullptr
) 
```




<hr>



### function ~QfFileReceiver 

```C++
QfFileReceiver::~QfFileReceiver () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/qffilereceiver.h`

