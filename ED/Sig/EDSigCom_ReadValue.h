#ifndef EDSIGCOM_READVALUE_H
#define EDSIGCOM_READVALUE_H

#include "../EDCommand.h"
#include "EDSignal.h"

class EDSigCom_ReadValue : public EDCommand
{
public:
    explicit EDSigCom_ReadValue(const QString &name);

    void setSignalID(uint value);
    uint getSignalID() const;

    void setDescriptor(const EDSignalDescriptor &value);
    const EDSignalDescriptor &getDescriptor() const;

    void setValuePtr(void *value);

protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);

private:
    uint                 m_signal_id;
    EDSignalDescriptor   m_descriptor;
    void                *m_value_ptr;
};

#endif // EDSIGCOM_READVALUE_H
