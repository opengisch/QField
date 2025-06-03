#ifndef VENDORCOMMANDHELPER_H
#define VENDORCOMMANDHELPER_H

#include <QObject>
#include <QIODevice>

class VendorCommandHelper : public QObject
{
    Q_OBJECT

  public:
    explicit VendorCommandHelper( QIODevice *ioDevice = nullptr, QObject *parent = nullptr );

    void setDevice( QIODevice *ioDevice );
    bool sendCommand( const QString &command );

    bool configImuToAntOffset( double x, double y, double z,
                               double stdx, double stdy, double stdz );
    bool configInsSlantMeas();
    bool configInsDisable();
    bool configInsReset();
    bool configAntennaDeltaHen( double len );
    bool eraseImuParam();
    bool configInsReliability( int level );
    bool saveConfig();

  private:
    QIODevice *mIoDevice = nullptr;
};

#endif // VENDORCOMMANDHELPER_H
