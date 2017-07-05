#include "EDSigCom_ReadValue.h"

//-------------------------------------------------------------------------------------------------
EDSigCom_ReadValue::EDSigCom_ReadValue(const QString &name):
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

    m_value_ptr = nullptr;

    setFinishType(FT_TIMEOUT);
    setTimeout(10);
}
//-------------------------------------------------------------------------------------------------
void EDSigCom_ReadValue::get_tx_data(QByteArray &data)
{
    data.clear();

    data.resize(4);

    data[0] = 0x03;
    data[1] = 0x05;
    data[2] = m_signal_id;
    data[3] = m_signal_id >> 8;
}
//-------------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDSigCom_ReadValue::process_rx_data(const QByteArray &data)
{
    char *sptr = static_cast<char*>(m_value_ptr);
    int size = data.size();

    switch(m_descriptor.type_class)
    {
    case SCL_STD:
        switch(m_descriptor.type)
        {
        case STYPE_STRING:
            for(int i = 0; i < size; i++)
            {
                *sptr = data.at(i);
                sptr++;
            }
            *sptr = 0;//zero-ending - %%% патч того что при чтении не передается завершающий ноль
            return CR_Success;
            break;

        case STYPE_BOOL:
        case STYPE_BYTE:
        case STYPE_UINT:
        case STYPE_INT:
        case STYPE_LONG:
        case STYPE_ULONG:
        case STYPE_FLOAT:
            if(size < getRxDataSize()) return CR_Waiting;
            if(data.size() != getRxDataSize()) return CR_BadData;
            for(int i = 0; i < size; i++)
            {
                *sptr = data.at(i);
                sptr++;
            }
            return CR_Success;
            break;

        default:
            return CR_Unknown;
            break;
        }
    break;

    case SCL_ENUM:
        // not yet implemented
        return CR_Unknown;
        break;

    case SCL_ARRAY1:
        // not yet implemented
        return CR_Unknown;
        break;

    case SCL_ARRAY2:
        // not yet implemented
        return CR_Unknown;
        break;

    default:
        return CR_Unknown;
        break;
    }
}
//-------------------------------------------------------------------------------------------------
void EDSigCom_ReadValue::setSignalID(uint value)
{
    m_signal_id = value;
}
//-------------------------------------------------------------------------------------------------
uint EDSigCom_ReadValue::getSignalID() const
{
    return m_signal_id;
}
//-------------------------------------------------------------------------------------------------
void EDSigCom_ReadValue::setDescriptor(const EDSignalDescriptor &value)
{
    m_descriptor = value;

    switch(m_descriptor.type_class)
    {
    case SCL_STD:
        switch(m_descriptor.type)
        {
        case STYPE_STRING:
                setFinishType(FT_ZERO_ENDING);
            break;

        case STYPE_BOOL:
        case STYPE_BYTE:
                setFinishType(FT_FIXED_DATA);
                setRxDataSize(1);
            break;

        case STYPE_UINT:
        case STYPE_INT:
                setFinishType(FT_FIXED_DATA);
                setRxDataSize(2);
            break;

        case STYPE_LONG:
        case STYPE_ULONG:
        case STYPE_FLOAT:
                setFinishType(FT_FIXED_DATA);
                setRxDataSize(4);
            break;
        }
    break;

    case SCL_ENUM:
        // not yet implemented
        break;

    case SCL_ARRAY1:
        // not yet implemented
        break;

    case SCL_ARRAY2:
        // not yet implemented
        break;
    }
}
//-------------------------------------------------------------------------------------------------
const EDSignalDescriptor &EDSigCom_ReadValue::getDescriptor() const
{
    return m_descriptor;
}
//-------------------------------------------------------------------------------------------------
void EDSigCom_ReadValue::setValuePtr(void *value)
{
    m_value_ptr = value;
}



