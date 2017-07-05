#ifndef EDSYSCOM_CHECKCHANNEL_H
#define EDSYSCOM_CHECKCHANNEL_H

#include "../EDCommand.h"

#define RX_DATA_SIZE 8
#define TX_DATA_SIZE 8

class EDSysCom_CheckChannel : public EDCommand
{
public:
    explicit EDSysCom_CheckChannel(const QString &name);

protected:
    virtual void get_tx_data(QByteArray & data);
    virtual EDCommandResultState process_rx_data(const QByteArray & data);
};

#endif // EDSYSCOM_CHECKCHANNEL_H
