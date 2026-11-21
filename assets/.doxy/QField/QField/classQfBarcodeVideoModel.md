

# Class QfBarcodeVideoModel



[**ClassList**](annotated.md) **>** [**QfBarcodeVideoModel**](classQfBarcodeVideoModel.md)



[More...](#detailed-description)

* `#include <qfbarcodevideomodel.h>`



Inherits the following classes: QAbstractListModel












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**Barcode**](structQfBarcodeVideoModel_1_1Barcode.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Roles**](#enum-roles)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfBarcodeVideoModel**](#function-qfbarcodevideomodel) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**clear**](#function-clear) () <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  int | [**rowForText**](#function-rowfortext) (const QString & text) const<br> |
|  void | [**updateBarcodes**](#function-updatebarcodes) (const QList&lt; [**Barcode**](structQfBarcodeVideoModel_1_1Barcode.md) &gt; & barcodes) <br> |




























## Detailed Description


A list model of the barcodes seen in a video feed. 


    
## Public Types Documentation




### enum Roles 

```C++
enum QfBarcodeVideoModel::Roles {
    BarcodeText = Qt::UserRole + 1,
    BarcodeBounds
};
```




<hr>
## Public Functions Documentation




### function QfBarcodeVideoModel 

```C++
explicit QfBarcodeVideoModel::QfBarcodeVideoModel (
    QObject * parent=nullptr
) 
```




<hr>



### function clear 

```C++
Q_INVOKABLE void QfBarcodeVideoModel::clear () 
```



Removes all barcodes from the model. 


        

<hr>



### function data 

```C++
QVariant QfBarcodeVideoModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfBarcodeVideoModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfBarcodeVideoModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function rowForText 

```C++
int QfBarcodeVideoModel::rowForText (
    const QString & text
) const
```



Returns the row matching a given _text_. If the text is not found, the returned value will be -1. 


        

<hr>



### function updateBarcodes 

```C++
void QfBarcodeVideoModel::updateBarcodes (
    const QList< Barcode > & barcodes
) 
```



Updates the model with the _barcodes_ found in a video frame. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfbarcodevideomodel.h`

