#ifndef EDSYSCOM_SELFTEST_H
#define EDSYSCOM_SELFTEST_H

#include "../EDCommand.h"

class EDSysCom_SelfTest : public EDCommand
{
public:
    EDSysCom_SelfTest(const QString &name);

    const QString &getSelfTestResult() const;

protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);

private:
    QString m_result_selftest;
};

#endif // EDSYSCOM_SELFTEST_H
