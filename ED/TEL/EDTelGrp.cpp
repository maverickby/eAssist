#include "EDTelGrp.h"
#include "../EDLogger.h"

//------------------------------------------------------------------------------------------------
EDTelGrp::EDTelGrp(EDCommandDispatcher *dispatcher, EDSysGrp *sysgrp, EDSignalsList *siglist):
    EDCommandGroup(dispatcher)
{
    m_supported.AddSignal   = false;
    m_supported.ReadData    = false;
    m_supported.ReadDescr   = false;
    m_supported.SetMode     = false;

    m_sysgrp = sysgrp;
    m_siglist = siglist;

    com_addsignal = new EDTelCom_AddSignal      ("AddSignal");
    com_readdata =  new EDTelCom_ReadData       ("ReadData");
    com_readdescr = new EDTelCom_ReadDescr      ("ReadDescr");
    com_setmode =   new EDTelCom_SetMode        ("SetMode");

    m_sample_size = 0;

    m_descriptor.period             = 0;
    m_descriptor.max_num_signals    = 0;
    m_descriptor.max_frame_size     = 0;
    m_descriptor.en_stream          = false;
    m_descriptor.en_frames          = false;
    m_descriptor.en_buffering       = false;

    m_state = TM_STOP;
}
//------------------------------------------------------------------------------------------------
EDTelGrp::~EDTelGrp()
{
    m_data_list.clear();
    m_data_buffer_list.clear();

    delete com_addsignal;
    delete com_readdata;
    delete com_readdescr;
    delete com_setmode;
}
//------------------------------------------------------------------------------------------------
void EDTelGrp::Scan()
{
    if(m_sysgrp == nullptr) return;
    if(!m_sysgrp->Available())
    {
        logger().WriteLn("System command group not supported. Imposible use telemetry commnads.", Qt::red);
        return;
    }

    if(m_siglist == nullptr) return;
    if(m_siglist->count() == 0)
    {
        logger().WriteLn("Signals list empty. Imposible use telemetry commnads.", Qt::red);
        return;
    }

    logger().WriteLn("******* Scan telemetry commands *******", Qt::darkMagenta);

    m_sysgrp->SetScanMode(true);

        if(m_supported.ReadDescr = read_descr())//ReadDescr
        {
            logger().WriteLn("    period         : " + QString::number(m_descriptor.period),            Qt::gray);
            logger().WriteLn("    max_num_signals: " + QString::number(m_descriptor.max_num_signals),   Qt::gray);
            logger().WriteLn("    max_frame_size : " + QString::number(m_descriptor.max_frame_size),    Qt::gray);
            logger().WriteLn("    en_stream      : " + QString::number(m_descriptor.en_stream),         Qt::gray);
            logger().WriteLn("    en_frames      : " + QString::number(m_descriptor.en_frames),         Qt::gray);
            logger().WriteLn("    en_buffering   : " + QString::number(m_descriptor.en_buffering),      Qt::gray);
            if(m_supported.SetMode =  set_mode(TM_STOP, 1, 0))//SetMode;
            {
                m_supported.AddSignal = AddSignal(0);//AddSignal;
            }
        }

        if(Available())
        {
            //ReadData;
            uchar tmp_status;
            uint tmp_num_samples;
            QByteArray tmp_data;
            m_supported.ReadData = read_data(tmp_status, tmp_num_samples, tmp_data, true);
        } else
        {
             logger().WriteLn("Telemetry imposible: all requered commnads not found", Qt::red);
        }

        m_sysgrp->SetScanMode(false);
}
//------------------------------------------------------------------------------------------------
void EDTelGrp::ReInit()
{
    m_supported.AddSignal   = false;
    m_supported.ReadData    = false;
    m_supported.ReadDescr   = false;
    m_supported.SetMode     = false;

    m_sample_size = 0;

    m_descriptor.period             = 0;
    m_descriptor.max_num_signals    = 0;
    m_descriptor.max_frame_size     = 0;
    m_descriptor.en_stream          = false;
    m_descriptor.en_frames          = false;
    m_descriptor.en_buffering       = false;

    m_state = TM_STOP;
}
//------------------------------------------------------------------------------------------------
const EDTELGrp_Supported &EDTelGrp::SuppotedCommand() const
{
    return m_supported;
}
//------------------------------------------------------------------------------------------------
bool EDTelGrp::Available() const
{
    return  m_supported.AddSignal && m_supported.ReadDescr && m_supported.SetMode;
}
//------------------------------------------------------------------------------------------------
bool EDTelGrp::AddSignal(uint signal_id)
{
    EDSignal *signal = &((*m_siglist)[signal_id]);

    com_addsignal->setSignalID(signal_id);

    logger().Write(signal->getDescriptor().name + ": ", Qt::gray);

    bool res = Run(com_addsignal);
    if(res && !m_sysgrp->isScanMode())
    {
        m_sample_size += signal->getSize();

        uint tmp_size = signal->getSize();
        EDTel_SignalData tmp_signal_data(signal_id, tmp_size);
        EDTel_SignalDataBuffer tmp_signal_data_buffer(signal_id, tmp_size);

        m_data_list.append(tmp_signal_data);
        m_data_buffer_list.append(tmp_signal_data_buffer);
    }

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDTelGrp::ReadData(uchar &status, QList<EDTel_SignalData> **list, bool logging)
{
    uint num_samples;
    uint num_signals = m_data_list.size();
    QByteArray data;

    bool res = read_data(status, num_samples, data, logging);

    if(res)
    {
        // clear and realloc data buffer
        for(int i = 0; i < m_data_list.size(); i++)
        {
            m_data_list[i].ClearSamples();
            m_data_list[i].setNumSamples(num_samples);
        }

        char *raw_data = data.data();
        uint offset = 0;

        // populate with new data
        for(uint k = 0; k < num_signals; k++)
        {
            char *sig_data = m_data_list[k].data();
            uint sig_size  = m_data_list[k].getValueSize();

            for(uint i = 0; i < num_samples; i++)
            {
                memcpy(&sig_data[i*sig_size], &raw_data[i*m_sample_size + offset], sig_size);
            }

            offset += sig_size;
        }
    }

    *list = &m_data_list;

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDTelGrp::ReadDataToBuffer(TelStatistics &statistics, bool logging)
{
    uint num_samples = 0;
    uint num_signals = m_data_buffer_list.size();
    QByteArray data;
    uchar status;
    bool status_ok;
    bool status_overflow;
    bool status_err1;
    bool status_err2;
    bool status_err3;
    bool status_err4;
    bool status_wait;
    bool status_frame_err;

    bool res = read_data(status, num_samples, data, logging);

    if(res)
    {
        status_ok        = (status & TEL_STATUS_OK) != 0;
        status_overflow  = (status & TEL_STATUS_OVERFLOW) != 0;
        status_err1      = (status & TEL_STATUS_ERR1) != 0;
        status_err2      = (status & TEL_STATUS_ERR2) != 0;
        status_err3      = (status & TEL_STATUS_ERR3) != 0;
        status_err4      = (status & TEL_STATUS_ERR4) != 0;
        status_wait      = (status & TEL_STATUS_WAIT) != 0;
        status_frame_err = (status & TEL_STATUS_FRAME_ERR) != 0;

		if (status_ok)
		{
			statistics.datarequests++;
            if (statistics.num_samples < static_cast<int>(num_samples)) statistics.num_samples = num_samples;
		}
        if(status_overflow) statistics.state_overflow++;
        if(status_err1 || status_err2 || status_err3 || status_err4) statistics.errors++;
        if(status_wait) statistics.state_wait++;

        char *raw_data = data.data();
        uint offset = 0;

        // populate with new data
        for(uint k = 0; k < num_signals; k++)
        {
            uint sig_size  = m_data_buffer_list[k].getValueSize();

            for(uint i = 0; i < num_samples; i++)
            {
                m_data_buffer_list[k].data().append(&raw_data[i*m_sample_size + offset], sig_size);
            }

            offset += sig_size;
        }
    } else
    {
        statistics.errors++;
    }

    return res;
}
//------------------------------------------------------------------------------------------------
void EDTelGrp::ClearReadDataBuffer()
{
    uint num_signals = m_data_buffer_list.size();

    for(uint i = 0; i < num_signals; i++) m_data_buffer_list[i].ClearSamples();
}
//------------------------------------------------------------------------------------------------
QList<EDTel_SignalDataBuffer> &EDTelGrp::getDataBuffer()
{
    return m_data_buffer_list;
}
//------------------------------------------------------------------------------------------------
EDTelDescriptor EDTelGrp::Descriptor() const
{
    return m_descriptor;
}
//------------------------------------------------------------------------------------------------
bool EDTelGrp::Start(ulong coef)
{
    logger().Write("Start: ", Qt::gray);

    m_dispatcher->setActive(true);

    bool res = set_mode(TM_BUFFERING, coef, 0);

    if(res)
    {
        m_state = TM_BUFFERING;
        m_dispatcher->setActive(true);
    } else
        m_dispatcher->setActive(false);

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDTelGrp::Stop()
{
    logger().Write("Stop: ", Qt::gray);

    bool res = set_mode(TM_STOP, 1, 0);

    m_dispatcher->setActive(false);

    if(res)
    {
        m_data_list.clear();
        m_data_buffer_list.clear();
        m_state = TM_STOP;
        m_sample_size = 0;
        m_dispatcher->setActive(false);
    }

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDTelGrp::Pause()
{
    bool res = set_mode(TM_PAUSE, 1, 0);

    m_dispatcher->setActive(false);

    if(res)
    {
        m_state = TM_PAUSE;
        logger().Write("Pause...", Qt::gray);
    }

    return res;
}
//------------------------------------------------------------------------------------------------
EDTelMode EDTelGrp::getState() const
{
    return m_state;
}
//------------------------------------------------------------------------------------------------
bool EDTelGrp::read_data(uchar &status, uint &num_samples, QByteArray &data, bool logging)
{
    com_readdata->setSampleSize(m_sample_size);

    bool res;

    res = Run(com_readdata, logging);

    if(res)
    {
        status = com_readdata->getStatus();
        num_samples = com_readdata->getNumSamples();
        data = com_readdata->Samples();
    }

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDTelGrp::read_descr()
{
    bool res = Run(com_readdescr);

    if(res)
    {
        m_descriptor = com_readdescr->Descriptor();
    }

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDTelGrp::set_mode(EDTelMode mode, ulong coef, uint frame_size)
{
    com_setmode->setMode(mode);
    com_setmode->setDivCoef(coef);
    com_setmode->setFrameSize(frame_size);

    bool res = Run(com_setmode);

    return res;
}

