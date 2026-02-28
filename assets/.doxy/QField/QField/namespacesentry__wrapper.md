

# Namespace sentry\_wrapper



[**Namespace List**](namespaces.md) **>** [**sentry\_wrapper**](namespacesentry__wrapper.md)




























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  QtMessageHandler | [**originalMessageHandler**](#variable-originalmessagehandler)   = `nullptr`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**capture\_event**](#function-capture_event) (const char \* message, const char \* cloudUser) <br> |
|  void | [**close**](#function-close) () <br> |
|  void | [**init**](#function-init) () <br> |
|  void | [**install\_message\_handler**](#function-install_message_handler) () <br> |
|  void | [**qfMessageHandler**](#function-qfmessagehandler) (QtMsgType type, const QMessageLogContext & context, const QString & msg) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  const char \* | [**logLevelForMessageType**](#function-loglevelformessagetype) (QtMsgType msgType) <br> |


























## Public Static Attributes Documentation




### variable originalMessageHandler 

```C++
QtMessageHandler sentry_wrapper::originalMessageHandler;
```




<hr>
## Public Functions Documentation




### function capture\_event 

```C++
void sentry_wrapper::capture_event (
    const char * message,
    const char * cloudUser
) 
```




<hr>



### function close 

```C++
void sentry_wrapper::close () 
```




<hr>



### function init 

```C++
void sentry_wrapper::init () 
```




<hr>



### function install\_message\_handler 

```C++
void sentry_wrapper::install_message_handler () 
```




<hr>



### function qfMessageHandler 

```C++
void sentry_wrapper::qfMessageHandler (
    QtMsgType type,
    const QMessageLogContext & context,
    const QString & msg
) 
```




<hr>
## Public Static Functions Documentation




### function logLevelForMessageType 

```C++
static const char * sentry_wrapper::logLevelForMessageType (
    QtMsgType msgType
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/sentry/sentry_classic.cpp`

