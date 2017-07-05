#ifndef EDPRGCOM_WRITE_H
#define EDPRGCOM_WRITE_H

#include "../EDCommand.h"

class EDPrgCom_Write : public EDCommand
{
public:
    explicit EDPrgCom_Write(const QString &name);

    char getDeviceID() const;
    void setDeviceID(char value);

    void setWrData(QByteArray value);
    const QByteArray & getWrData() const;

    void setWrTimeout(uint value);
    uint getWrTimeout() const;

protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);

private:
    char m_device_id;
    QByteArray m_wr_data;
    uint m_wr_timeout;

};

#endif // EDPRGCOM_WRITE_H
