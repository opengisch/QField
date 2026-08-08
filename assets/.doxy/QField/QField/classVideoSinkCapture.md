

# Class VideoSinkCapture



[**ClassList**](annotated.md) **>** [**VideoSinkCapture**](classVideoSinkCapture.md)



[More...](#detailed-description)

* `#include <videosinkcapture.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QVideoSink \* | [**videoSink**](classVideoSinkCapture.md#property-videosink-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**imageSaved**](classVideoSinkCapture.md#signal-imagesaved) (const QString & path=QString()) <br> |
| signal void | [**videoSinkChanged**](classVideoSinkCapture.md#signal-videosinkchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**VideoSinkCapture**](#function-videosinkcapture) () = default<br> |
|  Q\_INVOKABLE void | [**saveToFile**](#function-savetofile) (const QString & location) <br>_Save the current frame from the video sink within a directory_ _location_ _._ |
|  void | [**setVideoSink**](#function-setvideosink) (QVideoSink \* videoSink) <br>_Sets the video sink from which still frames will be drawn from._  |
|  QVideoSink \* | [**videoSink**](#function-videosink-22) () const<br>_Returns the video sink from which still frames will be drawn from._  |




























## Detailed Description


This object allows capturing of still frames from a video sink. 


    
## Public Properties Documentation




### property videoSink [1/2]

```C++
QVideoSink * VideoSinkCapture::videoSink;
```




<hr>
## Public Signals Documentation




### signal imageSaved 

```C++
void VideoSinkCapture::imageSaved;
```




<hr>



### signal videoSinkChanged 

```C++
void VideoSinkCapture::videoSinkChanged;
```




<hr>
## Public Functions Documentation




### function VideoSinkCapture 

```C++
explicit VideoSinkCapture::VideoSinkCapture () = default
```




<hr>



### function saveToFile 

_Save the current frame from the video sink within a directory_ _location_ _._
```C++
Q_INVOKABLE void VideoSinkCapture::saveToFile (
    const QString & location
) 
```




<hr>



### function setVideoSink 

_Sets the video sink from which still frames will be drawn from._ 
```C++
void VideoSinkCapture::setVideoSink (
    QVideoSink * videoSink
) 
```




<hr>



### function videoSink [2/2]

_Returns the video sink from which still frames will be drawn from._ 
```C++
inline QVideoSink * VideoSinkCapture::videoSink () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/videosinkcapture.h`

