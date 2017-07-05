#include "EDPrgCom_ReadID.h"

//--------------------------------------------------------------------------------------------------------
EDPrgCom_ReadID::EDPrgCom_ReadID(const QString &name):
    EDCommand(name)
{
    m_device_id = -1;
    m_ID = 0;

    setFinishType(FT_FIXED_DATA);
    setTimeout(1000);
    setRxDataSize(4);
}
//--------------------------------------------------------------------------------------------------------
char EDPrgCom_ReadID::getDeviceID() const
{
    return m_device_id;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_ReadID::setDeviceID(char value)
{
    m_device_id = value;
}
//--------------------------------------------------------------------------------------------------------
char EDPrgCom_ReadID::getID() const
{
    return m_ID;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_ReadID::setID(uint value)
{
    m_ID = value;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_ReadID::get_tx_data(QByteArray & data)
{
    data.clear();

    data.resize(3);

    data[0] = 0x02;
    data[1] = 0x03;
    data[2] = m_device_id;
}
//--------------------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDPrgCom_ReadID::process_rx_data(const QByteArray &data)
{
    if(data.size() < 4) return CR_Waiting;
    if(data.size() > 4) return CR_BadData;

    m_ID = (static_cast<uint>(data[0])&0xFF) + ( (static_cast<uint>(data[1])&0xFF) << 8 ) + ( (static_cast<uint>(data[2])&0xFF) << 16 ) + ( (static_cast<uint>(data[3])&0xFF) << 24 );

    return CR_Success;
}

