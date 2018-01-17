#include "EDSigCom_ReqNum.h"

//----------------------------------------------------------------------------------------
EDSigCom_ReqNum::EDSigCom_ReqNum(const QString &name):
    EDCommand(name)
{
    m_NumSignals = 0;

    setFinishType(FT_FIXED_DATA);
    setTimeout(1000);
    setRxDataSize(2);
}
//----------------------------------------------------------------------------------------
void EDSigCom_ReqNum::get_tx_data(QByteArray &data)
{
    data.clear();

    data.resize(2);
    data[0] = 0x03;
    data[1] = 0x01;
}
//----------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDSigCom_ReqNum::process_rx_data(const QByteArray &data)
{
    if(data.size() < 2) return CR_Waiting;
    if(data.size() > 2) return CR_BadData;

    m_NumSignals = (static_cast<uint>(data.at(0))&0xFF) | ((static_cast<uint>(data.at(1))&0xFF) << 8);
    return CR_Success;
}
//----------------------------------------------------------------------------------------
uint EDSigCom_ReqNum::getNumSignals() const
{
    return m_NumSignals;
}
void EDSigCom_ReqNum::setNumSignals(int val)
{
	m_NumSignals = val;
}

