#include "EDPrgCom_Write.h"

//--------------------------------------------------------------------------------------------------------
EDPrgCom_Write::EDPrgCom_Write(const QString &name):
    EDCommand(name)
{
    m_device_id = -1;
    m_wr_timeout = 0;

    setFinishType(FT_NODATA);
    setTimeout(100);
    setTimeout(m_wr_timeout);
}
//--------------------------------------------------------------------------------------------------------
char EDPrgCom_Write::getDeviceID() const
{
    return m_device_id;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_Write::setDeviceID(char value)
{
    m_device_id = value;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_Write::setWrData(QByteArray value)
{
    m_wr_data = value;
}
//--------------------------------------------------------------------------------------------------------
const QByteArray &EDPrgCom_Write::getWrData() const
{
    return m_wr_data;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_Write::setWrTimeout(uint value)
{
    m_wr_timeout = value;
    setTimeout(value);
}
//--------------------------------------------------------------------------------------------------------
uint EDPrgCom_Write::getWrTimeout() const
{
    return m_wr_timeout;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_Write::get_tx_data(QByteArray & data)
{
    data.clear();

    data.resize(3);

    data[0] = 0x02;
    data[1] = 0x06;
    data[2] = m_device_id;

    data.append(m_wr_data);
}
//--------------------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDPrgCom_Write::process_rx_data(const QByteArray &data)
{
    if(data.size() == 0) return CR_Success;
    else return CR_BadData;
}
