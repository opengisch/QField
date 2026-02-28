

# Class CodeReader



[**ClassList**](annotated.md) **>** [**CodeReader**](classCodeReader.md)








Inherits the following classes: [Theme::QfPopup](classTheme_1_1QfPopup.md)






































## Public Properties

| Type | Name |
| ---: | :--- |
| property var | [**barcodeRequestedItem**](classCodeReader.md#property-barcoderequesteditem)  <br> |
| property string | [**decodedString**](classCodeReader.md#property-decodedstring)  <br> |
| property bool | [**openedOnce**](classCodeReader.md#property-openedonce)  <br> |
| property int | [**popupWidth**](classCodeReader.md#property-popupwidth)  <br> |








## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**accepted**](classCodeReader.md#signal-accepted) (var string) <br>_Emitted when a QR code or NFC tag has been accepted._  |
| signal void | [**decoded**](classCodeReader.md#signal-decoded) (var string) <br>_Emitted when a QR code or NFC tag has been decoded/received._  |
































































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

------------------------------
The documentation for this class was generated from the following file `src/qml/CodeReader.qml`

