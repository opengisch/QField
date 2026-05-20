

# Class NtripSettings



[**ClassList**](annotated.md) **>** [**NtripSettings**](classNtripSettings.md)



_A class containing NTRIP server settings._ 

* `#include <ntripsettings.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Protocol**](#enum-protocol)  <br>_NTRIP server protocol._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**forwardNmeaSentences**](classNtripSettings.md#property-forwardnmeasentences-12)  <br> |
| property QString | [**host**](classNtripSettings.md#property-host-12)  <br> |
| property bool | [**isValid**](classNtripSettings.md#property-isvalid-12)  <br> |
| property QString | [**mountPoint**](classNtripSettings.md#property-mountpoint-12)  <br> |
| property QString | [**password**](classNtripSettings.md#property-password-12)  <br> |
| property int | [**port**](classNtripSettings.md#property-port-12)  <br> |
| property [**Protocol**](classNtripSettings.md#enum-protocol) | [**protocol**](classNtripSettings.md#property-protocol-12)  <br> |
| property QString | [**username**](classNtripSettings.md#property-username-12)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**NtripSettings**](#function-ntripsettings) () <br>_The NTRIP settings constructor._  |
|  bool | [**forwardNmeaSentences**](#function-forwardnmeasentences-22) () const<br>_Returns TRUE if NMEA sentences will be forwarded to the NTRIP server._  |
|  QString | [**host**](#function-host-22) () const<br>_Returns the NTRIP server host._  |
|  bool | [**isValid**](#function-isvalid-22) () const<br>_Returns TRUE if the settings are complete enough to connect to an NTRIP server._  |
|  QString | [**mountPoint**](#function-mountpoint-22) () const<br>_Returns the NTRIP server mount point._  |
|  bool | [**operator!=**](#function-operator) (const [**NtripSettings**](classNtripSettings.md) & other) const<br> |
|  bool | [**operator==**](#function-operator_1) (const [**NtripSettings**](classNtripSettings.md) & other) const<br> |
|  QString | [**password**](#function-password-22) () const<br>_Returns the password to connect ot he NTRIP server._  |
|  int | [**port**](#function-port-22) () const<br>_Returns the NTRIP server port._  |
|  [**Protocol**](classNtripSettings.md#enum-protocol) | [**protocol**](#function-protocol-22) () const<br>_Returns the NTRIP server protocol._  |
|  void | [**setForwardNmeaSentences**](#function-setforwardnmeasentences) (bool forwardNmeaSentences) <br>_Sets whether NMEA sentences will be forwarded to the NTRIP server._  |
|  void | [**setHost**](#function-sethost) (const QString & host) <br>_Sets the NTRIP server host._  |
|  void | [**setMountPoint**](#function-setmountpoint) (const QString & mountPoint) <br>_Sets the NTRIP server mount point._  |
|  void | [**setPassword**](#function-setpassword) (const QString & password) <br>_Sets the password to connect ot he NTRIP server._  |
|  void | [**setPort**](#function-setport) (int port) <br>_Sets the NTRIP server port._  |
|  void | [**setProtocol**](#function-setprotocol) ([**Protocol**](classNtripSettings.md#enum-protocol) protocol) <br>_Sets the NTRIP server protocol._  |
|  void | [**setUsername**](#function-setusername) (const QString & username) <br>_Sets the username to connect ot he NTRIP server._  |
|  Q\_INVOKABLE QVariantMap | [**toMap**](#function-tomap) () const<br>_Return a map of settings._  |
|  QString | [**username**](#function-username-22) () const<br>_Returns the username to connect ot he NTRIP server._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**NtripSettings**](classNtripSettings.md) | [**fromMap**](#function-frommap) (const QVariantMap & settings) <br>_Creates an_ [_**NtripSettings**_](classNtripSettings.md) _object from a map of settings._ |


























## Public Types Documentation




### enum Protocol 

_NTRIP server protocol._ 
```C++
enum NtripSettings::Protocol {
    NtripVersion1,
    NtripVersion2
};
```




<hr>
## Public Properties Documentation




### property forwardNmeaSentences [1/2]

```C++
bool NtripSettings::forwardNmeaSentences;
```




<hr>



### property host [1/2]

```C++
QString NtripSettings::host;
```




<hr>



### property isValid [1/2]

```C++
bool NtripSettings::isValid;
```




<hr>



### property mountPoint [1/2]

```C++
QString NtripSettings::mountPoint;
```




<hr>



### property password [1/2]

```C++
QString NtripSettings::password;
```




<hr>



### property port [1/2]

```C++
int NtripSettings::port;
```




<hr>



### property protocol [1/2]

```C++
Protocol NtripSettings::protocol;
```




<hr>



### property username [1/2]

```C++
QString NtripSettings::username;
```




<hr>
## Public Functions Documentation




### function NtripSettings 

_The NTRIP settings constructor._ 
```C++
NtripSettings::NtripSettings () 
```




<hr>



### function forwardNmeaSentences [2/2]

_Returns TRUE if NMEA sentences will be forwarded to the NTRIP server._ 
```C++
inline bool NtripSettings::forwardNmeaSentences () const
```




<hr>



### function host [2/2]

_Returns the NTRIP server host._ 
```C++
inline QString NtripSettings::host () const
```




<hr>



### function isValid [2/2]

_Returns TRUE if the settings are complete enough to connect to an NTRIP server._ 
```C++
bool NtripSettings::isValid () const
```




<hr>



### function mountPoint [2/2]

_Returns the NTRIP server mount point._ 
```C++
inline QString NtripSettings::mountPoint () const
```




<hr>



### function operator!= 

```C++
inline bool NtripSettings::operator!= (
    const NtripSettings & other
) const
```




<hr>



### function operator== 

```C++
bool NtripSettings::operator== (
    const NtripSettings & other
) const
```




<hr>



### function password [2/2]

_Returns the password to connect ot he NTRIP server._ 
```C++
inline QString NtripSettings::password () const
```




<hr>



### function port [2/2]

_Returns the NTRIP server port._ 
```C++
inline int NtripSettings::port () const
```




<hr>



### function protocol [2/2]

_Returns the NTRIP server protocol._ 
```C++
inline Protocol NtripSettings::protocol () const
```




<hr>



### function setForwardNmeaSentences 

_Sets whether NMEA sentences will be forwarded to the NTRIP server._ 
```C++
void NtripSettings::setForwardNmeaSentences (
    bool forwardNmeaSentences
) 
```




<hr>



### function setHost 

_Sets the NTRIP server host._ 
```C++
void NtripSettings::setHost (
    const QString & host
) 
```




<hr>



### function setMountPoint 

_Sets the NTRIP server mount point._ 
```C++
void NtripSettings::setMountPoint (
    const QString & mountPoint
) 
```




<hr>



### function setPassword 

_Sets the password to connect ot he NTRIP server._ 
```C++
void NtripSettings::setPassword (
    const QString & password
) 
```




<hr>



### function setPort 

_Sets the NTRIP server port._ 
```C++
void NtripSettings::setPort (
    int port
) 
```




<hr>



### function setProtocol 

_Sets the NTRIP server protocol._ 
```C++
void NtripSettings::setProtocol (
    Protocol protocol
) 
```




<hr>



### function setUsername 

_Sets the username to connect ot he NTRIP server._ 
```C++
void NtripSettings::setUsername (
    const QString & username
) 
```




<hr>



### function toMap 

_Return a map of settings._ 
```C++
Q_INVOKABLE QVariantMap NtripSettings::toMap () const
```




<hr>



### function username [2/2]

_Returns the username to connect ot he NTRIP server._ 
```C++
inline QString NtripSettings::username () const
```




<hr>
## Public Static Functions Documentation




### function fromMap 

_Creates an_ [_**NtripSettings**_](classNtripSettings.md) _object from a map of settings._
```C++
static NtripSettings NtripSettings::fromMap (
    const QVariantMap & settings
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/ntripsettings.h`

