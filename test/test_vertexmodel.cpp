

#include <QtTest>

#include "vertexmodel.h"


class TestVertexModel: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    { qDebug("called before everything else"); }
    void myFirstTest()
    { QVERIFY(1 == 1); }
    void mySecondTest()
    { QVERIFY(1 == 2); }
    void cleanupTestCase()
    { qDebug("called after myFirstTest and mySecondTest"); }

private:
    VertexModel mModel;
};

QTEST_MAIN(TestVertexModel)
#include "test_vertexmodel.moc"
