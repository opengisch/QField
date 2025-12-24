/***************************************************************************
                              cogoexecutor.h
                              -------------------
              begin                : 2025.12.21
              copyright            : (C) 2025 by Mathieu Pellerin
              email                : mathieu (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef COGOEXECUTOR_H
#define COGOEXECUTOR_H

#include "rubberbandmodel.h"

#include <QMap>
#include <QObject>
#include <QVariant>

/**
 * \ingroup core
 */
class CogoExecutor : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString name READ name WRITE setName NOTIFY nameChanged )
    Q_PROPERTY( QVariantMap parameters READ parameters WRITE setParameters NOTIFY parametersChanged )
    Q_PROPERTY( bool isReady READ isReady NOTIFY isReadyChanged )

    Q_PROPERTY( RubberbandModel *rubberbandModel READ rubberbandModel WRITE setRubberbandModel NOTIFY rubberbandModelChanged )

  public:
    explicit CogoExecutor( QObject *parent = nullptr );

    QString name() const { return mName; }
    void setName( const QString &name );

    QVariantMap parameters() const { return mParameters; }
    void setParameters( const QVariantMap &parameters );

    bool isReady() const { return mIsReady; }

    RubberbandModel *rubberbandModel() const { return mRubberbandModel; }
    void setRubberbandModel( RubberbandModel *rubberbandModel );

    Q_INVOKABLE bool execute();

  signals:
    void nameChanged();
    void parametersChanged();
    void isReadyChanged();
    void rubberbandModelChanged();

  private:
    void checkReadiness();

    QString mName;
    QVariantMap mParameters;
    bool mIsReady = false;

    RubberbandModel *mRubberbandModel = nullptr;
};

Q_DECLARE_METATYPE( CogoExecutor )
#endif // COGOEXECUTOR_H
