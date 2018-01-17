#include "EDTelCom_ReadData.h"

//-----------------------------------------------------------------------------------------------------
EDTelCom_ReadData::EDTelCom_ReadData(const QString &name):
    EDCommand(name)
{
    setFinishType(FT_TIMEOUT);
    setTimeout(50);

    m_numsamples = 0;
    m_status = 0;
    m_samples.clear();
    m_sample_size = 0;
}
//-----------------------------------------------------------------------------------------------------
uint EDTelCom_ReadData::getNumSamples() const
{
    return m_numsamples;
}
//-----------------------------------------------------------------------------------------------------
uchar EDTelCom_ReadData::getStatus() const
{
    return m_status;
}
//-----------------------------------------------------------------------------------------------------
const QByteArray &EDTelCom_ReadData::Samples() const
{
    return m_samples;
}
//-----------------------------------------------------------------------------------------------------
void EDTelCom_ReadData::get_tx_data(QByteArray &data)
{
    data.clear();

    data.resize(2);

    data[0] = 0x04;
    data[1] = 0x04;
}
//-----------------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDTelCom_ReadData::process_rx_data(const QByteArray &data)
{
    uint size = data.size();

    if(size < 3) return CR_Waiting;

	uint num_samples = static_cast<uchar>(data[0]) | (static_cast<uchar>(data[1]) << 8);
    uchar status = data[2];

    if( m_sample_size*num_samples > (size - 3) ) return CR_Overflow;
    if( m_sample_size*num_samples < (size - 3) ) return CR_Waiting;

    m_status = status;
    m_numsamples = num_samples;
    m_samples.clear();
    m_samples.append(data);
    m_samples.remove(0,3);

    return CR_Success;
}
//-----------------------------------------------------------------------------------------------------
bool EDTelCom_ReadData::isReadyFinish(const QByteArray &data) const
{
	uint num_samples = 0;
	if(data.size()>=3)
		num_samples = static_cast<uchar>(data[1]) | (static_cast<uchar>(data[2]) << 8);

    return data.size() == (m_sample_size*num_samples + 5);
}
//-----------------------------------------------------------------------------------------------------
void EDTelCom_ReadData::setSampleSize(uint value)
{
    m_sample_size = value;
}
//-----------------------------------------------------------------------------------------------------
uint EDTelCom_ReadData::getSampleSize() const
{
    return m_sample_size;
}


