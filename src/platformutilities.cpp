/***************************************************************************
                            utilities.cpp  -  utilities for qfield

                              -------------------
              begin                : Wed Dec 04 10:48:28 CET 2015
              copyright            : (C) 2015 by Marco Bernasocchi
              email                : marco@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "platformutilities.h"
#include <QDebug>
#include <QtAndroid>


PlatformUtilities::PlatformUtilities( QObject *parent ) :
  QObject( parent )
{
}

QString PlatformUtilities::getIntentExtra(QString extra, QAndroidJniObject extras){
    if (extras == 0){
        extras = getNativeExtras();
    }
    if(extras.isValid()){
        QAndroidJniObject extraJni = QAndroidJniObject::fromString(extra);
        extraJni = extras.callObjectMethod("getString", "(Ljava/lang/String;)Ljava/lang/String;", extraJni.object<jstring>());
        if (extraJni.isValid()){
            extra = extraJni.toString();
            return extra;
        }
    }
    return "";
}


QMap<QString, QString> PlatformUtilities::getIntentExtras(QStringList intentExtras)
{
    QAndroidJniObject extras = getNativeExtras();
    QString extraValue, extraName;
    QMap<QString, QString> extraMap;

    for (int i = 0; i < intentExtras.size(); ++i){
        extraName = intentExtras.at(i).toLocal8Bit().constData();
        extraValue = getIntentExtra(extraValue, extras);
        extraMap.insert(extraName, extraValue);
    }
    return extraMap;
}

QAndroidJniObject PlatformUtilities::getNativeIntent()
{
    QAndroidJniObject activity = QtAndroid::androidActivity();
    if (activity.isValid()) {
        QAndroidJniObject intent = activity.callObjectMethod("getIntent", "()Landroid/content/Intent;");
        if (intent.isValid()) {
            qDebug() << "Intent: " << intent.toString();
            return intent;
        }
    }
    return 0;
}

QAndroidJniObject PlatformUtilities::getNativeExtras()
{
    QAndroidJniObject intent = getNativeIntent();
    if (intent.isValid()){
        QAndroidJniObject extras = intent.callObjectMethod("getExtras", "()Landroid/os/Bundle;");
        qDebug() << "Extras: " << extras.toString();

        return extras;
    }
    return 0;
}
