#ifndef EDCOMPORTINTERFACE_H
#define EDCOMPORTINTERFACE_H

#include "../QextSerialPort/qextserialport.h"

class EDComPortInterface : public QextSerialPort
{
public:
    explicit EDComPortInterface(unsigned short port, unsigned int baudrate, ParityType parity);

    virtual bool	open(OpenMode mode);
    virtual void	close();

private:
    BaudRateType get_conv_baudrate();
    unsigned short m_port;
    unsigned int m_baudrate;
    ParityType m_parity;

signals:

public slots:

};

#endif // EDCOMPORTINTERFACE_H
