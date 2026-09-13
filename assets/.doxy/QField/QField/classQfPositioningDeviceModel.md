

# Class QfPositioningDeviceModel



[**ClassList**](annotated.md) **>** [**QfPositioningDeviceModel**](classQfPositioningDeviceModel.md)








Inherits the following classes: QAbstractListModel












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**Device**](structQfPositioningDeviceModel_1_1Device.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Role**](#enum-role)  <br> |
| enum  | [**Type**](#enum-type)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfPositioningDeviceModel**](#function-qfpositioningdevicemodel) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE int | [**addDevice**](#function-adddevice) (const Type & type, const QString & name, const QVariantMap & deviceSettings) <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  Q\_INVOKABLE const QString | [**deviceId**](#function-deviceid) (const [**Device**](structQfPositioningDeviceModel_1_1Device.md) & device) const<br> |
|  Q\_INVOKABLE int | [**findIndexFromDeviceId**](#function-findindexfromdeviceid) (const QString & id) <br> |
|  Q\_INVOKABLE void | [**reloadModel**](#function-reloadmodel) () <br> |
|  Q\_INVOKABLE void | [**removeDevice**](#function-removedevice) (const QString & name) <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |




























## Public Types Documentation




### enum Role 

```C++
enum QfPositioningDeviceModel::Role {
    DeviceType = Qt::UserRole,
    DeviceId,
    DeviceName,
    DeviceSettings
};
```




<hr>



### enum Type 

```C++
enum QfPositioningDeviceModel::Type {
    InternalDevice,
    BluetoothDevice,
    TcpDevice,
    UdpDevice,
    EgenioussDevice,
    SerialPortDevice,
    FileDevice
};
```




<hr>
## Public Functions Documentation




### function QfPositioningDeviceModel 

```C++
explicit QfPositioningDeviceModel::QfPositioningDeviceModel (
    QObject * parent=nullptr
) 
```




<hr>



### function addDevice 

```C++
Q_INVOKABLE int QfPositioningDeviceModel::addDevice (
    const Type & type,
    const QString & name,
    const QVariantMap & deviceSettings
) 
```



Adds a positioning device to the user settings 

**Parameters:**


* `type` device type 
* `name` friendly device name used as identifier in the user interface 
* `deviceSettings` settings map (used to generate the positioning device ID, editing, etc.) 



**Returns:**

returns index of the added device 





        

<hr>



### function data 

```C++
QVariant QfPositioningDeviceModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function deviceId 

```C++
Q_INVOKABLE const QString QfPositioningDeviceModel::deviceId (
    const Device & device
) const
```



Returns the device ID string for a specific _device_ 

**Note:**

this is the string to be used with the [**QfPositioning**](classQfPositioning.md) deviceId property 





        

<hr>



### function findIndexFromDeviceId 

```C++
Q_INVOKABLE int QfPositioningDeviceModel::findIndexFromDeviceId (
    const QString & id
) 
```



Returns the row index for a given device ID 


        

<hr>



### function reloadModel 

```C++
Q_INVOKABLE void QfPositioningDeviceModel::reloadModel () 
```



Reloads the model from the user settings 


        

<hr>



### function removeDevice 

```C++
Q_INVOKABLE void QfPositioningDeviceModel::removeDevice (
    const QString & name
) 
```



Removes the positioning device _name_ from the user settings 


        

<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfPositioningDeviceModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfPositioningDeviceModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/qfpositioningdevicemodel.h`

