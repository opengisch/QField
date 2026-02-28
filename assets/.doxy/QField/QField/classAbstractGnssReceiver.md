

# Class AbstractGnssReceiver



[**ClassList**](annotated.md) **>** [**AbstractGnssReceiver**](classAbstractGnssReceiver.md)








Inherits the following classes: QObject


Inherited by the following classes: [EgenioussReceiver](classEgenioussReceiver.md),  [InternalGnssReceiver](classInternalGnssReceiver.md),  [NmeaGnssReceiver](classNmeaGnssReceiver.md)












## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Capability**](#enum-capability)  <br> |
















## Public Signals

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
|   | [**AbstractGnssReceiver**](#function-abstractgnssreceiver) (QObject \* parent=nullptr) <br> |
|  void | [**connectDevice**](#function-connectdevice) () <br> |
| virtual [**GnssPositionDetails**](classGnssPositionDetails.md) | [**details**](#function-details) () const<br> |
|  void | [**disconnectDevice**](#function-disconnectdevice) () <br> |
|  QString | [**lastError**](#function-lasterror) () const<br> |
|  [**GnssPositionInformation**](classGnssPositionInformation.md) | [**lastGnssPositionInformation**](#function-lastgnsspositioninformation) () const<br> |
|  void | [**setValid**](#function-setvalid) (bool valid) <br> |
| virtual QAbstractSocket::SocketState | [**socketState**](#function-socketstate) () const<br> |
| virtual QString | [**socketStateString**](#function-socketstatestring) () <br> |
|  void | [**startLogging**](#function-startlogging) (const QString & path) <br> |
|  void | [**stopLogging**](#function-stoplogging) () <br> |
|  bool | [**valid**](#function-valid) () const<br> |
| virtual  | [**~AbstractGnssReceiver**](#function-abstractgnssreceiver) () = default<br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  void | [**setSocketState**](#function-setsocketstate) (const QAbstractSocket::SocketState & state) <br> |




## Public Types Documentation




### enum Capability 

```C++
enum AbstractGnssReceiver::Capability {
    NoCapabilities = 0,
    OrthometricAltitude = 1,
    Logging = 1 << 1
};
```




<hr>
## Public Signals Documentation




### signal lastErrorChanged 

```C++
void AbstractGnssReceiver::lastErrorChanged;
```




<hr>



### signal lastGnssPositionInformationChanged 

```C++
void AbstractGnssReceiver::lastGnssPositionInformationChanged;
```




<hr>



### signal socketStateChanged 

```C++
void AbstractGnssReceiver::socketStateChanged;
```




<hr>



### signal socketStateStringChanged 

```C++
void AbstractGnssReceiver::socketStateStringChanged;
```




<hr>



### signal validChanged 

```C++
void AbstractGnssReceiver::validChanged;
```




<hr>
## Public Functions Documentation




### function AbstractGnssReceiver 

```C++
explicit AbstractGnssReceiver::AbstractGnssReceiver (
    QObject * parent=nullptr
) 
```




<hr>



### function connectDevice 

```C++
inline void AbstractGnssReceiver::connectDevice () 
```




<hr>



### function details 

```C++
inline virtual GnssPositionDetails AbstractGnssReceiver::details () const
```



Returns extra details (such as hdop, vdop, pdop) provided by the positioning device. 


        

<hr>



### function disconnectDevice 

```C++
inline void AbstractGnssReceiver::disconnectDevice () 
```




<hr>



### function lastError 

```C++
inline QString AbstractGnssReceiver::lastError () const
```




<hr>



### function lastGnssPositionInformation 

```C++
inline GnssPositionInformation AbstractGnssReceiver::lastGnssPositionInformation () const
```




<hr>



### function setValid 

```C++
inline void AbstractGnssReceiver::setValid (
    bool valid
) 
```




<hr>



### function socketState 

```C++
inline virtual QAbstractSocket::SocketState AbstractGnssReceiver::socketState () const
```




<hr>



### function socketStateString 

```C++
virtual QString AbstractGnssReceiver::socketStateString () 
```




<hr>



### function startLogging 

```C++
inline void AbstractGnssReceiver::startLogging (
    const QString & path
) 
```




<hr>



### function stopLogging 

```C++
inline void AbstractGnssReceiver::stopLogging () 
```




<hr>



### function valid 

```C++
inline bool AbstractGnssReceiver::valid () const
```




<hr>



### function ~AbstractGnssReceiver 

```C++
virtual AbstractGnssReceiver::~AbstractGnssReceiver () = default
```




<hr>
## Protected Functions Documentation




### function setSocketState 

```C++
void AbstractGnssReceiver::setSocketState (
    const QAbstractSocket::SocketState & state
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/abstractgnssreceiver.h`

