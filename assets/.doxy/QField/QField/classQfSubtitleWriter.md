

# Class QfSubtitleWriter



[**ClassList**](annotated.md) **>** [**QfSubtitleWriter**](classQfSubtitleWriter.md)



_Collects timed text and writes it out as a SubRip (.srt) subtitle file._ [More...](#detailed-description)

* `#include <qfsubtitlewriter.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property int | [**minimumCueDuration**](classQfSubtitleWriter.md#property-minimumcueduration-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**minimumCueDurationChanged**](classQfSubtitleWriter.md#signal-minimumcuedurationchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfSubtitleWriter**](#function-qfsubtitlewriter) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**addCue**](#function-addcue) (qint64 elapsedTime, const QString & text) <br> |
|  Q\_INVOKABLE void | [**clear**](#function-clear) () <br>_Drops every cue, returning the writer to its initial state._  |
|  int | [**minimumCueDuration**](#function-minimumcueduration-22) () const<br>_Returns the shortest duration in milliseconds an individual cue may have._  |
|  void | [**setMinimumCueDuration**](#function-setminimumcueduration) (int duration) <br>_Sets the shortest_ _duration_ _in milliseconds an individual cue may have._ |
|  Q\_INVOKABLE bool | [**write**](#function-write) (const QString & mediaFilePath, qint64 endTime) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  QString | [**formatTimestamp**](#function-formattimestamp) (qint64 milliseconds) <br>_Returns_ _milliseconds_ _as a SubRip timestamp, HH:MM:SS,zzz._ |
|  QString | [**sanitizeCueText**](#function-sanitizecuetext) (const QString & text) <br>_Returns_ _text_ _with line endings normalized and blank lines removed, as a blank line would end the cue._ |
|  QString | [**subtitleFilePath**](#function-subtitlefilepath) (const QString & mediaFilePath) <br>_Returns_ _mediaFilePath_ _with its suffix replaced by .srt._ |


























## Detailed Description


The writer holds a single open cue and closes it only when the text changes, so a caller can feed it every update without filtering out repetitions. Changes arriving faster than minimumCueDuration() are folded into the open cue instead of producing subtitles too short to read.


The caller owns the clock: it passes the elapsed time of every cue and the final duration to [**write()**](classQfSubtitleWriter.md#function-write), which keeps the cue timeline expressed against the media itself. 


    
## Public Properties Documentation




### property minimumCueDuration [1/2]

```C++
int QfSubtitleWriter::minimumCueDuration;
```




<hr>
## Public Signals Documentation




### signal minimumCueDurationChanged 

```C++
void QfSubtitleWriter::minimumCueDurationChanged;
```




<hr>
## Public Functions Documentation




### function QfSubtitleWriter 

```C++
explicit QfSubtitleWriter::QfSubtitleWriter (
    QObject * parent=nullptr
) 
```




<hr>



### function addCue 

```C++
Q_INVOKABLE void QfSubtitleWriter::addCue (
    qint64 elapsedTime,
    const QString & text
) 
```



Records _text_ as being valid from _elapsedTime_ milliseconds onwards.


Passing the text the open cue already holds does nothing, which is what stops an unchanging value from producing a stream of identical cues. An empty _text_ closes the open cue without starting a new one, leaving a gap in the track. 


        

<hr>



### function clear 

_Drops every cue, returning the writer to its initial state._ 
```C++
Q_INVOKABLE void QfSubtitleWriter::clear () 
```




<hr>



### function minimumCueDuration [2/2]

_Returns the shortest duration in milliseconds an individual cue may have._ 
```C++
inline int QfSubtitleWriter::minimumCueDuration () const
```




<hr>



### function setMinimumCueDuration 

_Sets the shortest_ _duration_ _in milliseconds an individual cue may have._
```C++
void QfSubtitleWriter::setMinimumCueDuration (
    int duration
) 
```




<hr>



### function write 

```C++
Q_INVOKABLE bool QfSubtitleWriter::write (
    const QString & mediaFilePath,
    qint64 endTime
) 
```



Writes the cues as a SubRip file next to _mediaFilePath_, closing the open cue at _endTime_ milliseconds. Returns FALSE and writes no file when there is nothing to write. 


        

<hr>
## Public Static Functions Documentation




### function formatTimestamp 

_Returns_ _milliseconds_ _as a SubRip timestamp, HH:MM:SS,zzz._
```C++
static QString QfSubtitleWriter::formatTimestamp (
    qint64 milliseconds
) 
```




<hr>



### function sanitizeCueText 

_Returns_ _text_ _with line endings normalized and blank lines removed, as a blank line would end the cue._
```C++
static QString QfSubtitleWriter::sanitizeCueText (
    const QString & text
) 
```




<hr>



### function subtitleFilePath 

_Returns_ _mediaFilePath_ _with its suffix replaced by .srt._
```C++
static QString QfSubtitleWriter::subtitleFilePath (
    const QString & mediaFilePath
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfsubtitlewriter.h`

