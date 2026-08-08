

# Class BarcodeDecoder



[**ClassList**](annotated.md) **>** [**BarcodeDecoder**](classBarcodeDecoder.md)








Inherits the following classes: QObject






















## Public Slots

| Type | Name |
| ---: | :--- |
| slot void | [**decodeVideoFrame**](classBarcodeDecoder.md#slot-decodevideoframe) (const QVideoFrame & frame) <br> |




## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**decodedString**](classBarcodeDecoder.md#property-decodedstring-12)  <br> |
| property QVideoSink \* | [**videoSink**](classBarcodeDecoder.md#property-videosink-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**decodedStringChanged**](classBarcodeDecoder.md#signal-decodedstringchanged)  <br> |
| signal void | [**videoSinkChanged**](classBarcodeDecoder.md#signal-videosinkchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BarcodeDecoder**](#function-barcodedecoder) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**clearDecodedString**](#function-cleardecodedstring) () <br> |
|  bool | [**decodeImage**](#function-decodeimage) (const QImage & image) <br> |
|  Q\_INVOKABLE bool | [**decodeImageFile**](#function-decodeimagefile) (const QString & path) <br> |
|  QString | [**decodedString**](#function-decodedstring-22) () const<br> |
|  void | [**setVideoSink**](#function-setvideosink) (QVideoSink \* sink) <br> |
|  QVideoSink \* | [**videoSink**](#function-videosink-22) () const<br> |




























## Public Properties Documentation




### property decodedString [1/2]

```C++
QString BarcodeDecoder::decodedString;
```




<hr>



### property videoSink [1/2]

```C++
QVideoSink * BarcodeDecoder::videoSink;
```




<hr>
## Public Slots Documentation




### slot decodeVideoFrame 

```C++
void BarcodeDecoder::decodeVideoFrame;
```




<hr>
## Public Signals Documentation




### signal decodedStringChanged 

```C++
void BarcodeDecoder::decodedStringChanged;
```




<hr>



### signal videoSinkChanged 

```C++
void BarcodeDecoder::videoSinkChanged;
```




<hr>
## Public Functions Documentation




### function BarcodeDecoder 

```C++
explicit BarcodeDecoder::BarcodeDecoder (
    QObject * parent=nullptr
) 
```




<hr>



### function clearDecodedString 

```C++
Q_INVOKABLE void BarcodeDecoder::clearDecodedString () 
```



Clears the decoded string value and sets it to an empty string. 


        

<hr>



### function decodeImage 

```C++
bool BarcodeDecoder::decodeImage (
    const QImage & image
) 
```



Scans a provided _image_ for barcodes and if present sets the decoded string value. Returns TRUE when a barcode was decoded. 


        

<hr>



### function decodeImageFile 

```C++
Q_INVOKABLE bool BarcodeDecoder::decodeImageFile (
    const QString & path
) 
```



Scans an image found at a given _path_ for barcodes and if present sets the decoded string value. Returns TRUE when a barcode was decoded. 


        

<hr>



### function decodedString [2/2]

```C++
inline QString BarcodeDecoder::decodedString () const
```



Returns the last barcode decoded string. 


        

<hr>



### function setVideoSink 

```C++
void BarcodeDecoder::setVideoSink (
    QVideoSink * sink
) 
```




<hr>



### function videoSink [2/2]

```C++
QVideoSink * BarcodeDecoder::videoSink () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/barcodedecoder.h`

