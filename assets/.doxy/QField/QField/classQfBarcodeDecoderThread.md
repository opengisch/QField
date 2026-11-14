

# Class QfBarcodeDecoderThread



[**ClassList**](annotated.md) **>** [**QfBarcodeDecoderThread**](classQfBarcodeDecoderThread.md)








Inherits the following classes: QThread,  QThread


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfBarcodeDecoderThread**](#function-qfbarcodedecoderthread-12) (const QImage & image, const QTransform & displayTransform) <br> |
|   | [**QfBarcodeDecoderThread**](#function-qfbarcodedecoderthread-22) ([**QfBarcodeDecoder**](classQfBarcodeDecoder.md) \* decoder, const QImage & image) <br> |
|  QList&lt; [**QfBarcodeVideoModel::Barcode**](structQfBarcodeVideoModel_1_1Barcode.md) &gt; | [**barcodes**](#function-barcodes) () const<br> |




























## Public Functions Documentation




### function QfBarcodeDecoderThread [1/2]

```C++
inline explicit QfBarcodeDecoderThread::QfBarcodeDecoderThread (
    const QImage & image,
    const QTransform & displayTransform
) 
```




<hr>



### function QfBarcodeDecoderThread [2/2]

```C++
inline explicit QfBarcodeDecoderThread::QfBarcodeDecoderThread (
    QfBarcodeDecoder * decoder,
    const QImage & image
) 
```




<hr>



### function barcodes 

```C++
inline QList< QfBarcodeVideoModel::Barcode > QfBarcodeDecoderThread::barcodes () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfbarcodedecoder.cpp`

