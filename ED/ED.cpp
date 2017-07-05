#include "ED.h"
#include "EDLogger.h"

//---------------------------------------------------------------------------------------------------------
ED::ED(QIODevice *interface, QObject *parent) :
    QObject(parent),
    m_interface(interface)
{
    m_dispatcher = new EDCommandDispatcher(m_interface);

    m_sysgrp     = new EDSysGrp(m_dispatcher);
    m_prggrp     = new EDPrgGrp(m_dispatcher, m_sysgrp);
    m_siggrp     = new EDSigGrp(m_dispatcher, m_sysgrp);
        m_siglist    = new EDSignalsList(*m_siggrp);
    m_telgrp     = new EDTelGrp(m_dispatcher, m_sysgrp, m_siglist);
}
//---------------------------------------------------------------------------------------------------------
ED::~ED()
{
    delete m_sysgrp;
    delete m_prggrp;
    delete m_siglist;
    delete m_siggrp;
    delete m_telgrp;
    delete m_dispatcher;
}
//---------------------------------------------------------------------------------------------------------
void ED::setInterface(QIODevice *interface)
{
    m_interface = interface;
    m_dispatcher->setInterface(interface);
}
//---------------------------------------------------------------------------------------------------------
QIODevice *ED::getInterface()
{
    return m_interface;
}
//---------------------------------------------------------------------------------------------------------
void ED::setActiveInterface(bool active)
{
    m_dispatcher->setActive(active);
}
//---------------------------------------------------------------------------------------------------------
void ED::setWrapperSettings(bool enabled, unsigned char local_address, unsigned char remote_address)
{
    if(enabled) m_dispatcher->Enable_Wrapper();
    else  m_dispatcher->Disable_Wrapper();
    m_dispatcher->setWrpLocalAddress(local_address);
    m_dispatcher->setWrpRemoteAddress(remote_address);
}
//---------------------------------------------------------------------------------------------------------
void ED::setTimeoutDescriptor(uint32_t value)
{
    m_siggrp->setTimeoutDescriptor(value);
}
//---------------------------------------------------------------------------------------------------------
EDSys &ED::Sys()
{
    return *m_sysgrp;
}
//---------------------------------------------------------------------------------------------------------
EDPrg &ED::Prg()
{
    return *m_prggrp;
}
//---------------------------------------------------------------------------------------------------------
EDTel &ED::Tel()
{
    return *m_telgrp;
}
//---------------------------------------------------------------------------------------------------------
EDSignalsList &ED::Signals()
{
    return *m_siglist;
}
//---------------------------------------------------------------------------------------------------------
void ED::Scan()
{
    m_sysgrp->ReInit();
    m_prggrp->ReInit();
    m_siggrp->ReInit();
    m_telgrp->ReInit();
    emit ScanProgress(5);

    m_interface->open(QIODevice::ReadWrite);
        m_sysgrp->Scan();
            emit ScanProgress(10);
        if(m_sysgrp->Available()) m_prggrp->Scan();
            emit ScanProgress(25);
        if(m_sysgrp->Available()) m_siggrp->Scan();
            emit ScanProgress(70);
        if(m_siggrp->Available()) m_siglist->Populate();
            emit ScanProgress(90);
        if(m_sysgrp->Available() && m_siggrp->Available())
        {
            m_telgrp->Scan();
                emit ScanProgress(100);
            if(m_telgrp->Available()) emit(NewTeldescriptor());
        }
    m_interface->close();

    logger().WriteLn("Scan done", Qt::green);
}
