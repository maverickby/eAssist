#ifndef ED_H
#define ED_H

#include <QObject>
#include "EDUdpInterface.h"
#include "EDCommandDispatcher.h"
#include "SYS/EDSysGrp.h"
#include "Prg/EDPrgGrp.h"
#include "TEL/EDTelGrp.h"
#include "Sig/EDSignalsList.h"

class ED : public QObject
{
    Q_OBJECT
public:
    explicit ED(QIODevice *interface, QObject *parent = 0);
    virtual ~ED();

    void setInterface(QIODevice *interface);
    QIODevice *getInterface();
    void setActiveInterface(bool active);

    void setWrapperSettings(bool enabled, unsigned char local_address, unsigned char remote_address);
    void setTimeoutDescriptor(uint32_t value);

    EDSys &Sys();
    EDPrg &Prg();
    EDTel &Tel();
    EDSignalsList &Signals();

    void Scan();

private:
    QIODevice           *m_interface;
    EDCommandDispatcher *m_dispatcher;

    EDSysGrp  *m_sysgrp;
    EDPrgGrp  *m_prggrp;
    EDSigGrp  *m_siggrp;
    EDTelGrp  *m_telgrp;

    EDSignalsList *m_siglist;

signals:
    void NewTeldescriptor();
    void ScanProgress(uint percent);

public slots:

};

#endif // ED_H
