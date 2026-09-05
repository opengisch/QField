

# Class QfAudioPeaksGatherer



[**ClassList**](annotated.md) **>** [**QfAudioPeaksGatherer**](classQfAudioPeaksGatherer.md)








Inherits the following classes: QThread






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**collectedRawPeaks**](classQfAudioPeaksGatherer.md#signal-collectedrawpeaks)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfAudioPeaksGatherer**](#function-qfaudiopeaksgatherer) (const QUrl & source) <br> |
|  QList&lt; float &gt; | [**rawPeaks**](#function-rawpeaks) () const<br> |
|  void | [**run**](#function-run) () override<br> |
|  void | [**stop**](#function-stop) () <br> |




























## Public Signals Documentation




### signal collectedRawPeaks 

```C++
void QfAudioPeaksGatherer::collectedRawPeaks;
```




<hr>
## Public Functions Documentation




### function QfAudioPeaksGatherer 

```C++
QfAudioPeaksGatherer::QfAudioPeaksGatherer (
    const QUrl & source
) 
```




<hr>



### function rawPeaks 

```C++
inline QList< float > QfAudioPeaksGatherer::rawPeaks () const
```




<hr>



### function run 

```C++
void QfAudioPeaksGatherer::run () override
```




<hr>



### function stop 

```C++
void QfAudioPeaksGatherer::stop () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfaudioanalyzer.h`

