#include "EDSigCom_Ctrl.h"

//---------------------------------------------------------------------------------------
EDSigCom_Ctrl::EDSigCom_Ctrl(const QString &name):
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

    m_ctrl_mode = CM_NO;

    m_value_ptr = nullptr;

    setFinishType(FT_NODATA);
    setTimeout(1000);
}
//---------------------------------------------------------------------------------------
void EDSigCom_Ctrl::get_tx_data(QByteArray &data)
{
    data.clear();
    QByteArray value;
    uchar ctrl_mode;

    data.resize(5);

    data[0] = 0x03;
    data[1] = 0x03;
    data[2] = m_signal_id;
    data[3] = m_signal_id >> 8;

    // control mode
    switch(m_ctrl_mode)
    {
    case CM_NO:         ctrl_mode = 0; break;
    case CM_FIXED:      ctrl_mode = 1; break;
    case CM_SIGNATURE:  ctrl_mode = 2; break;
    }
    data[4] = ctrl_mode;

    // signal value
    char *sptr = static_cast<char*>(m_value_ptr);
    switch(m_descriptor.type_class)
    {
    case SCL_STD:
        switch(m_descriptor.type)
        {
        case STYPE_STRING:
            while(*sptr)
            {
                value.append(*sptr);
                sptr++;
            }
            value.append("\0");// zero-ending string
            break;

        case STYPE_BOOL:
            if(*static_cast<bool*>(m_value_ptr)) value.append(static_cast<char>(1));
            else value.append(static_cast<char>(0));
            break;

        case STYPE_BYTE:
            value.append(sptr, 1);
            break;

        case STYPE_UINT:
		case STYPE_INT:
            value.append(sptr, 2);
            break;

        case STYPE_LONG:
		case STYPE_ULONG:
		case STYPE_FLOAT:
            value.append(sptr, 4);
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

    data.append(value); // attach signal value
}
//---------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDSigCom_Ctrl::process_rx_data(const QByteArray &data)
{
    if(data.size() == 0) return CR_Success;
    else return CR_BadData;
}
//---------------------------------------------------------------------------------------
void EDSigCom_Ctrl::setSignalID(uint value)
{
    m_signal_id = value;
}
//---------------------------------------------------------------------------------------
uint EDSigCom_Ctrl::getSignalID() const
{
    return m_signal_id;
}
//---------------------------------------------------------------------------------------
void EDSigCom_Ctrl::setDescriptor(const EDSignalDescriptor &value)
{
    m_descriptor = value;
}
//---------------------------------------------------------------------------------------
const EDSignalDescriptor &EDSigCom_Ctrl::getDescriptor() const
{
    return m_descriptor;
}
//---------------------------------------------------------------------------------------
void EDSigCom_Ctrl::setCtrlMode(EDSigCtrl_Mode value)
{
    m_ctrl_mode = value;
}
//---------------------------------------------------------------------------------------
EDSigCtrl_Mode EDSigCom_Ctrl::getCtrlMode() const
{
    return m_ctrl_mode;
}
//---------------------------------------------------------------------------------------
void EDSigCom_Ctrl::setValuePtr(void *pvalue)
{
    m_value_ptr = pvalue;
}



