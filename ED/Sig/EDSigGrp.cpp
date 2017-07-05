#include <math.h>
#include <cstdint>
#include <QCoreApplication>
#include "EDSigGrp.h"
#include "../EDLogger.h"

//---------------------------------------------------------------------------------------------------
EDSigGrp::EDSigGrp(EDCommandDispatcher *dispatcher, EDSysGrp *sysgrp):
    EDCommandGroup(dispatcher)
{
    m_sysgrp = sysgrp;

    com_ctrl =          new EDSigCom_Ctrl           ("Ctrl");
    com_readvalue =     new EDSigCom_ReadValue      ("ReadValue");
    com_reqdescr =      new EDSigCom_ReqDescr       ("ReqDescr");
    com_reqnum =        new EDSigCom_ReqNum         ("ReqNum");
    com_setsignature =  new EDSigCom_SetSignature   ("SetSignature");

    m_supported.Ctrl            = false;
    m_supported.ReadValue       = false;
    m_supported.ReqDescr        = false;
    m_supported.ReqNum          = false;
    m_supported.SetSignature    = false;
}
//---------------------------------------------------------------------------------------------------
EDSigGrp::~EDSigGrp()
{
    delete com_ctrl;
    delete com_readvalue;
    delete com_reqdescr;
    delete com_reqnum;
    delete com_setsignature;
}
//---------------------------------------------------------------------------------------------------
void EDSigGrp::Scan()
{
    uint num = 0;

    if(m_sysgrp == nullptr) return;
    if(!m_sysgrp->Available())
    {
        logger().WriteLn("System command group not supported. Imposible use program commnads.", Qt::red);
        return;
    }

    logger().WriteLn("******* Scan signals commands *******", Qt::darkMagenta);

    m_sysgrp->SetScanMode(true);

    // request number of signals
    if(!(m_supported.ReqNum = ReqNum(num)))
    {
        logger().WriteLn("Command ReqNum not supported. Imposible to continue...", Qt::red);
        m_sysgrp->SetScanMode(false);
        return;
    }
    if(num == 0)
    {
        logger().WriteLn("There is no signals detected ", Qt::red);
        m_sysgrp->SetScanMode(false);
        return;
    }
    else
    {
        logger().WriteLn("Signals number: " + QString::number(num), Qt::blue);
    }

    // read descriptors
    EDSignal signal_tmp;
    signal_tmp.signal_id = 0;
    if( !(m_supported.ReqDescr = ReqDescr(0, signal_tmp.descriptor) ))
    {
        logger().WriteLn("Error while reading signal descriptor", Qt::red);
        m_sysgrp->SetScanMode(false);
        return;
    }

    signal_tmp.value = new char[EDSignal::getStdSignalSize(signal_tmp.descriptor.type)];
    m_supported.ReadValue = getSignal(signal_tmp);

    if(Available())
    {
        m_supported.Ctrl = setSignal(signal_tmp);

        m_supported.SetSignature = false;// NOT YET IMPLEMENTED
    }else
    {
        logger().WriteLn("Signals features imposible: all requered commnads not found", Qt::red);
    }

    delete [] signal_tmp.value;

    m_sysgrp->SetScanMode(false);
}
//---------------------------------------------------------------------------------------------------
void EDSigGrp::ReInit()
{
    m_supported.Ctrl            = false;
    m_supported.ReadValue       = false;
    m_supported.ReqDescr        = false;
    m_supported.ReqNum          = false;
    m_supported.SetSignature    = false;
}
//---------------------------------------------------------------------------------------------------
const EDSigGrp_Supported &EDSigGrp::SuppotedCommand() const
{
    return m_supported;
}
//---------------------------------------------------------------------------------------------------
bool EDSigGrp::Available() const
{
    return m_supported.ReadValue && m_supported.ReqDescr && m_supported.ReqNum;
}
//---------------------------------------------------------------------------------------------------
void EDSigGrp::setTimeoutDescriptor(uint32_t value)
{
    com_reqdescr->setTimeout(value);
}
//---------------------------------------------------------------------------------------------------
bool EDSigGrp::setSignal(const EDSignal &signal)
{
    if(!signal.descriptor.en_ctrl) return false;

    com_ctrl->setSignalID(signal.signal_id);
    com_ctrl->setCtrlMode(signal.ctrl_mode);
    com_ctrl->setDescriptor(signal.descriptor);
    com_ctrl->setValuePtr(signal.value);

    logger().Write(signal.fullname + ":  ", Qt::gray);

    bool res = Run(com_ctrl);

    return res;
}
//---------------------------------------------------------------------------------------------------
bool EDSigGrp::getSignal(EDSignal &signal)
{
    if(!signal.descriptor.en_read) return false;

    com_readvalue->setSignalID(signal.signal_id);
    com_readvalue->setValuePtr(signal.value);
    com_readvalue->setDescriptor(signal.descriptor);

    logger().Write(signal.fullname + ":  ", Qt::gray);

    bool res = Run(com_readvalue);

    return res;
}
//---------------------------------------------------------------------------------------------------
bool EDSigGrp::getSignal_str(EDSignal &signal, QString &value, bool hex)
{
    bool res = false;

    int prec = (hex)?16:10;

    res = getSignal(signal);

    if(res) // successfull read
    {
        switch(signal.descriptor.type)
        {
        case STYPE_STRING:
            value = static_cast<char*>(signal.value);
            break;

        case STYPE_BOOL:
            if(*(static_cast<bool*>(signal.value))) value = "true";
            else value = "false";
            break;

        case STYPE_BYTE:
            value = QString::number(*static_cast<uint8_t*>(signal.value), prec);
            break;

        case STYPE_UINT:
            value = QString::number(*static_cast<uint16_t*>(signal.value), prec);
            break;

        case STYPE_INT:
            value = QString::number(*static_cast<int16_t*>(signal.value), prec);
            break;

        case STYPE_LONG:
            value = QString::number(*static_cast<int32_t*>(signal.value), prec);
            break;

        case STYPE_ULONG:
            value = QString::number(*static_cast<uint32_t*>(signal.value), prec);
            break;

        case STYPE_FLOAT:
            value = QString::number(*static_cast<float*>(signal.value), 'g');
            break;

        default:
            value.clear();
            break;
        }
    }

    return res;
}
//---------------------------------------------------------------------------------------------------
bool EDSigGrp::ReqDescr(uint signal_id, EDSignalDescriptor &descriptor)
{
    com_reqdescr->setSignalID(signal_id);

    bool res = Run(com_reqdescr, false);

    descriptor = com_reqdescr->Descriptor();

    return res;
}
//---------------------------------------------------------------------------------------------------
bool EDSigGrp::ReqNum(uint &num)
{
    bool res = Run(com_reqnum);

    num = com_reqnum->getNumSignals();

    return res;
}
//---------------------------------------------------------------------------------------------------
bool EDSigGrp::SetSignature(uint signal_id, uint coef, const QList<void *> &list, const EDSignalDescriptor &descriptor)
{
    if(!descriptor.en_signature) return false;

    com_setsignature->setSignalID(signal_id);
    com_setsignature->setCoef(coef);
    com_setsignature->setValuesList(list);
    com_setsignature->setDescriptor(descriptor);

    bool res = Run(com_setsignature);

    return res;
}



