#ifndef EDPRGGRP_H
#define EDPRGGRP_H

#include "../EDCommandGroup.h"
#include "../SYS/EDSysGrp.h"

#include "EDPrgCom_FinishProg.h"
#include "EDPrgCom_PrepForProg.h"
#include "EDPrgCom_Read.h"
#include "EDPrgCom_ReadDescr.h"
#include "EDPrgCom_ReadID.h"
#include "EDPrgCom_ReqDevNum.h"
#include "EDPrgCom_Restart.h"
#include "EDPrgCom_Write.h"
#include "EDFirmware.h"
#include "EDPrgProgress.h"

typedef struct
{
    bool FinishProg;
    bool PrepForProg;
    bool Read;
    bool ReadDescr;
    bool ReadID;
    bool ReqDevNum;
    bool Restart;
    bool Write;
} EDPrgGrp_Supported;

// interface to Program group
class EDPrg
{
public:
    virtual const EDPrgGrp_Supported &SuppotedCommand() const = 0;
    virtual bool Available() const = 0;
    virtual const QList<EDDeviceDescriptor> &Descriptors() const = 0;

    virtual bool Erase  (char device_id) = 0;
    virtual bool Write  (char device_id, const EDFirmware &firmware,                EDPrgProgress &progress) = 0;
    virtual bool ReadAll(char device_id, EDFirmware &firmware,                      EDPrgProgress &progress) = 0;
    virtual bool Read   (char device_id, EDFirmware &firmware, uint size_to_read,   EDPrgProgress &progress) = 0;
    virtual bool Restart(char device_id) = 0;
};

class EDPrgGrp : public EDCommandGroup, public EDPrg
{
public:
    explicit EDPrgGrp(EDCommandDispatcher *dispatcher, EDSysGrp *sysgrp);
    ~EDPrgGrp();

    virtual void Scan();
    virtual void ReInit();
    virtual const EDPrgGrp_Supported &SuppotedCommand() const;
    virtual bool Available() const;
    virtual const QList<EDDeviceDescriptor> &Descriptors() const;

    virtual bool Erase  (char device_id);
    virtual bool Write  (char device_id, const EDFirmware &firmware,                EDPrgProgress &progress);
    virtual bool ReadAll(char device_id, EDFirmware &firmware,                      EDPrgProgress &progress);
    virtual bool Read   (char device_id, EDFirmware &firmware, uint size_to_read,   EDPrgProgress &progress);
    virtual bool Restart(char device_id);

private:
    bool PrepForProg(char device_id, bool erase);
    bool ReqDevNum(char &devnum);
    bool ReadDescr(char device_id, EDDeviceDescriptor &descr);
    bool ReadID(char device_id, uint &id);
    bool FinishProg(char device_id);
    bool Read(char device_id, QByteArray &data);
    bool Write(char device_id, const QByteArray &data);

    EDSysGrp *m_sysgrp;

    EDPrgCom_FinishProg     *com_finishprog;
    EDPrgCom_PrepForProg    *com_prepforprog;
    EDPrgCom_Read           *com_read;
    EDPrgCom_ReadDescr      *com_readdescr;
    EDPrgCom_ReadID         *com_readid;
    EDPrgCom_ReqDevNum      *com_reqdevnum;
    EDPrgCom_Restart        *com_restart;
    EDPrgCom_Write          *com_write;

    EDPrgGrp_Supported m_supported;

    QList<EDDeviceDescriptor> m_descr_list;
};

#endif // EDPRGGRP_H
