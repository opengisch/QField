

# Class QfAudioProbe



[**ClassList**](annotated.md) **>** [**QfAudioProbe**](classQfAudioProbe.md)








Inherits the following classes: QIODevice






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**levelCalculated**](classQfAudioProbe.md#signal-levelcalculated) (double level) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfAudioProbe**](#function-qfaudioprobe) (QObject \* parent=nullptr) <br> |
|  qint64 | [**readData**](#function-readdata) (char \* data, qint64 maxlen) override<br> |
|  qint64 | [**writeData**](#function-writedata) (const char \* data, qint64 len) override<br> |




























## Public Signals Documentation




### signal levelCalculated 

```C++
void QfAudioProbe::levelCalculated;
```




<hr>
## Public Functions Documentation




### function QfAudioProbe 

```C++
explicit QfAudioProbe::QfAudioProbe (
    QObject * parent=nullptr
) 
```




<hr>



### function readData 

```C++
qint64 QfAudioProbe::readData (
    char * data,
    qint64 maxlen
) override
```




<hr>



### function writeData 

```C++
qint64 QfAudioProbe::writeData (
    const char * data,
    qint64 len
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfaudiorecorder.h`

