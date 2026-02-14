

# Class InternalGnssReceiver



[**ClassList**](annotated.md) **>** [**InternalGnssReceiver**](classInternalGnssReceiver.md)








Inherits the following classes: [AbstractGnssReceiver](classAbstractGnssReceiver.md)
















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
|   | [**InternalGnssReceiver**](#function-internalgnssreceiver) (QObject \* parent=nullptr) <br> |
|   | [**~InternalGnssReceiver**](#function-internalgnssreceiver) () override<br> |


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
















































## Protected Functions inherited from AbstractGnssReceiver

See [AbstractGnssReceiver](classAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
|  void | [**setSocketState**](classAbstractGnssReceiver.md#function-setsocketstate) (const QAbstractSocket::SocketState & state) <br> |






## Public Functions Documentation




### function InternalGnssReceiver 

```C++
explicit InternalGnssReceiver::InternalGnssReceiver (
    QObject * parent=nullptr
) 
```




<hr>



### function ~InternalGnssReceiver 

```C++
InternalGnssReceiver::~InternalGnssReceiver () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/internalgnssreceiver.h`

