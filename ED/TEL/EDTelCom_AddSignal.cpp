#include "EDTelCom_AddSignal.h"

//----------------------------------------------------------------------------------------------------
EDTelCom_AddSignal::EDTelCom_AddSignal(const QString &name):
    EDCommand(name)
{
    m_signal_id = -1;

    setFinishType(FT_NODATA);
    setTimeout(1000);
}
//----------------------------------------------------------------------------------------------------
void EDTelCom_AddSignal::setSignalID(uint value)
{
    m_signal_id = value;
}
//----------------------------------------------------------------------------------------------------
uint EDTelCom_AddSignal::getSignalID() const
{
    return m_signal_id;
}
//----------------------------------------------------------------------------------------------------
void EDTelCom_AddSignal::get_tx_data(QByteArray &data)
{
    data.clear();

    data.resize(4);

    data[0] = 0x04;
    data[1] = 0x03;
    data[2] = m_signal_id;
    data[3] = m_signal_id >> 8;
}
//----------------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDTelCom_AddSignal::process_rx_data(const QByteArray &data)
{
    if(data.size() == 0) return CR_Success;
    else return CR_BadData;
}


