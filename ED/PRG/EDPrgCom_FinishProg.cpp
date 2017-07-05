#include "EDPrgCom_FinishProg.h"

//--------------------------------------------------------------------------------------------------------
EDPrgCom_FinishProg::EDPrgCom_FinishProg(const QString &name):
    EDCommand(name)
{
    setFinishType(FT_NODATA);
    setTimeout(1000);
    setRxDataSize(0);
    m_device_id = -1;
}
//--------------------------------------------------------------------------------------------------------
char EDPrgCom_FinishProg::getDeviceID()
{
    return m_device_id;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_FinishProg::setDeviceID(char value)
{
    m_device_id = value;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_FinishProg::get_tx_data(QByteArray & data)
{
    data.clear();

    data.resize(3);

    data[0] = 0x02;
    data[1] = 0x07;
    data[2] = m_device_id;
}
//--------------------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDPrgCom_FinishProg::process_rx_data(const QByteArray &data)
{
    if(data.size() == 0) return CR_Success;
    else return CR_BadData;
}



