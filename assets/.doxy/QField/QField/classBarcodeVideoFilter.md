

# Class BarcodeVideoFilter



[**ClassList**](annotated.md) **>** [**BarcodeVideoFilter**](classBarcodeVideoFilter.md)








Inherits the following classes: QAbstractVideoFilter


























## Public Properties

| Type | Name |
| ---: | :--- |
| property [**BarcodeDecoder**](classBarcodeDecoder.md) \* | [**decoder**](classBarcodeVideoFilter.md#property-decoder-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**decoderChanged**](classBarcodeVideoFilter.md#signal-decoderchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BarcodeVideoFilter**](#function-barcodevideofilter) () <br> |
|  QVideoFilterRunnable \* | [**createFilterRunnable**](#function-createfilterrunnable) () override<br> |
|  void | [**decodeVideoFrame**](#function-decodevideoframe) (QVideoFrame \* input) <br> |
|  [**BarcodeDecoder**](classBarcodeDecoder.md) \* | [**decoder**](#function-decoder-22) () const<br> |
|  bool | [**isDecoding**](#function-isdecoding) () const<br> |
|  void | [**setDecoder**](#function-setdecoder) ([**BarcodeDecoder**](classBarcodeDecoder.md) \* decoder) <br> |




























## Public Properties Documentation




### property decoder [1/2]

```C++
BarcodeDecoder * BarcodeVideoFilter::decoder;
```




<hr>
## Public Signals Documentation




### signal decoderChanged 

```C++
void BarcodeVideoFilter::decoderChanged;
```




<hr>
## Public Functions Documentation




### function BarcodeVideoFilter 

```C++
explicit BarcodeVideoFilter::BarcodeVideoFilter () 
```




<hr>



### function createFilterRunnable 

```C++
QVideoFilterRunnable * BarcodeVideoFilter::createFilterRunnable () override
```




<hr>



### function decodeVideoFrame 

```C++
void BarcodeVideoFilter::decodeVideoFrame (
    QVideoFrame * input
) 
```



Handle the decoding of a video frame barcodes. The frame will be analyzed for the presence of a barcode, and if so the decoder will capture the decoded string. 


        

<hr>



### function decoder [2/2]

```C++
inline BarcodeDecoder * BarcodeVideoFilter::decoder () const
```



Returns the barcode decoder to be used when analyzing video frames. 


        

<hr>



### function isDecoding 

```C++
inline bool BarcodeVideoFilter::isDecoding () const
```



Returns whether a video frame decoding is happening, which will dictate whether incoming video frames will be skipped or not. 


        

<hr>



### function setDecoder 

```C++
void BarcodeVideoFilter::setDecoder (
    BarcodeDecoder * decoder
) 
```



Sets the _decoder_ to be used when analyzing video frames. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/barcodevideofilter.h`

