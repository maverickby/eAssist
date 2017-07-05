#ifndef EDPRGCOM_READDESCR_H
#define EDPRGCOM_READDESCR_H

#include "../EDCommand.h"

typedef struct
{
    QString name;
    uint wr_block_size;
    uint rd_block_size;
    uint prog_size;
    uint wr_timeout;
    uint prep_timeout;
    bool restart;
} EDDeviceDescriptor;

class EDPrgCom_ReadDescr : public EDCommand
{
public:
    explicit EDPrgCom_ReadDescr(const QString &name);

    const EDDeviceDescriptor &Descriptor() const;

    char getDeviceID() const;
    void setDeviceID(char value);

protected:
    virtual void get_tx_data(QByteArray & data);
    virtual EDCommandResultState process_rx_data(const QByteArray & data);

private:
    EDDeviceDescriptor m_descriptor;
    char m_device_id;
};

#endif // EDPRGCOM_READDESCR_H
