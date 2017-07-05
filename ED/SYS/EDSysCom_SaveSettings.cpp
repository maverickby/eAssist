#include "EDSysCom_SaveSettings.h"

//---------------------------------------------------------------------------------------------------------
EDSysCom_SaveSettings::EDSysCom_SaveSettings(const QString &name):
    EDCommand(name)
{
    setTimeout(10000);
    setFinishType(FT_NODATA);
    setRxDataSize(0);
}
//---------------------------------------------------------------------------------------------------------
void EDSysCom_SaveSettings::get_tx_data(QByteArray &data)
{
    data.clear();

    data.resize(2);

    data[0] = 0x01;
    data[1] = 0x06;
}
//---------------------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDSysCom_SaveSettings::process_rx_data(const QByteArray &data)
{
    if(data.size() == 0) return CR_Success;
    else return CR_BadData;
}



