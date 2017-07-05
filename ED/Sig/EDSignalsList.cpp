#include <QCoreApplication>
#include <cstdint>
#include <QStringList>
#include "EDSignalsList.h"
#include "../EDLogger.h"
#include "EDSignal.h"

static const char SIG_DELIMITER = '.';

//-----------------------------------------------------------------------------------------------------------
EDSignalsList::EDSignalsList(EDSigGrp &siggrp) :
    m_siggrp(siggrp)
{
}
//-----------------------------------------------------------------------------------------------------------
EDSignalsList::~EDSignalsList()
{
    Clear();
}
//-----------------------------------------------------------------------------------------------------------
void EDSignalsList::Clear()
{
    uint size = m_sigvector.size();

    // free memory for signals values
    for(uint i = 0; i < size; i++)
    {
        if( m_sigvector[i].value != nullptr )
            delete m_sigvector[i].value;
    }

    m_sigvector.clear();

    m_numfixedsignals = 0;
}
//-----------------------------------------------------------------------------------------------------------
void EDSignalsList::Populate()
{
    EDSignalDescriptor descr;

    logger().WriteLn("Populating signals list...", Qt::blue);

    Clear();

    uint size;
    m_siggrp.ReqNum(size);
    m_sigvector.reserve(size);

    for(uint i = 0; i < size; i++)
    {
        m_siggrp.ReqDescr(i, descr);
        EDSignal signal;
        signal.descriptor = descr;
        signal.signal_id = i;
        signal.ctrl_mode = CM_NO;
        signal.size = EDSignal::getStdSignalSize(descr.type);
        signal.value = new uchar[signal.size];
            memset(signal.value, 0, signal.size);
        signal.user_data = nullptr;
        signal.cnt_fixed_childs = 0;
        signal.intelemetry = false;
        signal.isparent = false;
        m_sigvector.append(signal);

        logger().setPauseLogging(true);
            m_siggrp.getSignal(signal);// read signal value
        logger().setPauseLogging(false);

        if(i == 0) logger().WritePercent(0, Qt::darkGreen);
        else logger().UpdatePercent(ceil(static_cast<float>(i*100)/size), Qt::darkGreen);

        QCoreApplication::instance()->processEvents();
    }
    logger().WriteLn("", Qt::blue); // new line

    update_parents();
    update_fullnames();

    emit NewSignalsList();
}
//--------------------------------------------------------------------------------------------
void EDSignalsList::update_parents()
{
    uint size = m_sigvector.size();

    for(uint i = 0; i < size; i++)
    {
        int parent_id = m_sigvector.at(i).getDescriptor().parent_id;
        if( parent_id >= 0 )
        {
            m_sigvector[parent_id].isparent = true;
        }
    }
}
//-----------------------------------------------------------------------------------------------------------
void EDSignalsList::update_fullnames()
{
    uint size = m_sigvector.size();

    for(uint i = 0; i < size; i++)
    {
        const EDSignal &signal = m_sigvector.at(i);

        // build string path to signal
        QString path = signal.getDescriptor().name;

        if(signal.getDescriptor().parent_id >= 0) // track over hierarchy
        {
            const EDSignal *parent = &m_sigvector.at( signal.getDescriptor().parent_id );
            bool done = false;
            while(!done)
            {
                if(parent->getDescriptor().parent_id < 0)
                {
                    path = parent->getDescriptor().name + SIG_DELIMITER + path;
                    done = true;
                }
                else
                {
                    path = parent->getDescriptor().name + SIG_DELIMITER + path;
                    parent = &m_sigvector.at( m_sigvector.at(parent->getSignal_id()).getDescriptor().parent_id );
                }
            }
        }

        m_sigvector[i].fullname = path;
    }
}
//-----------------------------------------------------------------------------------------------------------
int EDSignalsList::count() const
{
    return m_sigvector.size();
}
//-----------------------------------------------------------------------------------------------------------
EDSignal &EDSignalsList::operator [](int index)
{
    return m_sigvector[index];
}
//-----------------------------------------------------------------------------------------------------------
const EDSignal &EDSignalsList::at(int index) const
{
    return m_sigvector.at(index);
}
//-----------------------------------------------------------------------------------------------------------
void EDSignalsList::update_cnt_fixed_childs(int parent_id, bool add)
{
    if(parent_id < 0) return;
    EDSignal &sig = m_sigvector[parent_id];
    if(add) sig.cnt_fixed_childs++;
    else if(sig.cnt_fixed_childs > 0) sig.cnt_fixed_childs--;

    if(sig.descriptor.parent_id >= 0) update_cnt_fixed_childs(sig.descriptor.parent_id, add); // climb up on the tree
}
//-----------------------------------------------------------------------------------------------------------
bool EDSignalsList::setSignalValue(EDSignal &signal, const void *value)
{
    memcpy(signal.value, value, signal.size);// copy value to internal signals list

    bool res = m_siggrp.setSignal(signal);

    emit SignalValueChanged(&signal);

    return res;
}
//-----------------------------------------------------------------------------------------------------------
bool EDSignalsList::setSignalValueStr(EDSignal &signal, const QString *value, int base)
{
    QByteArray  tmp_array;
    uint8_t     tmp_uint8;
    uint16_t    tmp_uint16;
    int16_t     tmp_int16;
    int32_t     tmp_int32;
    uint32_t    tmp_uint32;
    float       tmp_float;

    void *pdata = nullptr;

    bool res;
    ushort tmp_ushort;

    switch(signal.getDescriptor().type)
    {
        case STYPE_STRING:
            tmp_array = value->toLatin1();
            pdata = &tmp_array;
            break;

        case STYPE_BYTE:
            tmp_ushort = value->toUShort(&res, base);
            if(res) tmp_uint8 = (tmp_ushort > 255)?255:tmp_ushort;
            pdata = &tmp_uint8;
            break;

        case STYPE_UINT:
            tmp_uint16 = value->toUShort(&res, base);
            pdata = &tmp_uint16;
            break;

        case STYPE_INT:
            tmp_int16 = value->toShort(&res, base);
            pdata = &tmp_int16;
            break;

        case STYPE_LONG:
            tmp_int32 = value->toInt(&res, base);
            pdata = &tmp_int32;
            break;

        case STYPE_ULONG:
            tmp_uint32 = value->toUInt(&res, base);
            pdata = &tmp_uint32;
            break;

        case STYPE_FLOAT:
            tmp_float = value->toFloat(&res);
            pdata = &tmp_float;
            break;
    }
    if(res) res &= setSignalValue(signal, pdata);

    return res;
}
//-----------------------------------------------------------------------------------------------------------
// set signal value (for signal_id from signals in list) to value stored in signal in list
bool EDSignalsList::setSignalValueList(const QList<EDSignal> &list)
{
    int size = list.size();

    bool res = true;

    for(int i = 0; i < size; i++)
    {
        res &= setSignalValue(m_sigvector[list.at(i).getSignal_id()], list.at(i).value);
    }

    return true;
}
//-----------------------------------------------------------------------------------------------------------
bool EDSignalsList::getSignalValue(EDSignal &signal)
{
    bool res = m_siggrp.getSignal(signal);

    emit SignalValueChanged(&signal);

    return res;
}
//-----------------------------------------------------------------------------------------------------------
bool EDSignalsList::getSignalValueStr(EDSignal &signal, QString &value, bool hex)
{
    bool res = m_siggrp.getSignal_str(signal, value, hex);

    emit SignalValueChanged(&signal);

    return res;
}
//-----------------------------------------------------------------------------------------------------------
bool EDSignalsList::FixSignalValue(EDSignal &signal, bool fix)
{
    EDSigCtrl_Mode mode;

    if(fix) mode = CM_FIXED;
    else mode = CM_NO;

    if(fix && (signal.ctrl_mode != CM_FIXED)) m_numfixedsignals++;
    else  if(m_numfixedsignals > 0) m_numfixedsignals--;

    signal.ctrl_mode = mode;
    bool res = m_siggrp.setSignal(signal);

    update_cnt_fixed_childs(signal.descriptor.parent_id, fix);

    emit SignalFixModeChanged(&signal);

    return res;
}
//-----------------------------------------------------------------------------------------------------------
int EDSignalsList::getNumFixedSignals() const
{
    return m_numfixedsignals;
}
//-----------------------------------------------------------------------------------------------------------
void EDSignalsList::setSignalTelMode(EDSignal &signal, bool intelemetry)
{
    signal.intelemetry = intelemetry;
    emit SignalTelModeChanged(&signal);
}
//--------------------------------------------------------------------------------------------
// maps key == full sigal name
// maps value == reference to signal
void EDSignalsList::FillMapWithChilds(QMap<QString, EDSignal> &map, const EDSignal &signal) const
{
    // find childs
    int size = m_sigvector.size();
    for(int i = 0; i < size; i++)
    {
        if(m_sigvector.at(i).getDescriptor().parent_id == signal.getSignal_id())
        {
            if(m_sigvector.at(i).isParent())
                FillMapWithChilds(map, m_sigvector.at(i));
            else
                map.insert(m_sigvector.at(i).getFullName(), m_sigvector.at(i));
        }
    }
}
//--------------------------------------------------------------------------------------------
EDSignal * EDSignalsList::FullnameToSignal(const QString &fullname)
{
    EDSignal *res = nullptr;

    QStringList sl = fullname.split(SIG_DELIMITER);
    const int sl_size = sl.size();

    int size = m_sigvector.size();


    for(int i = 0; i < size; i++)
    {
        if(m_sigvector.at(i).getDescriptor().name == sl.at(sl_size - 1))// match first name
        {
            bool done = false;
            int name_id = sl_size - 2;
            const EDSignal *parent = &m_sigvector.at( m_sigvector.at(i).getDescriptor().parent_id );
            while(!done)
            {
                if(parent->getDescriptor().parent_id < 0) // found last parents node
                {
                    if(parent->getDescriptor().name == sl.at(name_id))
                    {
                        res = &m_sigvector[i];
                    }
                    done = true;
                }
                else
                {
                    if(parent->getDescriptor().name == sl.at(name_id)) // assign new parent
                    {
                        parent = &m_sigvector.at( parent->getDescriptor().parent_id );
                        name_id--;
                    }
                    else done = true;
                }
            }
        }

        if(res != nullptr) break; // no need to continue when signal found
    }

    return res;
}
//--------------------------------------------------------------------------------------------
bool EDSignalsList::VerifySignalNames(const QList<QString> &list)
{
    int size = list.size();

    for(int i = 0; i < size; i++)
    {
        if( FullnameToSignal(list.at(i)) == nullptr ) // not found
            return false;
    }

    return true;
}
//--------------------------------------------------------------------------------------------
// return signal childs in vector childs. Child nodes not returned.
void EDSignalsList::getSignalChilds(const EDSignal &signal, QVector<EDSignal *> &childs)
{
    childs.clear();

    int size = m_sigvector.size();

    if(signal.isParent())
    {
        for(int i = 0; i < size; i++)
        {
            if(!m_sigvector[i].isParent())
            {
                if(m_sigvector[i].getDescriptor().parent_id == signal.getSignal_id()) childs << &m_sigvector[i];
            }
        }
    } else // only one signal selected
    {
        childs << const_cast<EDSignal*>(&signal);
    }
}
