#include <QFile>
#include <QDataStream>
#include "Settings.h"
#include "ED/EDLogger.h"
//---------------------------------------------------------------------------------------------------------
Settings::Settings():
    m_filename("settings.dat")
{
    setDefaults();
}
//---------------------------------------------------------------------------------------------------------
void Settings::Save(QString path)
{
    QString filename = path + "\\" + m_filename;

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);

    out << m_showresponce;
    int tmp = static_cast<int>(m_interface_type);
    out << tmp;

    out << m_comport_settings.port;
    out << m_comport_settings.baudrate;
    out << static_cast<int>(m_comport_settings.parity);

    out << m_udp_settings.ip_address;
    out << m_udp_settings.port;

    out << m_wrapper_settings.enable;
    out << m_wrapper_settings.local_address;
    out << m_wrapper_settings.remote_address;

    out << m_code1;
    out << m_code2;

    out << m_timeout_descriptor;
    out << m_period_telemetry;

    file.close();
}
//---------------------------------------------------------------------------------------------------------
void Settings::Restore(QString path)
{
    QString filename = path + "\\" + m_filename;

    if(QFile::exists(filename))
    {
        QFile file(filename);
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);

        in >> m_showresponce;
        int tmp;
        in >> tmp;
        m_interface_type = static_cast<INTERFACE_TYPE>(tmp);

        in >> m_comport_settings.port;
        in >> m_comport_settings.baudrate;
        in >> tmp;
        m_comport_settings.parity = static_cast<ParityType>(tmp);

        in >> m_udp_settings.ip_address;
        in >> m_udp_settings.port;

        in >> m_wrapper_settings.enable;
        in >> m_wrapper_settings.local_address;
        in >> m_wrapper_settings.remote_address;

        in >> m_code1;
        in >> m_code2;

        in >> m_timeout_descriptor;
        in >> m_period_telemetry;

        file.close();
    } else // file not exist, restore defaults values
    {
        setDefaults();
    }
}
//---------------------------------------------------------------------------------------------------------
bool Settings::isShowResponce() const
{
    return m_showresponce;
}
//---------------------------------------------------------------------------------------------------------
INTERFACE_TYPE Settings::getInterfaceType() const
{
    return m_interface_type;
}
//---------------------------------------------------------------------------------------------------------
ComPort_Settings Settings::getComPortSettings() const
{
    return m_comport_settings;
}
//---------------------------------------------------------------------------------------------------------
UDP_Settings Settings::getUDPSettings() const
{
    return m_udp_settings;
}
//---------------------------------------------------------------------------------------------------------
Wrapper_Settings Settings::getWrapperSettings() const
{
    return m_wrapper_settings;
}
//---------------------------------------------------------------------------------------------------------
QString Settings::getCode1() const
{
    return m_code1;
}
//---------------------------------------------------------------------------------------------------------
QByteArray Settings::getCode1ByteArray()
{
    bool ok;
    QByteArray tmp = str2arr(&ok, m_code1);
    if(!ok) logger().WriteLn("Error while converting code1", Qt::red);

    return tmp;
}
//---------------------------------------------------------------------------------------------------------
QString Settings::getCode2() const
{
    return m_code2;
}
//---------------------------------------------------------------------------------------------------------
QByteArray Settings::getCode2ByteArray()
{
    bool ok;
    QByteArray tmp = str2arr(&ok, m_code2);
    if(!ok) logger().WriteLn("Error while converting code1", Qt::red);

    return tmp;
}
//---------------------------------------------------------------------------------------------------------
uint32_t Settings::getTimeoutDescriptor() const
{
    return m_timeout_descriptor;
}
//---------------------------------------------------------------------------------------------------------
uint32_t Settings::getPeriodTelemetry() const
{
    return m_period_telemetry;
}
//---------------------------------------------------------------------------------------------------------
void Settings::setDefaults()
{
    m_showresponce = false;
    m_interface_type = IT_UDP;

    m_comport_settings.port = 3;
    m_comport_settings.baudrate = 460800;

    m_udp_settings.ip_address = "192.168.200.220";
    m_udp_settings.port = 1011;

    m_wrapper_settings.enable = false;
    m_wrapper_settings.local_address = 1;
    m_wrapper_settings.remote_address = 2;

    m_code1 = "77 57 3D 44 55 66 77 89 99 AB BB CC DD EE FF EF";
    m_code2 = "5D AA 00 00 A2 55 00 00";

    m_timeout_descriptor = 10;
    m_period_telemetry = 1;
}
//---------------------------------------------------------------------------------------------------------
QByteArray Settings::str2arr(bool *ok, QString value)
{
    QString tmp_str = value;
    tmp_str.remove(QString(" "));

    QByteArray tmp;
    for(; tmp_str.size() > 0;)
    {
        int sz = (tmp_str.size() > 1)?2:1;
        QString val_str = tmp_str.left(sz);
        tmp_str.remove(0, sz);
        int val = val_str.toInt(ok, 16);

        if (*ok) tmp.append(static_cast<char>(val));
        else
        {
            tmp.clear();
            return tmp;
        }
    }

    return tmp;
}
//---------------------------------------------------------------------------------------------------------
Settings &SettingsSingletone::getInstance()
{
    return m_settings;
}
//---------------------------------------------------------------------------------------------------------
Settings SettingsSingletone::m_settings; //init static variables

Settings &settings()
{
    return SettingsSingletone::getInstance();
}


