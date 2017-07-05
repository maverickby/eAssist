#include "EDTelCom_SetMode.h"

//-----------------------------------------------------------------------------------------------
EDTelCom_SetMode::EDTelCom_SetMode(const QString &name):
    EDCommand(name)
{
    m_mode        = TM_STOP;
    m_div_coef    = 1;
    m_frame_size  = 0;

    setFinishType(FT_NODATA);
    setTimeout(1000);
    setRxDataSize(0);
}
//-----------------------------------------------------------------------------------------------
void EDTelCom_SetMode::setMode(EDTelMode value)
{
    m_mode = value;
}
//-----------------------------------------------------------------------------------------------
EDTelMode EDTelCom_SetMode::getMode() const
{
    return m_mode;
}
//-----------------------------------------------------------------------------------------------
void EDTelCom_SetMode::setDivCoef(ulong value)
{
    m_div_coef = value;
}
//-----------------------------------------------------------------------------------------------
ulong EDTelCom_SetMode::getDivCoef() const
{
    return m_div_coef;
}
//-----------------------------------------------------------------------------------------------
void EDTelCom_SetMode::setFrameSize(uint value)
{
    m_frame_size = value;
}
//-----------------------------------------------------------------------------------------------
uint EDTelCom_SetMode::getFramesize() const
{
    return m_frame_size;
}
//-----------------------------------------------------------------------------------------------
void EDTelCom_SetMode::get_tx_data(QByteArray &data)
{
    data.clear();

    data.resize(9);

    data[0] = 0x04;
    data[1] = 0x02;
    data[2] = static_cast<uchar>(m_mode);
    data[3] = m_div_coef;
    data[4] = m_div_coef >> 8;
    data[5] = m_div_coef >> 16;
    data[6] = m_div_coef >> 24;
    data[7] = m_frame_size;
    data[8] = m_frame_size >> 8;
}
//-----------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDTelCom_SetMode::process_rx_data(const QByteArray &data)
{
    if(data.size() == 0) return CR_Success;
    else return CR_BadData;
}



