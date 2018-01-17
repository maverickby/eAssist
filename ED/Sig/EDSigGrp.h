#pragma once
#ifndef EDSIGGRP_H
#define EDSIGGRP_H

#include "../EDCommandGroup.h"
#include "../SYS/EDSysGrp.h"
#include "EDSigCom_Ctrl.h"
#include "EDSigCom_ReadValue.h"
#include "EDSigCom_ReqDescr.h"
#include "EDSigCom_ReqNum.h"
#include "EDSigCom_SetSignature.h"

typedef struct
{
    bool Ctrl;
    bool ReadValue;
    bool ReqDescr;
    bool ReqNum;
    bool SetSignature;
} EDSigGrp_Supported;

class EDSigGrp : public EDCommandGroup
{
public:
    explicit EDSigGrp(EDCommandDispatcher *dispatcher, EDSysGrp *sysgrp);
    ~EDSigGrp();

    virtual void Scan();
    virtual void ReInit();
    const EDSigGrp_Supported &SuppotedCommand() const;
    bool Available() const;

    void setTimeoutDescriptor(uint32_t value);

    bool ReqDescr(uint signal_id, EDSignalDescriptor &descriptor);
    bool ReqNum(uint &num);

    bool setSignal(const EDSignal &signal);
    bool getSignal(EDSignal &signal);
    bool getSignal_str(EDSignal &signal, QString &value, bool hex = false);
	inline EDSigCom_ReqNum *getCom_reqnum() {return com_reqnum;	}

private:
    bool SetSignature(uint signal_id, uint coef,  const QList<void*> &list, const EDSignalDescriptor &descriptor);// NOT YET IMPLEMENTED

    EDSigCom_Ctrl           *com_ctrl;
    EDSigCom_ReadValue      *com_readvalue;
    EDSigCom_ReqDescr       *com_reqdescr;
    EDSigCom_ReqNum         *com_reqnum;
    EDSigCom_SetSignature   *com_setsignature;

    EDSysGrp *m_sysgrp;

    EDSigGrp_Supported m_supported;
};

#endif // EDSIGGRP_H
