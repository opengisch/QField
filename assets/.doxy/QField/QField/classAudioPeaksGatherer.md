

# Class AudioPeaksGatherer



[**ClassList**](annotated.md) **>** [**AudioPeaksGatherer**](classAudioPeaksGatherer.md)








Inherits the following classes: QThread






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**collectedRawPeaks**](classAudioPeaksGatherer.md#signal-collectedrawpeaks)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**AudioPeaksGatherer**](#function-audiopeaksgatherer) (const QUrl & source) <br> |
|  QList&lt; float &gt; | [**rawPeaks**](#function-rawpeaks) () const<br> |
|  void | [**run**](#function-run) () override<br> |
|  void | [**stop**](#function-stop) () <br> |




























## Public Signals Documentation




### signal collectedRawPeaks 

```C++
void AudioPeaksGatherer::collectedRawPeaks;
```




<hr>
## Public Functions Documentation




### function AudioPeaksGatherer 

```C++
AudioPeaksGatherer::AudioPeaksGatherer (
    const QUrl & source
) 
```




<hr>



### function rawPeaks 

```C++
inline QList< float > AudioPeaksGatherer::rawPeaks () const
```




<hr>



### function run 

```C++
void AudioPeaksGatherer::run () override
```




<hr>



### function stop 

```C++
void AudioPeaksGatherer::stop () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/audioanalyzer.h`

