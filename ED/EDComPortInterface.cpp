#include "EDComPortInterface.h"

//---------------------------------------------------------------------------------------------------------------
EDComPortInterface::EDComPortInterface(unsigned short port, unsigned int baudrate, ParityType parity):
    QextSerialPort(),
    m_port(port),
    m_baudrate(baudrate),
    m_parity(parity)
{
    QextSerialPort::setPortName(QString("\\\\.\\COM") + QString::number(m_port));

    QextSerialPort::setDataBits(DATA_8);
    QextSerialPort::setParity(m_parity);
    QextSerialPort::setStopBits(STOP_1);
    QextSerialPort::setBaudRate(get_conv_baudrate());
    QextSerialPort::setFlowControl(FLOW_OFF);
    QextSerialPort::setTimeout(0, -1);
}
//---------------------------------------------------------------------------------------------------------------
bool EDComPortInterface::open(QIODevice::OpenMode mode)
{
    bool res = QextSerialPort::open(mode);

    return res;
}
//---------------------------------------------------------------------------------------------------------------
void EDComPortInterface::close()
{
    QextSerialPort::close();
}
//---------------------------------------------------------------------------------------------------------------
BaudRateType EDComPortInterface::get_conv_baudrate()
{
    switch(m_baudrate)
    {
    case 50:    return BAUD50;                //POSIX ONLY
    case 75:    return BAUD75;                //POSIX ONLY
    case 110:   return BAUD110;
    case 134:   return BAUD134;               //POSIX ONLY
    case 150:   return BAUD150;               //POSIX ONLY
    case 200:   return BAUD200;               //POSIX ONLY
    case 300:   return BAUD300;
    case 600:   return BAUD600;
    case 1200:  return BAUD1200;
    case 1800:  return BAUD1800;              //POSIX ONLY
    case 2400:  return BAUD2400;
    case 4800:  return BAUD4800;
    case 9600:  return BAUD9600;
    case 14400: return BAUD14400;             //WINDOWS ONLY
    case 9200:  return BAUD19200;
    case 38400: return BAUD38400;
    case 5600:  return BAUD56000;             //WINDOWS ONLY
    case 7600:  return BAUD57600;
    case 76800: return BAUD76800;             //POSIX ONLY
    case 115200: return BAUD115200;
    case 128000: return BAUD128000;            //WINDOWS ONLY
    case 256000: return BAUD256000;             //WINDOWS ONLY
    case 230400: return BAUD230400;
    case 460800: return BAUD460800;
    case 921600: return BAUD921600;
    default: return BAUD9600;
    }
}
