#include "EDPrgCom_ReqDevNum.h"

//--------------------------------------------------------------------------------------------------------
EDPrgCom_ReqDevNum::EDPrgCom_ReqDevNum(const QString &name):
    EDCommand(name)
{
    setFinishType(FT_FIXED_DATA);
    setTimeout(1000);
    setRxDataSize(1);

    m_devnum = 0;
}
//--------------------------------------------------------------------------------------------------------
char EDPrgCom_ReqDevNum::getDevNum() const
{
    return m_devnum;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_ReqDevNum::get_tx_data(QByteArray & data)
{
    data.clear();

    data.resize(2);

    data[0] = 0x02;
    data[1] = 0x01;
}
//--------------------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDPrgCom_ReqDevNum::process_rx_data(const QByteArray &data)
{
    int size = data.size();

    if(size > 1) return CR_Overflow;
    else if(size == 0) return CR_BadData;
    else
    {
        m_devnum = data[0];
        return CR_Success;
    }
}

