#include "EDSysCom_ReqDevInfo.h"

//-----------------------------------------------------------------------------------------------------
EDSysCom_ReqDevInfo::EDSysCom_ReqDevInfo(const QString &name):
    EDCommand(name)
{
    m_deviceinfo = "not known";

    setTimeout(1000);
    setFinishType(FT_ZERO_ENDING);
    setRxDataSize(0);
}
//-----------------------------------------------------------------------------------------------------
const QString &EDSysCom_ReqDevInfo::DeviceInfo()
{
    return m_deviceinfo;
}
//-----------------------------------------------------------------------------------------------------
void EDSysCom_ReqDevInfo::get_tx_data(QByteArray &data)
{
    data.clear();

    data.resize(2);

    data[0] = 0x01;
    data[1] = 0x03;
}
//-----------------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDSysCom_ReqDevInfo::process_rx_data(const QByteArray &data)
{
    if(data.size() < 2) return CR_Waiting;

    QByteArray tmp = data;
    m_deviceinfo = QString(tmp);

    return CR_Success;
}



