#ifndef EDSYSGRP_H
#define EDSYSGRP_H

#include "../EDCommandGroup.h"
#include "EDSysCom_CheckChannel.h"
#include "EDSysCom_ReqDevInfo.h"
#include "EDSysCom_Reset.h"
#include "EDSysCom_SaveSettings.h"
#include "EDSysCom_SelfTest.h"
#include "EDSysCom_SetScanMode.h"
#include "EDSysCom_TestChanel.h"

typedef struct
{
    bool CheckChannel;
    bool DevInfo;
    bool Reset;
    bool SaveSettings;
    bool SelfTest;
    bool SetScanMode;
    bool TestChannel;
} EDSysGrp_Supported;

// interface to System group
class EDSys
{
public:
    virtual const EDSysGrp_Supported &SuppotedCommand() const = 0;
    virtual bool Available() const = 0;

    virtual bool CheckChannel() = 0;
    virtual bool ReqDevInfo(QString &info) = 0;
    virtual bool Reset() = 0;
    virtual bool SetScanMode(bool state) = 0;
    virtual bool isScanMode() const = 0;
    virtual bool SelfTest(QString &result) = 0;
    virtual bool SaveSettings() = 0;
    virtual bool TestChannel(const QByteArray &data) = 0;
};

class EDSysGrp: public EDCommandGroup, public EDSys
{
public:
     EDSysGrp(EDCommandDispatcher *dispatcher);
     ~EDSysGrp();

     virtual void Scan();
     virtual void ReInit();
     virtual const EDSysGrp_Supported &SuppotedCommand() const;
     virtual bool Available() const;

     virtual bool CheckChannel();
     virtual bool ReqDevInfo(QString &info);
     virtual bool Reset();
     virtual bool SetScanMode(bool state);
     virtual bool isScanMode() const;
     virtual bool SelfTest(QString &result);
     virtual bool SaveSettings();
     virtual bool TestChannel(const QByteArray &data);

private:
     EDSysCom_CheckChannel   *com_checkchannel;
     EDSysCom_ReqDevInfo     *com_reqdevinfo;
     EDSysCom_Reset          *com_reset;
     EDSysCom_SaveSettings   *com_savesettings;
     EDSysCom_SelfTest       *com_selftest;
     EDSysCom_SetScanMode    *com_setscanmode;
     EDSysCom_TestChanel     *com_testchanel;

     EDSysGrp_Supported m_supported;

     bool m_scanmode;
};

#endif // EDSYSGRP_H
