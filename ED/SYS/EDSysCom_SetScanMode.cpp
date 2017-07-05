#include "EDSysCom_SetScanMode.h"

//--------------------------------------------------------------------------------
EDSysCom_SetScanMode::EDSysCom_SetScanMode(const QString &name):
    EDCommand(name)
{
    m_enable_scan_mode = false;
    setTimeout(1000);
    setFinishType(FT_NODATA);
    setRxDataSize(0);
}
//--------------------------------------------------------------------------------
void EDSysCom_SetScanMode::setScanMode(bool state)
{
    m_enable_scan_mode = state;
}
//--------------------------------------------------------------------------------
bool EDSysCom_SetScanMode::getScanMode() const
{
    return m_enable_scan_mode;
}
//--------------------------------------------------------------------------------
void EDSysCom_SetScanMode::get_tx_data(QByteArray &data)
{
    data.clear();

    data.resize(3);

    data[0] = 0x01;
    data[1] = 0x04;
    if(m_enable_scan_mode) data[2] = 0x01;
    else data[2] = 0x00;
}
//--------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDSysCom_SetScanMode::process_rx_data(const QByteArray &data)
{
    if(data.size() == 0) return CR_Success;
    else return CR_BadData;
}
