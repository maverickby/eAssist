#ifndef EDSYSCOM_REQDEVINFO_H
#define EDSYSCOM_REQDEVINFO_H

#include "../EDCommand.h"

class EDSysCom_ReqDevInfo : public EDCommand
{
public:
    EDSysCom_ReqDevInfo(const QString &name);

    const QString &DeviceInfo();
protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);

private:
    QString m_deviceinfo;
};

#endif // EDSYSCOM_REQDEVINFO_H
