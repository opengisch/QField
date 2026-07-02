

# Class BluetoothDeviceModel



[**ClassList**](annotated.md) **>** [**BluetoothDeviceModel**](classBluetoothDeviceModel.md)



[More...](#detailed-description)

* `#include <bluetoothdevicemodel.h>`



Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**BluetoothDeviceRoles**](#enum-bluetoothdeviceroles)  <br>_The roles provided by this model._  |
| enum  | [**ScanningStatus**](#enum-scanningstatus)  <br>_The status telling the result of the scanning._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property qsizetype | [**lastDiscoveredCount**](classBluetoothDeviceModel.md#property-lastdiscoveredcount-12)  <br> |
| property QString | [**lastError**](classBluetoothDeviceModel.md#property-lasterror-12)  <br> |
| property [**ScanningStatus**](classBluetoothDeviceModel.md#enum-scanningstatus) | [**scanningStatus**](classBluetoothDeviceModel.md#property-scanningstatus-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**lastDiscoveredCountChanged**](classBluetoothDeviceModel.md#signal-lastdiscoveredcountchanged)  <br> |
| signal void | [**lastErrorChanged**](classBluetoothDeviceModel.md#signal-lasterrorchanged) (QString lastError) <br> |
| signal void | [**scanningStatusChanged**](classBluetoothDeviceModel.md#signal-scanningstatuschanged) ([**BluetoothDeviceModel::ScanningStatus**](classBluetoothDeviceModel.md#enum-scanningstatus) scanningStatus) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BluetoothDeviceModel**](#function-bluetoothdevicemodel) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE int | [**addDevice**](#function-adddevice) (const QString & name, const QString & address) <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role=Qt::DisplayRole) override const<br> |
|  QString | [**deviceAddress**](#function-deviceaddress) (const QBluetoothDeviceInfo & info) const<br> |
|  Q\_INVOKABLE int | [**findIndexFromAddress**](#function-findindexfromaddress) (const QString & address) const<br> |
|  qsizetype | [**lastDiscoveredCount**](#function-lastdiscoveredcount-22) () const<br> |
|  QString | [**lastError**](#function-lasterror-22) () const<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  [**ScanningStatus**](classBluetoothDeviceModel.md#enum-scanningstatus) | [**scanningStatus**](#function-scanningstatus-22) () const<br> |
|  Q\_INVOKABLE void | [**startDeviceDiscovery**](#function-startdevicediscovery) () <br> |
|  Q\_INVOKABLE void | [**stopDeviceDiscovery**](#function-stopdevicediscovery) () <br> |
|   | [**~BluetoothDeviceModel**](#function-bluetoothdevicemodel) () <br> |




























## Detailed Description


A model that provides all paired bluetooth devices name/address that are accessible over the serial port. 


    
## Public Types Documentation




### enum BluetoothDeviceRoles 

_The roles provided by this model._ 
```C++
enum BluetoothDeviceModel::BluetoothDeviceRoles {
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
enum BluetoothDeviceModel::ScanningStatus {
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
qsizetype BluetoothDeviceModel::lastDiscoveredCount;
```




<hr>



### property lastError [1/2]

```C++
QString BluetoothDeviceModel::lastError;
```




<hr>



### property scanningStatus [1/2]

```C++
ScanningStatus BluetoothDeviceModel::scanningStatus;
```




<hr>
## Public Signals Documentation




### signal lastDiscoveredCountChanged 

```C++
void BluetoothDeviceModel::lastDiscoveredCountChanged;
```




<hr>



### signal lastErrorChanged 

```C++
void BluetoothDeviceModel::lastErrorChanged;
```




<hr>



### signal scanningStatusChanged 

```C++
void BluetoothDeviceModel::scanningStatusChanged;
```




<hr>
## Public Functions Documentation




### function BluetoothDeviceModel 

```C++
explicit BluetoothDeviceModel::BluetoothDeviceModel (
    QObject * parent=nullptr
) 
```




<hr>



### function addDevice 

```C++
Q_INVOKABLE int BluetoothDeviceModel::addDevice (
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
QVariant BluetoothDeviceModel::data (
    const QModelIndex & index,
    int role=Qt::DisplayRole
) override const
```




<hr>



### function deviceAddress 

```C++
QString BluetoothDeviceModel::deviceAddress (
    const QBluetoothDeviceInfo & info
) const
```




<hr>



### function findIndexFromAddress 

```C++
Q_INVOKABLE int BluetoothDeviceModel::findIndexFromAddress (
    const QString & address
) const
```



Returns the row index for a given Bluetooth device address 


        

<hr>



### function lastDiscoveredCount [2/2]

```C++
inline qsizetype BluetoothDeviceModel::lastDiscoveredCount () const
```




<hr>



### function lastError [2/2]

```C++
inline QString BluetoothDeviceModel::lastError () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > BluetoothDeviceModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int BluetoothDeviceModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function scanningStatus [2/2]

```C++
inline ScanningStatus BluetoothDeviceModel::scanningStatus () const
```




<hr>



### function startDeviceDiscovery 

```C++
Q_INVOKABLE void BluetoothDeviceModel::startDeviceDiscovery () 
```



Starts a scan to discover nearby Bluetooth devices, sequentially going through a fast scan then a full, deeper scan for devices. 


        

<hr>



### function stopDeviceDiscovery 

```C++
Q_INVOKABLE void BluetoothDeviceModel::stopDeviceDiscovery () 
```



Stops any ongoing scan to discover nearby Bluetooth devices. 


        

<hr>



### function ~BluetoothDeviceModel 

```C++
BluetoothDeviceModel::~BluetoothDeviceModel () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/bluetoothdevicemodel.h`

