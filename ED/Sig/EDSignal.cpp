#include <stdint.h>
#include <QDataStream>
#include "EDSignal.h"
#include "EDSigGrp.h"

//-----------------------------------------------------------------------------------------------------------
QDataStream &operator <<(QDataStream &out, const EDSignalDescriptor &descriptor)
{
    out << descriptor.name
        << static_cast<int>(descriptor.type_class)
        << static_cast<int>(descriptor.type)
        << descriptor.en_ctrl
        << descriptor.is_parent
        << descriptor.en_signature
        << descriptor.en_read
        << descriptor.en_setting
        << descriptor.en_telemetry
        << descriptor.parent_id
        << descriptor.period
        << static_cast<int>(descriptor.measure_type)
        << descriptor.coef;

    return out;
}
//-----------------------------------------------------------------------------------------------------------
QDataStream &operator >>(QDataStream &in, EDSignalDescriptor &descriptor)
{
    int tmp_type_class;
    int tmp_type;
    int tmp_measure_type;

    in  >> descriptor.name
        >> tmp_type_class
        >> tmp_type
        >> descriptor.en_ctrl
        >> descriptor.is_parent
        >> descriptor.en_signature
        >> descriptor.en_read
        >> descriptor.en_setting
        >> descriptor.en_telemetry
        >> descriptor.parent_id
        >> descriptor.period
        >> tmp_measure_type
        >> descriptor.coef;

    descriptor.property = nullptr;

    descriptor.type_class = static_cast<EDSig_Class>(tmp_type_class);
    descriptor.type = static_cast<EDSig_Type>(tmp_type);
	descriptor.measure_type = static_cast<EDSig_Measurement_type>(tmp_measure_type);

    return in;
}
//-----------------------------------------------------------------------------------------------------------
QDataStream &operator <<(QDataStream &out, const EDSignal &signal)
{
    out << signal.getDescriptor()
        << signal.getSignal_id();

    switch(signal.getDescriptor().type)
    {
	case STYPE_STRING:  out << static_cast<char*>(signal.getValue());		break; // zero-terminated string
	case STYPE_BOOL:    out << *static_cast<bool*>(signal.getValue());		break;
	case STYPE_BYTE:    out << *static_cast<int8_t*>(signal.getValue());	break;
	case STYPE_UINT:    out << *static_cast<uint16_t*>(signal.getValue());	break;
	case STYPE_INT:     out << *static_cast<int16_t*>(signal.getValue());	break;
	case STYPE_LONG:    out << *static_cast<int32_t*>(signal.getValue());	break;
	case STYPE_ULONG:   out << *static_cast<uint32_t*>(signal.getValue());	break;
	case STYPE_FLOAT:   out << *static_cast<float*>(signal.getValue());		break;
    }

    out << signal.getSize()
        << signal.getCtrl_mode()
        << signal.getIntelemetry()
        << signal.getCnt_fixed_childs();

    return out;
}
//-----------------------------------------------------------------------------------------------------------
QDataStream &operator >>(QDataStream &in, EDSignal &signal)
{
    char    *tmp_string;
    bool    tmp_bool;
    qint8   tmp_byte;
    quint16 tmp_uint;
    qint16  tmp_int;
    qint32  tmp_long;
    quint32 tmp_ulong;
    float   tmp_float;
    int     tmp_ctrl_mode;

    in >> signal.descriptor
       >> signal.signal_id;

    delete signal.value;
    signal.value = new uchar[EDSignal::getStdSignalSize(signal.descriptor.type)];

    switch(signal.descriptor.type)
    {
        case STYPE_STRING:  in >> tmp_string;                       signal.value = tmp_string;  break; // zero-terminated string
		case STYPE_BOOL:    in >> tmp_bool;     *static_cast<bool*>(signal.value) = tmp_bool;    break;
		case STYPE_BYTE:    in >> tmp_byte;     *static_cast<int8_t*>(signal.value) = tmp_byte;    break;
		case STYPE_UINT:    in >> tmp_uint;     *static_cast<uint16_t*>(signal.value) = tmp_uint;    break;
		case STYPE_INT:     in >> tmp_int;      *static_cast<int16_t*>(signal.value) = tmp_int;     break;
		case STYPE_LONG:    in >> tmp_long;     *static_cast<int32_t*>(signal.value) = tmp_long;    break;
		case STYPE_ULONG:   in >> tmp_ulong;    *static_cast<uint32_t*>(signal.value) = tmp_ulong;   break;
		case STYPE_FLOAT:   in >> tmp_float;    *static_cast<float*>(signal.value) = tmp_float;   break;
    }

    in  >> signal.size
        >> tmp_ctrl_mode
        >> signal.intelemetry
        >> signal.cnt_fixed_childs;

	signal.ctrl_mode = static_cast<EDSigCtrl_Mode>(tmp_ctrl_mode);

    return in;
}
//---------------------------------------------------------------------------------------------------
EDSignal::EDSignal():
    value(nullptr)
{
}
//---------------------------------------------------------------------------------------------------
uint EDSignal::getStdSignalSize(EDSig_Type type_class)
{
    uint res = 0;

    switch(type_class)
    {
    case STYPE_STRING:  res = 256; break;
    case STYPE_BOOL:    res = 1; break;
    case STYPE_BYTE:    res = 1; break;
    case STYPE_UINT:
    case STYPE_INT:     res = 2; break;
    case STYPE_LONG:
    case STYPE_ULONG:
    case STYPE_FLOAT:   res = 4; break;
    default:            res = 0; break;
    }

    return res;
}
//-----------------------------------------------------------------------------------------------------------
QString EDSignal::SignalValueToString(const EDSignal &sig, bool hex)
{
    int prec = (hex)?16:10;
    QString result;

    switch(sig.descriptor.type)
    {
    case STYPE_STRING:
        result = static_cast<char*>(sig.value);
        break;

    case STYPE_BOOL:
        if(*(static_cast<bool*>(sig.value))) result = "true";
        else result = "false";
        break;

    case STYPE_BYTE:
        result = QString::number(*static_cast<uint8_t*>(sig.value), prec);
        break;

    case STYPE_UINT:
        result = QString::number(*static_cast<uint16_t*>(sig.value), prec);
        break;

    case STYPE_INT:
        result = QString::number(*static_cast<int16_t*>(sig.value), prec);
        break;

    case STYPE_LONG:
        result = QString::number(*static_cast<int32_t*>(sig.value), prec);
        break;

    case STYPE_ULONG:
        result = QString::number(*static_cast<uint32_t*>(sig.value), prec);
        break;

    case STYPE_FLOAT:
        result = QString::number(*static_cast<float*>(sig.value), 'g');
        break;

    default:
        result.clear();
        break;
    }

    return result;
}
//-----------------------------------------------------------------------------------------------------------
const EDSignalDescriptor &EDSignal::getDescriptor() const
{
    return descriptor;
}
//-----------------------------------------------------------------------------------------------------------
int EDSignal::getSignal_id() const
{
    return signal_id;
}
//-----------------------------------------------------------------------------------------------------------
void *EDSignal::getValue() const
{
    return value;
}
//-----------------------------------------------------------------------------------------------------------
uint EDSignal::getSize() const
{
    return size;
}
//-----------------------------------------------------------------------------------------------------------
EDSigCtrl_Mode EDSignal::getCtrl_mode() const
{
    return ctrl_mode;
}
//-----------------------------------------------------------------------------------------------------------
bool EDSignal::getIntelemetry() const
{
    return intelemetry;
}
//-----------------------------------------------------------------------------------------------------------
const void *EDSignal::getUser_data() const
{
    return user_data;
}
//-----------------------------------------------------------------------------------------------------------
void EDSignal::setUser_data(void *data)
{
    if(value != nullptr) user_data = data;
    else throw "Trying to set nullptr pointer as user data";
}
//-----------------------------------------------------------------------------------------------------------
int EDSignal::getCnt_fixed_childs() const
{
    return cnt_fixed_childs;
}
//-----------------------------------------------------------------------------------------------------------
bool EDSignal::isParent() const
{
    return isparent;
}
//-----------------------------------------------------------------------------------------------------------
const QString &EDSignal::getFullName() const
{
    return fullname;
}
