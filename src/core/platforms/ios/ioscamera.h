#ifndef IOSCAMERA_H
#define IOSCAMERA_H

#include <QQuickItem>

class CameraDelegateContainer;

class ImagePicker : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString imagePath READ imagePath NOTIFY imagePathChanged)

public:
    explicit ImagePicker(QQuickItem *parent = 0);

    QString imagePath() {
        return m_imagePath;
    }

    QString m_imagePath;

signals:
    void pictureReceived( const QString &path );
    void imagePathChanged();

public slots:
    void open();

private:
    class CameraDelegateContainer;
    CameraDelegateContainer *m_delegate;
};

#endif // IOSCAMERA_H
