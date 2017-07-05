#ifndef EDSYSCOM_SETSCANMODE_H
#define EDSYSCOM_SETSCANMODE_H

#include "../EDCommand.h"

class EDSysCom_SetScanMode : public EDCommand
{
public:
    explicit EDSysCom_SetScanMode(const QString &name);

    void setScanMode(bool state);
    bool getScanMode() const;

protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);

private:
    bool m_enable_scan_mode;
};

#endif // EDSYSCOM_SETSCANMODE_H
