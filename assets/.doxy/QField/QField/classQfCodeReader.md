

# Class QfCodeReader



[**ClassList**](annotated.md) **>** [**QfCodeReader**](classQfCodeReader.md)








Inherits the following classes: [QfPopup](classQfPopup.md)


Inherited by the following classes: [CodeReader](classCodeReader.md)




































## Public Properties

| Type | Name |
| ---: | :--- |
| property var | [**barcodeRequestedItem**](classQfCodeReader.md#property-barcoderequesteditem)  <br> |
| property string | [**decodedString**](classQfCodeReader.md#property-decodedstring)  <br> |
| property var | [**imageResourceSource**](classQfCodeReader.md#property-imageresourcesource)  <br> |
| property bool | [**openedOnce**](classQfCodeReader.md#property-openedonce)  <br> |
| property int | [**popupWidth**](classQfCodeReader.md#property-popupwidth)  <br> |








## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**accepted**](classQfCodeReader.md#signal-accepted) (var string) <br>_Emitted when a QR code or NFC tag has been accepted._  |
| signal void | [**decoded**](classQfCodeReader.md#signal-decoded) (var string) <br>_Emitted when a QR code or NFC tag has been decoded/received._  |








## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**decodeImageResource**](#function-decodeimageresource) (path) <br> |
|  void | [**pickImage**](#function-pickimage) () <br> |
























































## Public Properties Documentation




### property barcodeRequestedItem 

```C++
var QfCodeReader::barcodeRequestedItem;
```




<hr>



### property decodedString 

```C++
string QfCodeReader::decodedString;
```




<hr>



### property imageResourceSource 

```C++
var QfCodeReader::imageResourceSource;
```




<hr>



### property openedOnce 

```C++
bool QfCodeReader::openedOnce;
```




<hr>



### property popupWidth 

```C++
int QfCodeReader::popupWidth;
```




<hr>
## Public Signals Documentation




### signal accepted 

_Emitted when a QR code or NFC tag has been accepted._ 
```C++
void QfCodeReader::accepted;
```




<hr>



### signal decoded 

_Emitted when a QR code or NFC tag has been decoded/received._ 
```C++
void QfCodeReader::decoded;
```




<hr>
## Public Functions Documentation




### function decodeImageResource 

```C++
void QfCodeReader::decodeImageResource (
    path
) 
```




<hr>



### function pickImage 

```C++
void QfCodeReader::pickImage () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/QfCodeReader.qml`

