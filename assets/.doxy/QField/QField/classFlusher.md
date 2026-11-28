

# Class Flusher



[**ClassList**](annotated.md) **>** [**Flusher**](classFlusher.md)








Inherits the following classes: QObject






















## Public Slots

| Type | Name |
| ---: | :--- |
| slot void | [**flush**](classFlusher.md#slot-flush) (const QString & filename) <br> |
| slot bool | [**isStopped**](classFlusher.md#slot-isstopped) (const QString & fileName) <br> |
| slot void | [**scheduleFlush**](classFlusher.md#slot-scheduleflush) (const QString & filename) <br> |
| slot void | [**start**](classFlusher.md#slot-start) (const QString & fileName) <br> |
| slot void | [**stop**](classFlusher.md#slot-stop) (const QString & fileName) <br> |








































## Public Slots Documentation




### slot flush 

```C++
void Flusher::flush;
```



Flushes the contents of the given _filename_. 


        

<hr>



### slot isStopped 

```C++
bool Flusher::isStopped;
```



Returns whether the flusher is stopped for a given _fileName_. 


        

<hr>



### slot scheduleFlush 

```C++
void Flusher::scheduleFlush;
```



Schedules a new flush for the given _filename_ after 500ms. If a new flush is scheduled for the same file before the actual flush is performed, the timer is reset to wait another 500ms. 


        

<hr>



### slot start 

```C++
void Flusher::start;
```



Reenables scheduling flushes for a given _fileName_. 


        

<hr>



### slot stop 

```C++
void Flusher::stop;
```



Immediately performs a flush for a given _fileName_ and returns. If the flusher is stopped, flush for that _fileName_ would be ignored. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qgsgpkgflusher.cpp`

