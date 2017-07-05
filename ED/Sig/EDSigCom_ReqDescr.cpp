#include "EDSigCom_ReqDescr.h"

//----------------------------------------------------------------------------------
EDSigCom_ReqDescr::EDSigCom_ReqDescr(const QString &name):
    EDCommand(name)
{
    setFinishType(FT_TIMEOUT);
    setTimeout(10);
    setRxDataSize(sizeof(EDSignalDescriptor));

    m_descriptor.name.clear();
    m_descriptor.type_class     = SCL_STD;
    m_descriptor.type           = STYPE_INT;
    m_descriptor.en_ctrl        = false;
    m_descriptor.is_parent      = false;
    m_descriptor.en_signature   = false;
    m_descriptor.en_read        = false;
    m_descriptor.en_setting     = false;
    m_descriptor.en_telemetry   = false;
    m_descriptor.parent_id      = -1;
    m_descriptor.period         = 0;
    m_descriptor.measure_type   = SMT_NONE;
    m_descriptor.coef           = 1.0;
    m_descriptor.property       = nullptr;
}
//----------------------------------------------------------------------------------
void EDSigCom_ReqDescr::setSignalID(uint value)
{
    m_signal_id = value;
}
//----------------------------------------------------------------------------------
uint EDSigCom_ReqDescr::getSignalID() const
{
    return m_signal_id;
}
//----------------------------------------------------------------------------------
const EDSignalDescriptor &EDSigCom_ReqDescr::Descriptor() const
{
    return m_descriptor;
}
//----------------------------------------------------------------------------------
void EDSigCom_ReqDescr::get_tx_data(QByteArray &data)
{
    data.clear();

    data.resize(4);

    data[0] = 0x03;
    data[1] = 0x02;
    data[2] = m_signal_id;
    data[3] = m_signal_id >> 8;
}
//----------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDSigCom_ReqDescr::process_rx_data(const QByteArray &data)
{
    const int fixed_data_size = 17;
    int size = data.size();
    int pos = 0;

    if( size < (fixed_data_size + 2) ) return CR_Waiting;

    // name
    m_descriptor.name.clear();
    for(int i = 0; i < size; i++)
    {
        if( ((size-1) - pos) < fixed_data_size ) return CR_BadData;
        if(data[pos] != 0) m_descriptor.name.append(data[pos]);
        else
        {
            ++pos;
            break;
        }
        ++pos;
    }

    if( (size - pos) != fixed_data_size) return CR_BadData;

    // type_class
    m_descriptor.type_class = static_cast<EDSig_Class>(data[pos]);
    ++pos;

    // type
    //m_descriptor.type = static_cast<EDSig_Type>(data[pos]);
    switch(data[pos])
    {
        case 0: m_descriptor.type = STYPE_STRING; break;
        case 1: m_descriptor.type = STYPE_BOOL; break;
        case 2: m_descriptor.type = STYPE_BYTE; break;
        case 3: m_descriptor.type = STYPE_UINT; break;
        case 4: m_descriptor.type = STYPE_INT; break;
        case 5: m_descriptor.type = STYPE_LONG; break;
        case 6: m_descriptor.type = STYPE_ULONG; break;
        case 7: m_descriptor.type = STYPE_FLOAT; break;
    }
    ++pos;

    // en_ctrl, is_parent, en_signature, en_read, en_setting, en_telemetry
    uchar tmp = data[pos];
    m_descriptor.en_ctrl        = ((tmp & 0x01) != 0);
    m_descriptor.is_parent      = ((tmp & 0x02) != 0);
    m_descriptor.en_signature   = ((tmp & 0x04) != 0);
    m_descriptor.en_read        = ((tmp & 0x08) != 0);
    m_descriptor.en_setting     = ((tmp & 0x10) != 0);
    m_descriptor.en_telemetry   = ((tmp & 0x20) != 0);
    pos += 4;

    // parent_id
    m_descriptor.parent_id = static_cast<uchar>(data[pos]) | ( (static_cast<uchar>(data[pos + 1])) << 8 );
    if( (m_descriptor.parent_id & 0x8000) != 0) m_descriptor.parent_id = -1;
    pos += 2;

    // period
    m_descriptor.period = ((static_cast<uint>(data[pos + 3]) & 0xFF) << (3 * 8)) |
						  ((static_cast<uint>(data[pos + 2]) & 0xFF) << (2 * 8)) |
                          ((static_cast<uint>(data[pos + 1]) & 0xFF) << (1 * 8)) |
                          ((static_cast<uint>(data[pos + 0]) & 0xFF) << (0 * 8));
    pos += 4;

    // measure_type
    m_descriptor.measure_type = static_cast<EDSig_Measurement_type>(data[pos]);
    ++pos;

    // coef
    volatile long_float_t tmp_coef;
    tmp_coef._u32 =  ((static_cast<uchar>(data[pos + 3]) & 0xFF) << (3 * 8)) |
					 ((static_cast<uchar>(data[pos + 2]) & 0xFF) << (2 * 8)) |
					 ((static_cast<uchar>(data[pos + 1]) & 0xFF) << (1 * 8)) |
					 ((static_cast<uchar>(data[pos + 0]) & 0xFF) << (0 * 8));
    m_descriptor.coef = tmp_coef._float;
    pos += 4;

    // property - not yet implemented

    if( size != pos ) return CR_BadData;

    return CR_Success;
}



