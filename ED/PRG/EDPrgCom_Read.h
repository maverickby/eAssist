#ifndef EDPRGCOM_READ_H
#define EDPRGCOM_READ_H

#include "../EDCommand.h"

class EDPrgCom_Read : public EDCommand
{
public:
    explicit EDPrgCom_Read(const QString &name);

    char getDeviceID() const;
    void setDeviceID(char value);

    char getRdBlockSize() const;
    void setRdBlockSize(uint value);

    const QByteArray &getRxData() const;

protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);

private:
    char m_device_id;
    uint m_rd_block_size;
    QByteArray m_rx_data;

};

#endif // EDPRGCOM_READ_H
