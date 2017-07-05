#ifndef EDSYSCOM_SAVESETTINGS_H
#define EDSYSCOM_SAVESETTINGS_H

#include "../EDCommand.h"

class EDSysCom_SaveSettings : public EDCommand
{
public:
    explicit EDSysCom_SaveSettings(const QString &name);

protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);
};

#endif // EDSYSCOM_SAVESETTINGS_H
