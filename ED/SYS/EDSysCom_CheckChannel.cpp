#include "EDSysCom_CheckChannel.h"

//-------------------------------------------------------------------------------------------
EDSysCom_CheckChannel::EDSysCom_CheckChannel(const QString &name) :
    EDCommand(name)
{
    setFinishType(FT_FIXED_DATA);
    setRxDataSize(RX_DATA_SIZE);
}
//-------------------------------------------------------------------------------------------
void EDSysCom_CheckChannel::get_tx_data(QByteArray & data)
{
    uchar tmp = 0xAA;

    data.clear();
    data.resize(2 + TX_DATA_SIZE);
    data[0] = 0x01;
    data[1] = 0x01;

    for(char i = 2; i < 2 + TX_DATA_SIZE; i++)
    {
        tmp ^= i;
        data[i] = tmp;
    }
}
//-------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDSysCom_CheckChannel::process_rx_data(const QByteArray & data)
{
    bool data_ok = true;
    uchar tmp = 0xAA;

    if(data.size() < RX_DATA_SIZE) return CR_Waiting;

    for(char i = 0; i < RX_DATA_SIZE; i++)
    {
        tmp ^= i + 2;
        data_ok &= (static_cast<uchar>(data[i]) == tmp);
    }

    if(data_ok) return CR_Success;
    else return CR_BadData;
}

