

# Class QfBarcodeDecoder



[**ClassList**](annotated.md) **>** [**QfBarcodeDecoder**](classQfBarcodeDecoder.md)








Inherits the following classes: QObject






















## Public Slots

| Type | Name |
| ---: | :--- |
| slot void | [**decodeVideoFrame**](classQfBarcodeDecoder.md#slot-decodevideoframe) (const QVideoFrame & frame) <br> |




## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**decodedString**](classQfBarcodeDecoder.md#property-decodedstring-12)  <br> |
| property QVideoSink \* | [**videoSink**](classQfBarcodeDecoder.md#property-videosink-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**decodedStringChanged**](classQfBarcodeDecoder.md#signal-decodedstringchanged)  <br> |
| signal void | [**videoSinkChanged**](classQfBarcodeDecoder.md#signal-videosinkchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfBarcodeDecoder**](#function-qfbarcodedecoder) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**clearDecodedString**](#function-cleardecodedstring) () <br> |
|  bool | [**decodeImage**](#function-decodeimage) (const QImage & image) <br> |
|  Q\_INVOKABLE bool | [**decodeImageFile**](#function-decodeimagefile) (const QString & path) <br> |
|  QString | [**decodedString**](#function-decodedstring-22) () const<br> |
|  void | [**setVideoSink**](#function-setvideosink) (QVideoSink \* sink) <br> |
|  QVideoSink \* | [**videoSink**](#function-videosink-22) () const<br> |




























## Public Properties Documentation




### property decodedString [1/2]

```C++
QString QfBarcodeDecoder::decodedString;
```




<hr>



### property videoSink [1/2]

```C++
QVideoSink * QfBarcodeDecoder::videoSink;
```




<hr>
## Public Slots Documentation




### slot decodeVideoFrame 

```C++
void QfBarcodeDecoder::decodeVideoFrame;
```




<hr>
## Public Signals Documentation




### signal decodedStringChanged 

```C++
void QfBarcodeDecoder::decodedStringChanged;
```




<hr>



### signal videoSinkChanged 

```C++
void QfBarcodeDecoder::videoSinkChanged;
```




<hr>
## Public Functions Documentation




### function QfBarcodeDecoder 

```C++
explicit QfBarcodeDecoder::QfBarcodeDecoder (
    QObject * parent=nullptr
) 
```




<hr>



### function clearDecodedString 

```C++
Q_INVOKABLE void QfBarcodeDecoder::clearDecodedString () 
```



Clears the decoded string value and sets it to an empty string. 


        

<hr>



### function decodeImage 

```C++
bool QfBarcodeDecoder::decodeImage (
    const QImage & image
) 
```



Scans a provided _image_ for barcodes and if present sets the decoded string value. Returns TRUE when a barcode was decoded. 


        

<hr>



### function decodeImageFile 

```C++
Q_INVOKABLE bool QfBarcodeDecoder::decodeImageFile (
    const QString & path
) 
```



Scans an image found at a given _path_ for barcodes and if present sets the decoded string value. Returns TRUE when a barcode was decoded. 


        

<hr>



### function decodedString [2/2]

```C++
inline QString QfBarcodeDecoder::decodedString () const
```



Returns the last barcode decoded string. 


        

<hr>



### function setVideoSink 

```C++
void QfBarcodeDecoder::setVideoSink (
    QVideoSink * sink
) 
```




<hr>



### function videoSink [2/2]

```C++
QVideoSink * QfBarcodeDecoder::videoSink () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfbarcodedecoder.h`

