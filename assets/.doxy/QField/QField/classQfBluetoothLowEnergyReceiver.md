

# Class QfBluetoothLowEnergyReceiver



[**ClassList**](annotated.md) **>** [**QfBluetoothLowEnergyReceiver**](classQfBluetoothLowEnergyReceiver.md)



[More...](#detailed-description)

* `#include <qfbluetoothlowenergyreceiver.h>`



Inherits the following classes: [QfNmeaGnssReceiver](classQfNmeaGnssReceiver.md)


















## Public Types inherited from QfAbstractGnssReceiver

See [QfAbstractGnssReceiver](classQfAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
| enum  | [**Capability**](classQfAbstractGnssReceiver.md#enum-capability)  <br> |














## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  QLatin1String | [**identifier**](#variable-identifier)   = `QLatin1String( "ble" )`<br> |
|  QMap&lt; QBluetoothUuid, std::pair&lt; QBluetoothUuid, QBluetoothUuid &gt; &gt; | [**serviceChars**](#variable-servicechars)   = `/* multi line expression */`<br> |






## Public Slots

| Type | Name |
| ---: | :--- |
| slot void | [**onCorrectionDataReceived**](classQfBluetoothLowEnergyReceiver.md#slot-oncorrectiondatareceived) (const QByteArray & data) <br> |
| slot QString | [**socketStateString**](classQfBluetoothLowEnergyReceiver.md#slot-socketstatestring)  <br> |


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
|   | [**QfBluetoothLowEnergyReceiver**](#function-qfbluetoothlowenergyreceiver) (const QString & address=QString(), QObject \* parent=nullptr) <br> |
| virtual QfAbstractGnssReceiver::Capabilities | [**capabilities**](#function-capabilities) () override const<br> |
|   | [**~QfBluetoothLowEnergyReceiver**](#function-qfbluetoothlowenergyreceiver) () <br> |


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


The [**QfBluetoothLowEnergyReceiver**](classQfBluetoothLowEnergyReceiver.md) connects to a BLE device and feeds the QgsNmeaConnection over a proxy QIODevice using QLowEnergyController. It receives QgsGpsInformation and converts it to [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) 


    
## Public Static Attributes Documentation




### variable identifier 

```C++
QLatin1String QfBluetoothLowEnergyReceiver::identifier;
```




<hr>



### variable serviceChars 

```C++
QMap< QBluetoothUuid, std::pair< QBluetoothUuid, QBluetoothUuid > > QfBluetoothLowEnergyReceiver::serviceChars;
```




<hr>
## Public Slots Documentation




### slot onCorrectionDataReceived 

```C++
void QfBluetoothLowEnergyReceiver::onCorrectionDataReceived;
```




<hr>



### slot socketStateString 

```C++
QString QfBluetoothLowEnergyReceiver::socketStateString;
```




<hr>
## Public Functions Documentation




### function QfBluetoothLowEnergyReceiver 

```C++
explicit QfBluetoothLowEnergyReceiver::QfBluetoothLowEnergyReceiver (
    const QString & address=QString(),
    QObject * parent=nullptr
) 
```




<hr>



### function capabilities 

```C++
virtual QfAbstractGnssReceiver::Capabilities QfBluetoothLowEnergyReceiver::capabilities () override const
```



Implements [*QfAbstractGnssReceiver::capabilities*](classQfAbstractGnssReceiver.md#function-capabilities)


<hr>



### function ~QfBluetoothLowEnergyReceiver 

```C++
QfBluetoothLowEnergyReceiver::~QfBluetoothLowEnergyReceiver () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/qfbluetoothlowenergyreceiver.h`

