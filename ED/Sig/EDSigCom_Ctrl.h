#ifndef EDSIGCOM_CTRL_H
#define EDSIGCOM_CTRL_H

#include "../EDCommand.h"
#include "EDSignal.h"

class EDSigCom_Ctrl : public EDCommand
{
public:
    explicit EDSigCom_Ctrl(const QString &name);

    void setSignalID(uint value);
    uint getSignalID() const;

    void setDescriptor(const EDSignalDescriptor &value);
    const EDSignalDescriptor &getDescriptor() const;

    void setCtrlMode(EDSigCtrl_Mode value);
    EDSigCtrl_Mode getCtrlMode() const;

    void setValuePtr(void *pvalue);

protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);

private:
    uint                 m_signal_id;
    EDSignalDescriptor   m_descriptor;
    EDSigCtrl_Mode       m_ctrl_mode;
    void                *m_value_ptr;
};

#endif // EDSIGCOM_CTRL_H
