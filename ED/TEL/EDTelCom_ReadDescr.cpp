#include "EDTelCom_ReadDescr.h"

//-------------------------------------------------------------------------------------------------
EDTelCom_ReadDescr::EDTelCom_ReadDescr(const QString &name):
    EDCommand(name)
{
    setFinishType(FT_FIXED_DATA);
    setTimeout(1000);
    setRxDataSize(12);

    m_descriptor.period             = 0;
    m_descriptor.max_num_signals    = 0;
    m_descriptor.max_frame_size     = 0;
    m_descriptor.en_stream          = false;
    m_descriptor.en_frames          = false;
    m_descriptor.en_buffering       = false;
}
//-------------------------------------------------------------------------------------------------
const EDTelDescriptor &EDTelCom_ReadDescr::Descriptor() const
{
    return m_descriptor;
}
//-------------------------------------------------------------------------------------------------
void EDTelCom_ReadDescr::get_tx_data(QByteArray &data)
{
    data.clear();

    data.resize(2);

    data[0] = 0x04;
    data[1] = 0x01;
}
//-------------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDTelCom_ReadDescr::process_rx_data(const QByteArray &data)
{
    const int fixed_data_size = 12;
    int size = data.size();
    int pos = 0;

    if( size < fixed_data_size ) return CR_Waiting;
    if(size > fixed_data_size) return CR_BadData;

    //period
    m_descriptor.period =	((static_cast<uint>(data[pos + 3]) & 0xFF) << (3 * 8)) |
							((static_cast<uint>(data[pos + 2]) & 0xFF) << (2 * 8)) |
							((static_cast<uint>(data[pos + 1]) & 0xFF) << (1 * 8)) |
							((static_cast<uint>(data[pos + 0]) & 0xFF) << (0 * 8));
    pos += 4;

    //max_num_signals
	m_descriptor.max_num_signals = (static_cast<uint>(data[pos + 1]) << (1 * 8)) | (static_cast<uint>(data[pos + 0]) << (0 * 8));
    pos += 2;

    //max_frame_size
	m_descriptor.max_frame_size = (static_cast<uint>(data[pos + 1]) << (1 * 8)) | (static_cast<uint>(data[pos + 0]) << (0 * 8));
    pos += 2;

    //en_stream, en_frames, en_buffering
    uchar tmp = data[pos];
    m_descriptor.en_stream          = ( (tmp & 0x01) != 0 );
    m_descriptor.en_frames          = ( (tmp & 0x02) != 0 );
    m_descriptor.en_buffering       = ( (tmp & 0x04) != 0 );
    pos += 4;

    return CR_Success;
}



