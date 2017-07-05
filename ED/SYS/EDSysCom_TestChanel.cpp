#include "EDSysCom_TestChanel.h"

//-----------------------------------------------------------------------------------------
EDSysCom_TestChanel::EDSysCom_TestChanel(const QString &name):
    EDCommand(name)
{
    m_signature.resize(1);

    setTimeout(1000);
    setFinishType(FT_FIXED_DATA);
}
//-----------------------------------------------------------------------------------------
void EDSysCom_TestChanel::setTestSignature(const QByteArray &value)
{
    m_signature = value;
    setRxDataSize(m_signature.size() + 1);
}
//-----------------------------------------------------------------------------------------
void EDSysCom_TestChanel::get_tx_data(QByteArray &data)
{
    data.clear();

    data.resize(2 + m_signature.size() + 1);
    data[0] = 0x01;
    data[1] = 0x07;

    for(int i = 2; i < m_signature.size() + 2; i++) data[i] = m_signature[i - 2];

    data[data.size() - 1] = 0;
}
//-----------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDSysCom_TestChanel::process_rx_data(const QByteArray &data)
{
    bool data_ok = true;
    int size = m_signature.size();

    if(data.size() < size) return CR_Waiting;

    for(int i = 0; i < size - 1; i++) data_ok &= data[i] == m_signature[i];

    if(data_ok) return CR_Success;
    else return CR_BadData;
}


