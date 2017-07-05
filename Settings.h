#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <stdint.h>
#include "QextSerialPort/qextserialport.h"

typedef enum {IT_COMPORT, IT_UDP} INTERFACE_TYPE;

typedef struct
{
    unsigned short port;
    unsigned int   baudrate;
    ParityType     parity;
} ComPort_Settings;

typedef struct
{
    QString ip_address;
    unsigned short port;
} UDP_Settings;

typedef struct
{
    bool enable;
    unsigned char local_address;
    unsigned char remote_address;
} Wrapper_Settings;

//------------------------------------------------------------------------------------------------------------
class Settings
{
    friend class SettingsDlg;

public:
    Settings();

    void Save(QString path);
    void Restore(QString path);

    bool isShowResponce() const;

    INTERFACE_TYPE      getInterfaceType() const;
    ComPort_Settings    getComPortSettings() const;
    UDP_Settings        getUDPSettings() const;
    Wrapper_Settings    getWrapperSettings() const;
    QString             getCode1() const;
    QByteArray          getCode1ByteArray();
    QString             getCode2() const;
    QByteArray          getCode2ByteArray();
    uint32_t            getTimeoutDescriptor() const;
    uint32_t            getPeriodTelemetry() const;

private:
    // begin: settings
    bool                m_showresponce;
    INTERFACE_TYPE      m_interface_type;
    ComPort_Settings    m_comport_settings;
    UDP_Settings        m_udp_settings;
    Wrapper_Settings    m_wrapper_settings;
    QString             m_code1;
    QString             m_code2;
    uint32_t            m_timeout_descriptor;
    uint32_t            m_period_telemetry;
    // end: settings

    const QString m_filename;

    void setDefaults();

    QByteArray str2arr(bool *ok, QString value);
};
//------------------------------------------------------------------------------------------------------------
// Singletone settings
class SettingsSingletone
{
public:
    static Settings &getInstance();

private:
    SettingsSingletone();
    SettingsSingletone(SettingsSingletone&);
    SettingsSingletone & operator = (SettingsSingletone&);

    static Settings m_settings;
};

Settings &settings();

#endif // SETTINGS_H
