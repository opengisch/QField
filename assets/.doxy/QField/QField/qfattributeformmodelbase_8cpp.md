

# File qfattributeformmodelbase.cpp



[**FileList**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfattributeformmodelbase.cpp**](qfattributeformmodelbase_8cpp.md)

[Go to the source code of this file](qfattributeformmodelbase_8cpp_source.md)



* `#include "qfattributeformmodel.h"`
* `#include "qfattributeformmodelbase.h"`
* `#include <QDirIterator>`
* `#include <QRegularExpression>`
* `#include <qgsattributeeditorelement.h>`
* `#include <qgsattributeeditorfield.h>`
* `#include <qgsattributeeditorhtmlelement.h>`
* `#include <qgsattributeeditorqmlelement.h>`
* `#include <qgsattributeeditorrelation.h>`
* `#include <qgsattributeeditorspacerelement.h>`
* `#include <qgsattributeeditortextelement.h>`
* `#include <qgsdatetimefieldformatter.h>`
* `#include <qgseditorwidgetsetup.h>`
* `#include <qgsexpressioncontextutils.h>`
* `#include <qgsmapthemecollection.h>`
* `#include <qgsproject.h>`
* `#include <qgsrelationmanager.h>`
* `#include <qgsvaluerelationfieldformatter.h>`
* `#include <qgsvectorlayer.h>`
* `#include <qgsvectorlayerutils.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Q\_GLOBAL\_STATIC**](#function-q_global_static) (QStringList, sSupportedEditorWidgets) <br> |
|  void | [**\_checkChildrenValidity**](#function-_checkchildrenvalidity) (const QStandardItem \* parent, bool & hardValidity, bool & softValidity) <br> |




























## Public Functions Documentation




### function Q\_GLOBAL\_STATIC 

```C++
Q_GLOBAL_STATIC (
    QStringList,
    sSupportedEditorWidgets
) 
```




<hr>



### function \_checkChildrenValidity 

```C++
void _checkChildrenValidity (
    const QStandardItem * parent,
    bool & hardValidity,
    bool & softValidity
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfattributeformmodelbase.cpp`

