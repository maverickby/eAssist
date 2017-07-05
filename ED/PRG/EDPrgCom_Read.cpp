#include "EDPrgCom_Read.h"

//--------------------------------------------------------------------------------------------------------
EDPrgCom_Read::EDPrgCom_Read(const QString &name):
    EDCommand(name)
{
    m_device_id = -1;
    m_rd_block_size = 0;

    setFinishType(FT_FIXED_DATA);
    setRxDataSize(m_rd_block_size);
    setTimeout(5000);
}
//--------------------------------------------------------------------------------------------------------
char EDPrgCom_Read::getDeviceID() const
{
    return m_device_id;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_Read::setDeviceID(char value)
{
    m_device_id = value;
}
//--------------------------------------------------------------------------------------------------------
char EDPrgCom_Read::getRdBlockSize() const
{
    return m_rd_block_size;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_Read::setRdBlockSize(uint value)
{
    m_rd_block_size = value;
    setRxDataSize(value);
}
//--------------------------------------------------------------------------------------------------------
const QByteArray &EDPrgCom_Read::getRxData() const
{
    return m_rx_data;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_Read::get_tx_data(QByteArray &data)
{
    data.clear();

    data.resize(3);

    data[0] = 0x02;
    data[1] = 0x04;
    data[2] = m_device_id;
}
//--------------------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDPrgCom_Read::process_rx_data(const QByteArray &data)
{
    if(data.size() < getRxDataSize()) return CR_Waiting;
    else
    {
        m_rx_data = data;
        return CR_Success;
    }
}



