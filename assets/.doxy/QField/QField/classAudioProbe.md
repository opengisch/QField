

# Class AudioProbe



[**ClassList**](annotated.md) **>** [**AudioProbe**](classAudioProbe.md)








Inherits the following classes: QIODevice






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**levelCalculated**](classAudioProbe.md#signal-levelcalculated) (double level) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**AudioProbe**](#function-audioprobe) (QObject \* parent=nullptr) <br> |
|  qint64 | [**readData**](#function-readdata) (char \* data, qint64 maxlen) override<br> |
|  qint64 | [**writeData**](#function-writedata) (const char \* data, qint64 len) override<br> |




























## Public Signals Documentation




### signal levelCalculated 

```C++
void AudioProbe::levelCalculated;
```




<hr>
## Public Functions Documentation




### function AudioProbe 

```C++
explicit AudioProbe::AudioProbe (
    QObject * parent=nullptr
) 
```




<hr>



### function readData 

```C++
qint64 AudioProbe::readData (
    char * data,
    qint64 maxlen
) override
```




<hr>



### function writeData 

```C++
qint64 AudioProbe::writeData (
    const char * data,
    qint64 len
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/audiorecorder.h`

