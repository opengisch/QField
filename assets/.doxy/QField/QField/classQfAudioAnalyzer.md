

# Class QfAudioAnalyzer



[**ClassList**](annotated.md) **>** [**QfAudioAnalyzer**](classQfAudioAnalyzer.md)



_This class analyzes the peaks of audio clips and generate a list of bars that can be used to visualize the audio's overall texture._ 

* `#include <qfaudioanalyzer.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property int | [**barCount**](classQfAudioAnalyzer.md#property-barcount-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**barCountChanged**](classQfAudioAnalyzer.md#signal-barcountchanged)  <br> |
| signal void | [**ready**](classQfAudioAnalyzer.md#signal-ready) (const QList&lt; qreal &gt; & bars) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfAudioAnalyzer**](#function-qfaudioanalyzer) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**analyze**](#function-analyze) (const QUrl & source) <br> |
|  int | [**barCount**](#function-barcount-22) () const<br> |
|  void | [**setBarCount**](#function-setbarcount) (int barCount) <br> |




























## Public Properties Documentation




### property barCount [1/2]

```C++
int QfAudioAnalyzer::barCount;
```




<hr>
## Public Signals Documentation




### signal barCountChanged 

```C++
void QfAudioAnalyzer::barCountChanged;
```



Emitted when the bar count property has changed. 


        

<hr>



### signal ready 

```C++
void QfAudioAnalyzer::ready;
```



Emitted when an analysis is over. 

**Note:**

If the analysis failed, the list will be empty. 





        

<hr>
## Public Functions Documentation




### function QfAudioAnalyzer 

```C++
explicit QfAudioAnalyzer::QfAudioAnalyzer (
    QObject * parent=nullptr
) 
```




<hr>



### function analyze 

```C++
Q_INVOKABLE void QfAudioAnalyzer::analyze (
    const QUrl & source
) 
```



Run an analysis of the audio _source_. 


        

<hr>



### function barCount [2/2]

```C++
inline int QfAudioAnalyzer::barCount () const
```



Returns the bar count that will be returned upon successful audio clip analysis. 


        

<hr>



### function setBarCount 

```C++
void QfAudioAnalyzer::setBarCount (
    int barCount
) 
```



Sets the bar count that will be returned upon successful audio clip analysis. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfaudioanalyzer.h`

