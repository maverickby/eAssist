#ifndef EDPRGCOM_FINISHPROG_H
#define EDPRGCOM_FINISHPROG_H

#include "../EDCommand.h"

class EDPrgCom_FinishProg : public EDCommand
{
public:
    explicit EDPrgCom_FinishProg(const QString &name);

    char getDeviceID();
    void setDeviceID(char value);

protected:
    virtual void get_tx_data(QByteArray & data);
    virtual EDCommandResultState process_rx_data(const QByteArray & data);

private:
    char m_device_id;
};

#endif // EDPRGCOM_FINISHPROG_H
