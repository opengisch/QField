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

    //! returns the mimetype of a filepath as string
    Q_INVOKABLE static QString mimeTypeName( const QString filePath );
    //! returns the filename of a filepath - if no file name exists it's empty
    Q_INVOKABLE static QString fileName( const QString filePath );
    //! returns true if the file exists
    Q_INVOKABLE static bool fileExists( const QString filePath );

};

#endif // FILEUTILS_H
