#ifndef EDPRGCOM_READID_H
#define EDPRGCOM_READID_H

#include "../EDCommand.h"

class EDPrgCom_ReadID : public EDCommand
{
public:
    explicit EDPrgCom_ReadID(const QString &name);

    char getDeviceID() const;
    void setDeviceID(char value);

    char getID() const;
    void setID(uint value);

protected:
    virtual void get_tx_data(QByteArray & data);
    virtual EDCommandResultState process_rx_data(const QByteArray & data);

private:
    char m_device_id;
    uint m_ID;
};

#endif // EDPRGCOM_READID_H
