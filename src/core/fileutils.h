#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QObject>

class FileUtils : public QObject
{
    Q_OBJECT

  public:
    FileUtils( QObject *parent = nullptr );
    //! Destructor
    ~FileUtils() = default;

    Q_INVOKABLE static QString mimeTypeName( const QString filePath );
    Q_INVOKABLE static QString fileName( const QString filePath );

};

#endif // FILEUTILS_H
