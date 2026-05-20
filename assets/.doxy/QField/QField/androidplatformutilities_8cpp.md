

# File androidplatformutilities.cpp



[**FileList**](files.md) **>** [**android**](dir_4a4823380b8154b853061c3a6b746fec.md) **>** [**androidplatformutilities.cpp**](androidplatformutilities_8cpp.md)

[Go to the source code of this file](androidplatformutilities_8cpp_source.md)



* `#include "androidplatformutilities.h"`
* `#include "androidprojectsource.h"`
* `#include "androidresourcesource.h"`
* `#include "androidviewstatus.h"`
* `#include "appinterface.h"`
* `#include "fileutils.h"`
* `#include "qfield.h"`
* `#include "qfield_android.h"`
* `#include "qfieldcloudconnection.h"`
* `#include <QJniEnvironment>`
* `#include <QJniObject>`
* `#include <QtCore/private/qandroidextras_p.h>`
* `#include <QApplication>`
* `#include <QCoreApplication>`
* `#include <QDebug>`
* `#include <QDir>`
* `#include <QFile>`
* `#include <QFileInfo>`
* `#include <QMap>`
* `#include <QMimeDatabase>`
* `#include <QQmlApplicationEngine>`
* `#include <QQmlContext>`
* `#include <QScreen>`
* `#include <QSettings>`
* `#include <QStandardPaths>`
* `#include <QString>`
* `#include <QTimer>`
* `#include <qgsfileutils.h>`
* `#include <android/bitmap.h>`
* `#include <android/log.h>`
* `#include <jni.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  JNIEXPORT void JNICALL jobject jstring | [**action**](#variable-action)   = `/* multi line expression */`<br> |
|  const char \*const | [**applicationName**](#variable-applicationname)   = `"QField"`<br> |
|  JNIEXPORT void JNICALL jobject jstring | [**message**](#variable-message)   = `/* multi line expression */`<br> |
|  JNIEXPORT void JNICALL jobject | [**obj**](#variable-obj)  <br> |
|  JNIEXPORT void JNICALL jobject jstring | [**path**](#variable-path)   = `/* multi line expression */`<br> |
|  JNIEXPORT void JNICALL jobject int | [**volumeKeyCode**](#variable-volumekeycode)   = `/* multi line expression */`<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  JNIEXPORT void JNICALL | [**JNI\_FUNCTION\_NAME**](#function-jni_function_name) (APP\_PACKAGE\_JNI\_NAME, QFieldActivity, openProject) <br> |
|  JNIEXPORT void JNICALL | [**JNI\_FUNCTION\_NAME**](#function-jni_function_name) (APP\_PACKAGE\_JNI\_NAME, QFieldActivity, executeAction) <br> |
|  JNIEXPORT void JNICALL | [**JNI\_FUNCTION\_NAME**](#function-jni_function_name) (APP\_PACKAGE\_JNI\_NAME, QFieldActivity, openPath) <br> |
|  JNIEXPORT void JNICALL | [**JNI\_FUNCTION\_NAME**](#function-jni_function_name) (APP\_PACKAGE\_JNI\_NAME, QFieldActivity, volumeKeyDown) <br> |
|  JNIEXPORT void JNICALL | [**JNI\_FUNCTION\_NAME**](#function-jni_function_name) (APP\_PACKAGE\_JNI\_NAME, QFieldActivity, volumeKeyUp) <br> |
|  JNIEXPORT void JNICALL | [**JNI\_FUNCTION\_NAME**](#function-jni_function_name) (APP\_PACKAGE\_JNI\_NAME, QFieldActivity, resourceReceived) <br> |
|  JNIEXPORT void JNICALL | [**JNI\_FUNCTION\_NAME**](#function-jni_function_name) (APP\_PACKAGE\_JNI\_NAME, QFieldActivity, resourceOpened) <br> |
|  JNIEXPORT void JNICALL | [**JNI\_FUNCTION\_NAME**](#function-jni_function_name) (APP\_PACKAGE\_JNI\_NAME, QFieldActivity, resourceCanceled) <br> |
|  QJniObject | [**qtAndroidContext**](#function-qtandroidcontext) () <br> |
|  int | [**qtAndroidSkdVersion**](#function-qtandroidskdversion) () <br> |
|  void | [**runOnAndroidMainThread**](#function-runonandroidmainthread) (const std::function&lt; void()&gt; & runnable) <br> |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ANDROID\_VOLUME\_DOWN**](androidplatformutilities_8cpp.md#define-android_volume_down)  `25`<br> |
| define  | [**ANDROID\_VOLUME\_UP**](androidplatformutilities_8cpp.md#define-android_volume_up)  `24`<br> |
| define  | [**GLUE\_HELPER**](androidplatformutilities_8cpp.md#define-glue_helper) (u, v, w, x, y, z) `u##v##w##x##y##z`<br> |
| define  | [**JNI\_FUNCTION\_NAME**](androidplatformutilities_8cpp.md#define-jni_function_name) (package\_name, class\_name, function\_name) `GLUE\_HELPER( Java\_ch\_opengis\_, package\_name, \_, class\_name, \_, function\_name )`<br> |

## Public Attributes Documentation




### variable action 

```C++
JNIEXPORT void JNICALL jobject jstring action;
```




<hr>



### variable applicationName 

```C++
const char* const applicationName;
```




<hr>



### variable message 

```C++
JNIEXPORT void JNICALL jobject jstring message;
```




<hr>



### variable obj 

```C++
JNIEXPORT void JNICALL jobject obj;
```




<hr>



### variable path 

```C++
JNIEXPORT void JNICALL jobject jstring path;
```




<hr>



### variable volumeKeyCode 

```C++
JNIEXPORT void JNICALL jobject int volumeKeyCode;
```




<hr>
## Public Functions Documentation




### function JNI\_FUNCTION\_NAME 

```C++
JNIEXPORT void JNICALL JNI_FUNCTION_NAME (
    APP_PACKAGE_JNI_NAME,
    QFieldActivity,
    openProject
) 
```




<hr>



### function JNI\_FUNCTION\_NAME 

```C++
JNIEXPORT void JNICALL JNI_FUNCTION_NAME (
    APP_PACKAGE_JNI_NAME,
    QFieldActivity,
    executeAction
) 
```




<hr>



### function JNI\_FUNCTION\_NAME 

```C++
JNIEXPORT void JNICALL JNI_FUNCTION_NAME (
    APP_PACKAGE_JNI_NAME,
    QFieldActivity,
    openPath
) 
```




<hr>



### function JNI\_FUNCTION\_NAME 

```C++
JNIEXPORT void JNICALL JNI_FUNCTION_NAME (
    APP_PACKAGE_JNI_NAME,
    QFieldActivity,
    volumeKeyDown
) 
```




<hr>



### function JNI\_FUNCTION\_NAME 

```C++
JNIEXPORT void JNICALL JNI_FUNCTION_NAME (
    APP_PACKAGE_JNI_NAME,
    QFieldActivity,
    volumeKeyUp
) 
```




<hr>



### function JNI\_FUNCTION\_NAME 

```C++
JNIEXPORT void JNICALL JNI_FUNCTION_NAME (
    APP_PACKAGE_JNI_NAME,
    QFieldActivity,
    resourceReceived
) 
```




<hr>



### function JNI\_FUNCTION\_NAME 

```C++
JNIEXPORT void JNICALL JNI_FUNCTION_NAME (
    APP_PACKAGE_JNI_NAME,
    QFieldActivity,
    resourceOpened
) 
```




<hr>



### function JNI\_FUNCTION\_NAME 

```C++
JNIEXPORT void JNICALL JNI_FUNCTION_NAME (
    APP_PACKAGE_JNI_NAME,
    QFieldActivity,
    resourceCanceled
) 
```




<hr>



### function qtAndroidContext 

```C++
inline QJniObject qtAndroidContext () 
```




<hr>



### function qtAndroidSkdVersion 

```C++
inline int qtAndroidSkdVersion () 
```




<hr>



### function runOnAndroidMainThread 

```C++
inline void runOnAndroidMainThread (
    const std::function< void()> & runnable
) 
```




<hr>
## Macro Definition Documentation





### define ANDROID\_VOLUME\_DOWN 

```C++
#define ANDROID_VOLUME_DOWN `25`
```




<hr>



### define ANDROID\_VOLUME\_UP 

```C++
#define ANDROID_VOLUME_UP `24`
```




<hr>



### define GLUE\_HELPER 

```C++
#define GLUE_HELPER (
    u,
    v,
    w,
    x,
    y,
    z
) `u##v##w##x##y##z`
```




<hr>



### define JNI\_FUNCTION\_NAME 

```C++
#define JNI_FUNCTION_NAME (
    package_name,
    class_name,
    function_name
) `GLUE_HELPER( Java_ch_opengis_, package_name, _, class_name, _, function_name )`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/platforms/android/androidplatformutilities.cpp`

