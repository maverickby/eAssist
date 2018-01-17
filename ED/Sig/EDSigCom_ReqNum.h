#ifndef EDSIGCOM_REQNUM_H
#define EDSIGCOM_REQNUM_H

#include "../EDCommand.h"

class EDSigCom_ReqNum : public EDCommand
{
public:
    EDSigCom_ReqNum(const QString &name);

    uint getNumSignals() const;
	void setNumSignals(int val);

protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);

private:
    uint m_NumSignals;
};

#endif // EDSIGCOM_REQNUM_H
