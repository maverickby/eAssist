#ifndef EDPRGCOM_REQDEVNUM_H
#define EDPRGCOM_REQDEVNUM_H

#include "../EDCommand.h"

class EDPrgCom_ReqDevNum : public EDCommand
{
public:
    explicit EDPrgCom_ReqDevNum(const QString &name);
    char getDevNum() const;

protected:
    virtual void get_tx_data(QByteArray & data);
    virtual EDCommandResultState process_rx_data(const QByteArray & data);

private:
    char m_devnum;
};

#endif // EDPRGCOM_REQDEVNUM_H
