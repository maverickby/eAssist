#include "EDSigCom_SetSignature.h"

//----------------------------------------------------------------------------------------------
EDSigCom_SetSignature::EDSigCom_SetSignature(const QString &name):
    EDCommand(name)
{
    m_signal_id = -1;

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

    m_values_list.clear();

    setFinishType(FT_NODATA);
    setTimeout(1000);
}
//----------------------------------------------------------------------------------------------
void EDSigCom_SetSignature::setSignalID(uint value)
{
    m_signal_id = value;
}
//----------------------------------------------------------------------------------------------
uint EDSigCom_SetSignature::getSignalID() const
{
    return m_signal_id;
}
//----------------------------------------------------------------------------------------------
void EDSigCom_SetSignature::setDescriptor(const EDSignalDescriptor &value)
{
    m_descriptor = value;
}
//----------------------------------------------------------------------------------------------
const EDSignalDescriptor &EDSigCom_SetSignature::getDescriptor() const
{
    return m_descriptor;
}
//----------------------------------------------------------------------------------------------
void EDSigCom_SetSignature::setValuesList(const QList<void *> &list)
{
    m_values_list = list;
}
//----------------------------------------------------------------------------------------------
void EDSigCom_SetSignature::setCoef(uint value)
{
    m_coef = value;
}
//----------------------------------------------------------------------------------------------
uint EDSigCom_SetSignature::getCoef() const
{
    return m_coef;
}
//----------------------------------------------------------------------------------------------
void EDSigCom_SetSignature::get_tx_data(QByteArray & data)
{
    data.clear();
    uint size = m_values_list.size();

    data.resize(8);

    data[0] = 0x03;
    data[1] = 0x04;
    data[2] = m_signal_id;
    data[3] = m_signal_id >> 8;
    data[4] = size;
    data[5] = size >> 8;
    data[4] = m_coef;
    data[5] = m_coef >> 8;

    for(int i = 0; i < 32; i++) data.append("\0"); // not yet imlemented

    data.append("\0"); // zero-ending
}
//----------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDSigCom_SetSignature::process_rx_data(const QByteArray &data)
{
    if(data.size() == 0) return CR_Success;
    else return CR_BadData;
}



