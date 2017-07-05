#ifndef EDSYSCOM_TESTCHANEL_H
#define EDSYSCOM_TESTCHANEL_H

#include "../EDCommand.h"

class EDSysCom_TestChanel : public EDCommand
{
public:
    EDSysCom_TestChanel(const QString &name);

    void setTestSignature(const QByteArray &value);

protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);

private:
    QByteArray m_signature;
};

#endif // EDSYSCOM_TESTCHANEL_H
