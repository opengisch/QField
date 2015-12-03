#ifndef PLATFORMUTILITIES_H
#define PLATFORMUTILITIES_H

#include <QObject>
#include <QAndroidJniObject>


class PlatformUtilities: public QObject
{
    Q_OBJECT

    public:
        explicit PlatformUtilities( QObject *parent = 0 );
        QMap<QString, QString> getIntentExtras(QStringList);
        QString getIntentExtra(QString, QAndroidJniObject=0);
        QAndroidJniObject getNativeIntent();
        QAndroidJniObject getNativeExtras();
};
#endif // PLATFORMUTILITIES_H

