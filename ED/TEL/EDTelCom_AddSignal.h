#ifndef EDTELCOM_ADDSIGNAL_H
#define EDTELCOM_ADDSIGNAL_H

#include "../EDCommand.h"

class EDTelCom_AddSignal : public EDCommand
{
public:
    EDTelCom_AddSignal(const QString &name);

    void setSignalID(uint value);
    uint getSignalID() const;

protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);

private:
    uint m_signal_id;
};

#endif // EDTELCOM_ADDSIGNAL_H
