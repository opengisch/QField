

# Class QfBarcodeVideoFilter



[**ClassList**](annotated.md) **>** [**QfBarcodeVideoFilter**](classQfBarcodeVideoFilter.md)








Inherits the following classes: QAbstractVideoFilter


























## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfBarcodeDecoder**](classQfBarcodeDecoder.md) \* | [**decoder**](classQfBarcodeVideoFilter.md#property-decoder-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**decoderChanged**](classQfBarcodeVideoFilter.md#signal-decoderchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfBarcodeVideoFilter**](#function-qfbarcodevideofilter) () <br> |
|  QVideoFilterRunnable \* | [**createFilterRunnable**](#function-createfilterrunnable) () override<br> |
|  void | [**decodeVideoFrame**](#function-decodevideoframe) (QVideoFrame \* input) <br> |
|  [**QfBarcodeDecoder**](classQfBarcodeDecoder.md) \* | [**decoder**](#function-decoder-22) () const<br> |
|  bool | [**isDecoding**](#function-isdecoding) () const<br> |
|  void | [**setDecoder**](#function-setdecoder) ([**QfBarcodeDecoder**](classQfBarcodeDecoder.md) \* decoder) <br> |




























## Public Properties Documentation




### property decoder [1/2]

```C++
QfBarcodeDecoder * QfBarcodeVideoFilter::decoder;
```




<hr>
## Public Signals Documentation




### signal decoderChanged 

```C++
void QfBarcodeVideoFilter::decoderChanged;
```




<hr>
## Public Functions Documentation




### function QfBarcodeVideoFilter 

```C++
explicit QfBarcodeVideoFilter::QfBarcodeVideoFilter () 
```




<hr>



### function createFilterRunnable 

```C++
QVideoFilterRunnable * QfBarcodeVideoFilter::createFilterRunnable () override
```




<hr>



### function decodeVideoFrame 

```C++
void QfBarcodeVideoFilter::decodeVideoFrame (
    QVideoFrame * input
) 
```



Handle the decoding of a video frame barcodes. The frame will be analyzed for the presence of a barcode, and if so the decoder will capture the decoded string. 


        

<hr>



### function decoder [2/2]

```C++
inline QfBarcodeDecoder * QfBarcodeVideoFilter::decoder () const
```



Returns the barcode decoder to be used when analyzing video frames. 


        

<hr>



### function isDecoding 

```C++
inline bool QfBarcodeVideoFilter::isDecoding () const
```



Returns whether a video frame decoding is happening, which will dictate whether incoming video frames will be skipped or not. 


        

<hr>



### function setDecoder 

```C++
void QfBarcodeVideoFilter::setDecoder (
    QfBarcodeDecoder * decoder
) 
```



Sets the _decoder_ to be used when analyzing video frames. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfbarcodevideofilter.h`

