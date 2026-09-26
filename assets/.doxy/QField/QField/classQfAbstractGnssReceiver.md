

# Class QfAbstractGnssReceiver



[**ClassList**](annotated.md) **>** [**QfAbstractGnssReceiver**](classQfAbstractGnssReceiver.md)








Inherits the following classes: QObject


Inherited by the following classes: [QfEgenioussReceiver](classQfEgenioussReceiver.md),  [QfInternalGnssReceiver](classQfInternalGnssReceiver.md),  [QfNmeaGnssReceiver](classQfNmeaGnssReceiver.md)












## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Capability**](#enum-capability)  <br> |








## Public Slots

| Type | Name |
| ---: | :--- |
| slot void | [**onCorrectionDataReceived**](classQfAbstractGnssReceiver.md#slot-oncorrectiondatareceived) (const QByteArray & data) <br> |








## Public Signals

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
|   | [**QfAbstractGnssReceiver**](#function-qfabstractgnssreceiver) (QObject \* parent=nullptr) <br> |
|  double | [**batteryLevel**](#function-batterylevel) () const<br> |
| virtual QfAbstractGnssReceiver::Capabilities | [**capabilities**](#function-capabilities) () const<br> |
|  void | [**connectDevice**](#function-connectdevice) () <br> |
| virtual [**QfGnssPositionDetails**](classQfGnssPositionDetails.md) | [**details**](#function-details) () const<br> |
|  void | [**disconnectDevice**](#function-disconnectdevice) () <br> |
|  QString | [**lastError**](#function-lasterror) () const<br> |
|  [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) | [**lastGnssPositionInformation**](#function-lastgnsspositioninformation) () const<br> |
|  void | [**setValid**](#function-setvalid) (bool valid) <br> |
| virtual QAbstractSocket::SocketState | [**socketState**](#function-socketstate) () const<br> |
| virtual QString | [**socketStateString**](#function-socketstatestring) () <br> |
|  void | [**startLogging**](#function-startlogging) (const QString & path) <br> |
|  void | [**stopLogging**](#function-stoplogging) () <br> |
|  bool | [**valid**](#function-valid) () const<br> |
| virtual  | [**~QfAbstractGnssReceiver**](#function-qfabstractgnssreceiver) () = default<br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  void | [**setSocketState**](#function-setsocketstate) (const QAbstractSocket::SocketState & state) <br> |




## Public Types Documentation




### enum Capability 

```C++
enum QfAbstractGnssReceiver::Capability {
    NoCapabilities = 0,
    OrthometricAltitude = 1,
    Logging = 1 << 1,
    NtripCorrection = 1 << 2
};
```




<hr>
## Public Slots Documentation




### slot onCorrectionDataReceived 

```C++
virtual void QfAbstractGnssReceiver::onCorrectionDataReceived;
```




<hr>
## Public Signals Documentation




### signal batteryLevelChanged 

```C++
void QfAbstractGnssReceiver::batteryLevelChanged;
```




<hr>



### signal lastErrorChanged 

```C++
void QfAbstractGnssReceiver::lastErrorChanged;
```




<hr>



### signal lastGnssPositionInformationChanged 

```C++
void QfAbstractGnssReceiver::lastGnssPositionInformationChanged;
```




<hr>



### signal socketStateChanged 

```C++
void QfAbstractGnssReceiver::socketStateChanged;
```




<hr>



### signal socketStateStringChanged 

```C++
void QfAbstractGnssReceiver::socketStateStringChanged;
```




<hr>



### signal validChanged 

```C++
void QfAbstractGnssReceiver::validChanged;
```




<hr>
## Public Functions Documentation




### function QfAbstractGnssReceiver 

```C++
explicit QfAbstractGnssReceiver::QfAbstractGnssReceiver (
    QObject * parent=nullptr
) 
```




<hr>



### function batteryLevel 

```C++
inline double QfAbstractGnssReceiver::batteryLevel () const
```




<hr>



### function capabilities 

```C++
virtual QfAbstractGnssReceiver::Capabilities QfAbstractGnssReceiver::capabilities () const
```




<hr>



### function connectDevice 

```C++
inline void QfAbstractGnssReceiver::connectDevice () 
```




<hr>



### function details 

```C++
inline virtual QfGnssPositionDetails QfAbstractGnssReceiver::details () const
```



Returns extra details (such as hdop, vdop, pdop) provided by the positioning device. 


        

<hr>



### function disconnectDevice 

```C++
inline void QfAbstractGnssReceiver::disconnectDevice () 
```




<hr>



### function lastError 

```C++
inline QString QfAbstractGnssReceiver::lastError () const
```




<hr>



### function lastGnssPositionInformation 

```C++
inline QfGnssPositionInformation QfAbstractGnssReceiver::lastGnssPositionInformation () const
```




<hr>



### function setValid 

```C++
inline void QfAbstractGnssReceiver::setValid (
    bool valid
) 
```




<hr>



### function socketState 

```C++
inline virtual QAbstractSocket::SocketState QfAbstractGnssReceiver::socketState () const
```




<hr>



### function socketStateString 

```C++
virtual QString QfAbstractGnssReceiver::socketStateString () 
```




<hr>



### function startLogging 

```C++
inline void QfAbstractGnssReceiver::startLogging (
    const QString & path
) 
```




<hr>



### function stopLogging 

```C++
inline void QfAbstractGnssReceiver::stopLogging () 
```




<hr>



### function valid 

```C++
inline bool QfAbstractGnssReceiver::valid () const
```




<hr>



### function ~QfAbstractGnssReceiver 

```C++
virtual QfAbstractGnssReceiver::~QfAbstractGnssReceiver () = default
```




<hr>
## Protected Functions Documentation




### function setSocketState 

```C++
void QfAbstractGnssReceiver::setSocketState (
    const QAbstractSocket::SocketState & state
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/qfabstractgnssreceiver.h`

