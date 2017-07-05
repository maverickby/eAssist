#ifndef EDPRGCOM_PREPFORPROG_H
#define EDPRGCOM_PREPFORPROG_H

#include "../EDCommand.h"

class EDPrgCom_PrepForProg : public EDCommand
{
public:
    explicit EDPrgCom_PrepForProg(const QString &name);

    void setErase(bool value);
    bool getErase() const;

    char getDeviceID() const;
    void setDeviceID(char value);

    int getPrepTimeout() const;
    void setPrepTimeout(int value);

protected:
    virtual void get_tx_data(QByteArray & data);
    virtual EDCommandResultState process_rx_data(const QByteArray & data);

private:
    bool m_erase;
    char m_device_id;
    int m_preptimeout;
};

#endif // EDPRGCOM_PREPFORPROG_H
