#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : public QSettings
{
    Q_OBJECT
  public:
    explicit Settings( QObject *parent = 0 );

    Q_INVOKABLE
    void setValue( const QString &key, const QVariant &value );

    Q_INVOKABLE
    QVariant value( const QString &key, const QVariant &defaultValue );

  signals:
    void settingChanged( const QString& key );
};

#endif // SETTINGS_H
