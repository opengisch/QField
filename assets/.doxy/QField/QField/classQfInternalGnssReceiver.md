

# Class QfInternalGnssReceiver



[**ClassList**](annotated.md) **>** [**QfInternalGnssReceiver**](classQfInternalGnssReceiver.md)








Inherits the following classes: [QfAbstractGnssReceiver](classQfAbstractGnssReceiver.md)
















## Public Types inherited from QfAbstractGnssReceiver

See [QfAbstractGnssReceiver](classQfAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
| enum  | [**Capability**](classQfAbstractGnssReceiver.md#enum-capability)  <br> |
















## Public Slots inherited from QfAbstractGnssReceiver

See [QfAbstractGnssReceiver](classQfAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
| slot void | [**onCorrectionDataReceived**](classQfAbstractGnssReceiver.md#slot-oncorrectiondatareceived) (const QByteArray & data) <br> |
















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
|   | [**QfInternalGnssReceiver**](#function-qfinternalgnssreceiver) (QObject \* parent=nullptr) <br> |
|   | [**~QfInternalGnssReceiver**](#function-qfinternalgnssreceiver) () <br> |


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
















































## Protected Functions inherited from QfAbstractGnssReceiver

See [QfAbstractGnssReceiver](classQfAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
|  void | [**setSocketState**](classQfAbstractGnssReceiver.md#function-setsocketstate) (const QAbstractSocket::SocketState & state) <br> |






## Public Functions Documentation




### function QfInternalGnssReceiver 

```C++
explicit QfInternalGnssReceiver::QfInternalGnssReceiver (
    QObject * parent=nullptr
) 
```




<hr>



### function ~QfInternalGnssReceiver 

```C++
QfInternalGnssReceiver::~QfInternalGnssReceiver () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/qfinternalgnssreceiver.h`

