

# File attributeformmodelbase.cpp



[**FileList**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**attributeformmodelbase.cpp**](attributeformmodelbase_8cpp.md)

[Go to the source code of this file](attributeformmodelbase_8cpp_source.md)



* `#include "attributeformmodel.h"`
* `#include "attributeformmodelbase.h"`
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
The documentation for this class was generated from the following file `src/core/attributeformmodelbase.cpp`

