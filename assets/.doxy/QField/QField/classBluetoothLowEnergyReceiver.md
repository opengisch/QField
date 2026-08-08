

# Class BluetoothLowEnergyReceiver



[**ClassList**](annotated.md) **>** [**BluetoothLowEnergyReceiver**](classBluetoothLowEnergyReceiver.md)



[More...](#detailed-description)

* `#include <bluetoothlowenergyreceiver.h>`



Inherits the following classes: [NmeaGnssReceiver](classNmeaGnssReceiver.md)


















## Public Types inherited from AbstractGnssReceiver

See [AbstractGnssReceiver](classAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
| enum  | [**Capability**](classAbstractGnssReceiver.md#enum-capability)  <br> |














## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  QLatin1String | [**identifier**](#variable-identifier)   = `QLatin1String( "ble" )`<br> |
|  QMap&lt; QBluetoothUuid, std::pair&lt; QBluetoothUuid, QBluetoothUuid &gt; &gt; | [**serviceChars**](#variable-servicechars)   = `/* multi line expression */`<br> |






## Public Slots

| Type | Name |
| ---: | :--- |
| slot void | [**onCorrectionDataReceived**](classBluetoothLowEnergyReceiver.md#slot-oncorrectiondatareceived) (const QByteArray & data) <br> |
| slot QString | [**socketStateString**](classBluetoothLowEnergyReceiver.md#slot-socketstatestring)  <br> |


## Public Slots inherited from NmeaGnssReceiver

See [NmeaGnssReceiver](classNmeaGnssReceiver.md)

| Type | Name |
| ---: | :--- |
| slot void | [**onCorrectionDataReceived**](classNmeaGnssReceiver.md#slot-oncorrectiondatareceived) (const QByteArray & data) <br> |


## Public Slots inherited from AbstractGnssReceiver

See [AbstractGnssReceiver](classAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
| slot void | [**onCorrectionDataReceived**](classAbstractGnssReceiver.md#slot-oncorrectiondatareceived) (const QByteArray & data) <br> |






















## Public Signals inherited from NmeaGnssReceiver

See [NmeaGnssReceiver](classNmeaGnssReceiver.md)

| Type | Name |
| ---: | :--- |
| signal void | [**nmeaSentenceReceived**](classNmeaGnssReceiver.md#signal-nmeasentencereceived) (const QString & sentence) <br> |


## Public Signals inherited from AbstractGnssReceiver

See [AbstractGnssReceiver](classAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
| signal void | [**batteryLevelChanged**](classAbstractGnssReceiver.md#signal-batterylevelchanged) (const double batteryLevel) <br> |
| signal void | [**lastErrorChanged**](classAbstractGnssReceiver.md#signal-lasterrorchanged) (const QString & lastError) <br> |
| signal void | [**lastGnssPositionInformationChanged**](classAbstractGnssReceiver.md#signal-lastgnsspositioninformationchanged) (const [**GnssPositionInformation**](classGnssPositionInformation.md) & lastGnssPositionInformation) <br> |
| signal void | [**socketStateChanged**](classAbstractGnssReceiver.md#signal-socketstatechanged) (const QAbstractSocket::SocketState socketState) <br> |
| signal void | [**socketStateStringChanged**](classAbstractGnssReceiver.md#signal-socketstatestringchanged) (const QString & socketStateString) <br> |
| signal void | [**validChanged**](classAbstractGnssReceiver.md#signal-validchanged)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BluetoothLowEnergyReceiver**](#function-bluetoothlowenergyreceiver) (const QString & address=QString(), QObject \* parent=nullptr) <br> |
| virtual AbstractGnssReceiver::Capabilities | [**capabilities**](#function-capabilities) () override const<br> |
|   | [**~BluetoothLowEnergyReceiver**](#function-bluetoothlowenergyreceiver) () <br> |


## Public Functions inherited from NmeaGnssReceiver

See [NmeaGnssReceiver](classNmeaGnssReceiver.md)

| Type | Name |
| ---: | :--- |
|   | [**NmeaGnssReceiver**](classNmeaGnssReceiver.md#function-nmeagnssreceiver) (QObject \* parent=nullptr) <br> |
| virtual AbstractGnssReceiver::Capabilities | [**capabilities**](classNmeaGnssReceiver.md#function-capabilities) () override const<br> |
|  void | [**initNmeaConnection**](classNmeaGnssReceiver.md#function-initnmeaconnection) (QIODevice \* ioDevice) <br> |
|   | [**~NmeaGnssReceiver**](classNmeaGnssReceiver.md#function-nmeagnssreceiver) () override<br> |


## Public Functions inherited from AbstractGnssReceiver

See [AbstractGnssReceiver](classAbstractGnssReceiver.md)

| Type | Name |
| ---: | :--- |
|   | [**AbstractGnssReceiver**](classAbstractGnssReceiver.md#function-abstractgnssreceiver) (QObject \* parent=nullptr) <br> |
|  double | [**batteryLevel**](classAbstractGnssReceiver.md#function-batterylevel) () const<br> |
| virtual AbstractGnssReceiver::Capabilities | [**capabilities**](classAbstractGnssReceiver.md#function-capabilities) () const<br> |
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


The [**BluetoothLowEnergyReceiver**](classBluetoothLowEnergyReceiver.md) connects to a BLE device and feeds the QgsNmeaConnection over a proxy QIODevice using QLowEnergyController. It receives QgsGpsInformation and converts it to [**GnssPositionInformation**](classGnssPositionInformation.md) 


    
## Public Static Attributes Documentation




### variable identifier 

```C++
QLatin1String BluetoothLowEnergyReceiver::identifier;
```




<hr>



### variable serviceChars 

```C++
QMap< QBluetoothUuid, std::pair< QBluetoothUuid, QBluetoothUuid > > BluetoothLowEnergyReceiver::serviceChars;
```




<hr>
## Public Slots Documentation




### slot onCorrectionDataReceived 

```C++
void BluetoothLowEnergyReceiver::onCorrectionDataReceived;
```




<hr>



### slot socketStateString 

```C++
QString BluetoothLowEnergyReceiver::socketStateString;
```




<hr>
## Public Functions Documentation




### function BluetoothLowEnergyReceiver 

```C++
explicit BluetoothLowEnergyReceiver::BluetoothLowEnergyReceiver (
    const QString & address=QString(),
    QObject * parent=nullptr
) 
```




<hr>



### function capabilities 

```C++
virtual AbstractGnssReceiver::Capabilities BluetoothLowEnergyReceiver::capabilities () override const
```



Implements [*AbstractGnssReceiver::capabilities*](classAbstractGnssReceiver.md#function-capabilities)


<hr>



### function ~BluetoothLowEnergyReceiver 

```C++
BluetoothLowEnergyReceiver::~BluetoothLowEnergyReceiver () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/bluetoothlowenergyreceiver.h`

