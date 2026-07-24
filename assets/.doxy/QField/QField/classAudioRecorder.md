

# Class AudioRecorder



[**ClassList**](annotated.md) **>** [**AudioRecorder**](classAudioRecorder.md)








Inherits the following classes: QMediaRecorder


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**hasLevel**](classAudioRecorder.md#property-haslevel-12)  <br> |
| property double | [**level**](classAudioRecorder.md#property-level-12)  <br> |
| property bool | [**recording**](classAudioRecorder.md#property-recording-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**hasLevelChanged**](classAudioRecorder.md#signal-haslevelchanged)  <br> |
| signal void | [**levelChanged**](classAudioRecorder.md#signal-levelchanged)  <br> |
| signal void | [**recordingChanged**](classAudioRecorder.md#signal-recordingchanged)  <br> |
| signal void | [**recordingLoaded**](classAudioRecorder.md#signal-recordingloaded)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**AudioRecorder**](#function-audiorecorder) (QObject \* parent=nullptr) <br> |
|  bool | [**hasLevel**](#function-haslevel-22) () const<br> |
|  double | [**level**](#function-level-22) () const<br> |
|  bool | [**recording**](#function-recording-22) () const<br> |




























## Public Properties Documentation




### property hasLevel [1/2]

```C++
bool AudioRecorder::hasLevel;
```




<hr>



### property level [1/2]

```C++
double AudioRecorder::level;
```




<hr>



### property recording [1/2]

```C++
bool AudioRecorder::recording;
```




<hr>
## Public Signals Documentation




### signal hasLevelChanged 

```C++
void AudioRecorder::hasLevelChanged;
```



Emitted upon successfully setting up the audio probe used to return audio levels. 


        

<hr>



### signal levelChanged 

```C++
void AudioRecorder::levelChanged;
```



Emitted when the current audio level being recorded has changed. 


        

<hr>



### signal recordingChanged 

```C++
void AudioRecorder::recordingChanged;
```



Emitted when the recording state has changed. 


        

<hr>



### signal recordingLoaded 

```C++
void AudioRecorder::recordingLoaded;
```



Emitted when a recorded audio file is finalized and loaded. 


        

<hr>
## Public Functions Documentation




### function AudioRecorder 

```C++
explicit AudioRecorder::AudioRecorder (
    QObject * parent=nullptr
) 
```




<hr>



### function hasLevel [2/2]

```C++
bool AudioRecorder::hasLevel () const
```



Returns TRUE when audio level/amplitude is available during recording. 

**Note:**

this reflects whether a given platform supports QAudioProbe 




**See also:** level() 



        

<hr>



### function level [2/2]

```C++
double AudioRecorder::level () const
```



Returns the current level (0.0 to 1.0) of the audio being recorded. 

**See also:** hasLevel() 



        

<hr>



### function recording [2/2]

```C++
bool AudioRecorder::recording () const
```



Returns TRUE when audio is being recorded. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/audiorecorder.h`

