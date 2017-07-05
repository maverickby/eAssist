#include "EDPrgCom_PrepForProg.h"

//--------------------------------------------------------------------------------------------------------
EDPrgCom_PrepForProg::EDPrgCom_PrepForProg(const QString &name):
    EDCommand(name)
{
    m_erase = true;
    m_device_id = -1;

    setFinishType(FT_NODATA);
    setTimeout(100);
    setRxDataSize(0);

    setPrepTimeout(1000);
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_PrepForProg::setErase(bool value)
{
    m_erase = value;
}
//--------------------------------------------------------------------------------------------------------
bool EDPrgCom_PrepForProg::getErase() const
{
    return m_erase;
}
//--------------------------------------------------------------------------------------------------------
char EDPrgCom_PrepForProg::getDeviceID() const
{
    return m_device_id;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_PrepForProg::setDeviceID(char value)
{
    m_device_id = value;
}
//--------------------------------------------------------------------------------------------------------
int EDPrgCom_PrepForProg::getPrepTimeout() const
{
    return m_preptimeout;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_PrepForProg::setPrepTimeout(int value)
{
    m_preptimeout = value;
    setTimeout(m_preptimeout);
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_PrepForProg::get_tx_data(QByteArray & data)
{
    data.clear();

    data.resize(4);

    data[0] = 0x02;
    data[1] = 0x05;
    data[2] = m_device_id;
    if(m_erase) data[3] = 0x01;
    else data[3] = 0x00;
}
//--------------------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDPrgCom_PrepForProg::process_rx_data(const QByteArray &data)
{
    if(data.size() == 0) return CR_Success;
    else return CR_BadData;
}
