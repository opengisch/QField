
#include <QtTest>
#include <qgsapplication.h>
#include <qgslinestring.h>
#include <qgsgeometry.h>
#include <qgspoint.h>
#include <qgspointxy.h>

#include "qgsquickmapsettings.h"
#include "vertexmodel.h"

#ifdef ANDROID
#include "androidplatformutilities.h"
#endif

class TestReferencingFeatureListModel: public QObject
{
    Q_OBJECT
  private slots:
    void initTestCase()
    {
    }
};

QTEST_MAIN( TestReferencingFeatureListModel )
#include "test_referencingfeaturelistmodel.moc"
