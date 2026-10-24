

# Class QfBluetoothDeviceModel



[**ClassList**](annotated.md) **>** [**QfBluetoothDeviceModel**](classQfBluetoothDeviceModel.md)



[More...](#detailed-description)

* `#include <qfbluetoothdevicemodel.h>`



Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**BluetoothDeviceRoles**](#enum-bluetoothdeviceroles)  <br>_The roles provided by this model._  |
| enum  | [**ScanningStatus**](#enum-scanningstatus)  <br>_The status telling the result of the scanning._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property qsizetype | [**lastDiscoveredCount**](classQfBluetoothDeviceModel.md#property-lastdiscoveredcount-12)  <br> |
| property QString | [**lastError**](classQfBluetoothDeviceModel.md#property-lasterror-12)  <br> |
| property [**ScanningStatus**](classQfBluetoothDeviceModel.md#enum-scanningstatus) | [**scanningStatus**](classQfBluetoothDeviceModel.md#property-scanningstatus-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**lastDiscoveredCountChanged**](classQfBluetoothDeviceModel.md#signal-lastdiscoveredcountchanged)  <br> |
| signal void | [**lastErrorChanged**](classQfBluetoothDeviceModel.md#signal-lasterrorchanged) (QString lastError) <br> |
| signal void | [**scanningStatusChanged**](classQfBluetoothDeviceModel.md#signal-scanningstatuschanged) ([**QfBluetoothDeviceModel::ScanningStatus**](classQfBluetoothDeviceModel.md#enum-scanningstatus) scanningStatus) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfBluetoothDeviceModel**](#function-qfbluetoothdevicemodel) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE int | [**addDevice**](#function-adddevice) (const QString & name, const QString & address) <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role=Qt::DisplayRole) override const<br> |
|  QString | [**deviceAddress**](#function-deviceaddress) (const QBluetoothDeviceInfo & info) const<br> |
|  Q\_INVOKABLE int | [**findIndexFromAddress**](#function-findindexfromaddress) (const QString & address) const<br> |
|  qsizetype | [**lastDiscoveredCount**](#function-lastdiscoveredcount-22) () const<br> |
|  QString | [**lastError**](#function-lasterror-22) () const<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  [**ScanningStatus**](classQfBluetoothDeviceModel.md#enum-scanningstatus) | [**scanningStatus**](#function-scanningstatus-22) () const<br> |
|  Q\_INVOKABLE void | [**startDeviceDiscovery**](#function-startdevicediscovery) () <br> |
|  Q\_INVOKABLE void | [**stopDeviceDiscovery**](#function-stopdevicediscovery) () <br> |
|   | [**~QfBluetoothDeviceModel**](#function-qfbluetoothdevicemodel) () <br> |




























## Detailed Description


A model that provides all paired bluetooth devices name/address that are accessible over the serial port. 


    
## Public Types Documentation




### enum BluetoothDeviceRoles 

_The roles provided by this model._ 
```C++
enum QfBluetoothDeviceModel::BluetoothDeviceRoles {
    DeviceAddressRole = Qt::UserRole + 1,
    DeviceNameRole,
    DeviceClassicSupportRole,
    DeviceLowEnergySupportRole,
    DeviceLowEnergyByDefaultRole
};
```




<hr>



### enum ScanningStatus 

_The status telling the result of the scanning._ 
```C++
enum QfBluetoothDeviceModel::ScanningStatus {
    Discovering,
    Succeeded,
    Failed,
    Canceled,
    NoStatus
};
```




<hr>
## Public Properties Documentation




### property lastDiscoveredCount [1/2]

```C++
qsizetype QfBluetoothDeviceModel::lastDiscoveredCount;
```




<hr>



### property lastError [1/2]

```C++
QString QfBluetoothDeviceModel::lastError;
```




<hr>



### property scanningStatus [1/2]

```C++
ScanningStatus QfBluetoothDeviceModel::scanningStatus;
```




<hr>
## Public Signals Documentation




### signal lastDiscoveredCountChanged 

```C++
void QfBluetoothDeviceModel::lastDiscoveredCountChanged;
```




<hr>



### signal lastErrorChanged 

```C++
void QfBluetoothDeviceModel::lastErrorChanged;
```




<hr>



### signal scanningStatusChanged 

```C++
void QfBluetoothDeviceModel::scanningStatusChanged;
```




<hr>
## Public Functions Documentation




### function QfBluetoothDeviceModel 

```C++
explicit QfBluetoothDeviceModel::QfBluetoothDeviceModel (
    QObject * parent=nullptr
) 
```




<hr>



### function addDevice 

```C++
Q_INVOKABLE int QfBluetoothDeviceModel::addDevice (
    const QString & name,
    const QString & address
) 
```



Adds a Bluetooth device if not already in the model 

**Parameters:**


* `name` friendly device name used as identifier in the user interface 
* `address` Bluetooth address of the device 



**Returns:**

returns index of the Bluetooth device 





        

<hr>



### function data 

```C++
QVariant QfBluetoothDeviceModel::data (
    const QModelIndex & index,
    int role=Qt::DisplayRole
) override const
```




<hr>



### function deviceAddress 

```C++
QString QfBluetoothDeviceModel::deviceAddress (
    const QBluetoothDeviceInfo & info
) const
```




<hr>



### function findIndexFromAddress 

```C++
Q_INVOKABLE int QfBluetoothDeviceModel::findIndexFromAddress (
    const QString & address
) const
```



Returns the row index for a given Bluetooth device address 


        

<hr>



### function lastDiscoveredCount [2/2]

```C++
inline qsizetype QfBluetoothDeviceModel::lastDiscoveredCount () const
```




<hr>



### function lastError [2/2]

```C++
inline QString QfBluetoothDeviceModel::lastError () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfBluetoothDeviceModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfBluetoothDeviceModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function scanningStatus [2/2]

```C++
inline ScanningStatus QfBluetoothDeviceModel::scanningStatus () const
```




<hr>



### function startDeviceDiscovery 

```C++
Q_INVOKABLE void QfBluetoothDeviceModel::startDeviceDiscovery () 
```



Starts a scan to discover nearby Bluetooth devices, sequentially going through a fast scan then a full, deeper scan for devices. 


        

<hr>



### function stopDeviceDiscovery 

```C++
Q_INVOKABLE void QfBluetoothDeviceModel::stopDeviceDiscovery () 
```



Stops any ongoing scan to discover nearby Bluetooth devices. 


        

<hr>



### function ~QfBluetoothDeviceModel 

```C++
QfBluetoothDeviceModel::~QfBluetoothDeviceModel () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/qfbluetoothdevicemodel.h`

