#include "EDPrgCom_Restart.h"

//--------------------------------------------------------------------------------------------------------
EDPrgCom_Restart::EDPrgCom_Restart(const QString &name):
    EDCommand(name)
{
    m_device_id = -1;

    setFinishType(FT_NODATA);
    setTimeout(1000);
}
//--------------------------------------------------------------------------------------------------------
char EDPrgCom_Restart::getDeviceID() const
{
    return m_device_id;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_Restart::setDeviceID(char value)
{
    m_device_id = value;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_Restart::get_tx_data(QByteArray &data)
{
    data.clear();

    data.resize(3);

    data[0] = 0x02;
    data[1] = 0x08;
    data[2] = m_device_id;
}
//--------------------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDPrgCom_Restart::process_rx_data(const QByteArray &data)
{
    if(data.size() == 0) return CR_Success;
    else return CR_BadData;
}
