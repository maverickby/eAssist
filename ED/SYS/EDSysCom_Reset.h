#ifndef EDSYSCOM_RESET_H
#define EDSYSCOM_RESET_H

#include "../EDCommand.h"

class EDSysCom_Reset : public EDCommand
{
public:
    explicit EDSysCom_Reset(const QString &name);

protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);
};

#endif // EDSYSCOM_RESET_H
