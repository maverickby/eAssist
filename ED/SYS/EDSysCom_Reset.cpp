#include "EDSysCom_Reset.h"

//----------------------------------------------------------------------------------------------
EDSysCom_Reset::EDSysCom_Reset(const QString &name):
    EDCommand(name)
{
    setTimeout(1000);
    setFinishType(FT_NODATA);
    setRxDataSize(0);
}
//----------------------------------------------------------------------------------------------
void EDSysCom_Reset::get_tx_data(QByteArray &data)
{
    data.clear();

    data.resize(2);

    data[0] = 0x01;
    data[1] = 0x02;
}
//----------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDSysCom_Reset::process_rx_data(const QByteArray &data)
{
    if(data.size() == 0) return CR_Success;
    else return CR_BadData;
}
