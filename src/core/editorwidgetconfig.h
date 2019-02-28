#ifndef EDITORWIDGETCONFIG_H
#define EDITORWIDGETCONFIG_H

#include <QObject>
#include <QVariantMap>

class EditorWidgetConfig
{
    Q_GADGET

    Q_PROPERTY( QVariantMap configuration READ configuration )

  public:
    EditorWidgetConfig() = default;
    EditorWidgetConfig( const QVariantMap &configuration );
    virtual ~EditorWidgetConfig() = default;

    QVariantMap configuration() const;
    Q_INVOKABLE QVariant value( const QString &key ) const;

  private:
    QVariantMap mConfiguration;
};

#endif // EDITORWIDGETCONFIG_H
