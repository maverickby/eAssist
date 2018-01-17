#ifndef EDTELGRP_H
#define EDTELGRP_H

#include "../EDCommandGroup.h"
#include "../SYS/EDSysGrp.h"
#include "../SIG/EDSignalsList.h"
#include "EDTelCom_AddSignal.h"
#include "EDTelCom_ReadData.h"
#include "EDTelCom_ReadDescr.h"
#include "EDTelCom_SetMode.h"
#include "EDTel_SignalData.h"
#include "EDTel_SignalDataBuffer.h"
//#include "threadtelreaddata.h"

#define TEL_STATUS_OK			0x01
#define TEL_STATUS_OVERFLOW		0x02
#define TEL_STATUS_ERR1			0x04
#define TEL_STATUS_ERR2			0x08
#define TEL_STATUS_ERR3			0x10
#define TEL_STATUS_ERR4			0x20
#define TEL_STATUS_WAIT			0x40
#define TEL_STATUS_FRAME_ERROR	0x80

typedef struct
{
    bool AddSignal;
    bool ReadData;
    bool ReadDescr;
    bool SetMode;
} EDTELGrp_Supported;

typedef struct
{
    int datarequests;
    int num_samples;
    int errors;
    int state_overflow;
    int state_wait;
}   TelStatistics;

class ThreadTelReadData;

// interface to Telemetry group
class EDTel
{
public:
    virtual const EDTELGrp_Supported &SuppotedCommand() const = 0;
    virtual bool Available() const = 0;

    virtual bool AddSignal(uint signal_id) = 0;
    virtual bool ReadData(uchar &status, QList<EDTel_SignalData> **list, bool logging = true) = 0;

    virtual bool ReadDataToBuffer(TelStatistics &statistics, QString& command, EDCommand::EDCommandResult& result, bool logging = true) = 0;
    virtual void ClearReadDataBuffer() = 0;
    virtual QList<EDTel_SignalDataBuffer> &getDataBuffer() = 0;

    virtual EDTelDescriptor Descriptor() const = 0;
    virtual bool Start(ulong coef) = 0;
    virtual bool Stop() = 0;
    virtual bool Pause() = 0;
    virtual EDTelMode getState() const = 0;
};

class EDTelGrp : public EDCommandGroup, public EDTel
{
public:

    explicit EDTelGrp(EDCommandDispatcher *dispatcher, EDSysGrp *sysgrp, EDSignalsList *siglist);
    ~EDTelGrp();

    virtual void Scan();
    virtual void ReInit();
    virtual const EDTELGrp_Supported &SuppotedCommand() const;
    virtual bool Available() const;

    virtual bool AddSignal(uint signal_id);
    virtual bool ReadData(uchar &status, QList<EDTel_SignalData> **list, bool logging = true);

    virtual bool ReadDataToBuffer(TelStatistics &statistics, QString& command, EDCommand::EDCommandResult& result, bool logging = true);
    virtual void ClearReadDataBuffer();
    virtual QList<EDTel_SignalDataBuffer> &getDataBuffer();

    virtual EDTelDescriptor Descriptor() const;
    virtual bool Start(ulong coef);
    virtual bool Stop();
    virtual bool Pause();
    virtual EDTelMode getState() const;

private:
    bool read_data(uchar &status, uint &num_samples, QByteArray &data, QString& command, EDCommand::EDCommandResult& result, bool logging);
    bool read_descr();
    bool set_mode(EDTelMode mode, ulong coef, uint frame_size);

    EDSysGrp        *m_sysgrp;
    EDSignalsList   *m_siglist;

    EDTELGrp_Supported m_supported;

    EDTelCom_AddSignal      *com_addsignal;
    EDTelCom_ReadData       *com_readdata;
    EDTelCom_ReadDescr      *com_readdescr;
    EDTelCom_SetMode        *com_setmode;

    EDTelDescriptor   m_descriptor;
    uint              m_sample_size;

    QList<EDTel_SignalData> m_data_list;
    QList<EDTel_SignalDataBuffer> m_data_buffer_list;

    EDTelMode m_state;
};

#endif // EDTELGRP_H
