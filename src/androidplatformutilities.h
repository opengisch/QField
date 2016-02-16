#ifndef ANDROIDPLATFORMUTILITIES_H
#define ANDROIDPLATFORMUTILITIES_H

#include "platformutilities.h"
#include <QAndroidJniObject>

class AndroidPlatformUtilities : public PlatformUtilities
{
  public:
    AndroidPlatformUtilities();

    virtual QString configDir() const;
    virtual QString shareDir() const;

  private:
    QMap<QString, QString> getIntentExtras( QStringList );
    QString getIntentExtra( QString, QAndroidJniObject=0 );
    QAndroidJniObject getNativeIntent();
    QAndroidJniObject getNativeExtras();
};

#endif // ANDROIDPLATFORMUTILITIES_H
