#ifndef EDPRGCOM_RESTART_H
#define EDPRGCOM_RESTART_H

#include "../EDCommand.h"

class EDPrgCom_Restart : public EDCommand
{
public:
    explicit EDPrgCom_Restart(const QString &name);

    char getDeviceID() const;
    void setDeviceID(char value);

protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);

private:
    char m_device_id;
};

#endif // EDPRGCOM_RESTART_H
