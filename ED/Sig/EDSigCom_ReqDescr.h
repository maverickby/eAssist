#ifndef EDSIGCOM_REQDESCR_H
#define EDSIGCOM_REQDESCR_H

#include "../EDCommand.h"
#include "EDSignal.h"

// ***** DASigCom_ReqDescr_t *****
class EDSigCom_ReqDescr : public EDCommand
{
public:
    explicit EDSigCom_ReqDescr(const QString &name);

    void setSignalID(uint value);
    uint getSignalID() const;

    const EDSignalDescriptor &Descriptor() const;

protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);

private:
    EDSignalDescriptor m_descriptor;
    uint m_signal_id;

    typedef  union
        {uint _u32;
         float 	_float;
        } long_float_t;
};

#endif // EDSIGCOM_REQDESCR_H


