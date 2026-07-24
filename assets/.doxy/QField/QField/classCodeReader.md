

# Class CodeReader



[**ClassList**](annotated.md) **>** [**CodeReader**](classCodeReader.md)








Inherits the following classes: [Theme::QfPopup](classTheme_1_1QfPopup.md)






































## Public Properties

| Type | Name |
| ---: | :--- |
| property var | [**barcodeRequestedItem**](classCodeReader.md#property-barcoderequesteditem)  <br> |
| property string | [**decodedString**](classCodeReader.md#property-decodedstring)  <br> |
| property var | [**imageResourceSource**](classCodeReader.md#property-imageresourcesource)  <br> |
| property bool | [**openedOnce**](classCodeReader.md#property-openedonce)  <br> |
| property int | [**popupWidth**](classCodeReader.md#property-popupwidth)  <br> |








## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**accepted**](classCodeReader.md#signal-accepted) (var string) <br>_Emitted when a QR code or NFC tag has been accepted._  |
| signal void | [**decoded**](classCodeReader.md#signal-decoded) (var string) <br>_Emitted when a QR code or NFC tag has been decoded/received._  |








## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**decodeImageResource**](#function-decodeimageresource) (path) <br> |
|  void | [**pickImage**](#function-pickimage) () <br> |
























































## Public Properties Documentation




### property barcodeRequestedItem 

```C++
var CodeReader::barcodeRequestedItem;
```




<hr>



### property decodedString 

```C++
string CodeReader::decodedString;
```




<hr>



### property imageResourceSource 

```C++
var CodeReader::imageResourceSource;
```




<hr>



### property openedOnce 

```C++
bool CodeReader::openedOnce;
```




<hr>



### property popupWidth 

```C++
int CodeReader::popupWidth;
```




<hr>
## Public Signals Documentation




### signal accepted 

_Emitted when a QR code or NFC tag has been accepted._ 
```C++
void CodeReader::accepted;
```




<hr>



### signal decoded 

_Emitted when a QR code or NFC tag has been decoded/received._ 
```C++
void CodeReader::decoded;
```




<hr>
## Public Functions Documentation




### function decodeImageResource 

```C++
void CodeReader::decodeImageResource (
    path
) 
```




<hr>



### function pickImage 

```C++
void CodeReader::pickImage () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/CodeReader.qml`

