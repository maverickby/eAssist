#ifndef EDSIGCOM_SETSIGNATURE_H
#define EDSIGCOM_SETSIGNATURE_H

#include <QList>
#include "../EDCommand.h"
#include "EDSignal.h"

// !!!!!!!!!!!!!!!!!!!!!  this command not yet fully implemented  !!!!!!!!!!!!!!!!!!!!!!!!

class EDSigCom_SetSignature : public EDCommand
{
public:
    explicit EDSigCom_SetSignature(const QString &name);

    void setSignalID(uint value);
    uint getSignalID() const ;

    void setDescriptor(const EDSignalDescriptor &value);
    const EDSignalDescriptor &getDescriptor() const;

    void setValuesList(const QList<void*> &list);

    void setCoef(uint value);
    uint getCoef() const;

protected:
    virtual void get_tx_data(QByteArray & data);
    virtual EDCommandResultState process_rx_data(const QByteArray & data);

private:
    uint                 m_signal_id;
    EDSignalDescriptor   m_descriptor;
    QList<void*>         m_values_list;
    uint                 m_coef;
};

#endif // EDSIGCOM_SETSIGNATURE_H
