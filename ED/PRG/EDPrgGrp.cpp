#include "EDPrgGrp.h"
#include "../EDLogger.h"

//------------------------------------------------------------------------------------------------
EDPrgGrp::EDPrgGrp(EDCommandDispatcher *dispatcher, EDSysGrp *sysgrp):
    EDCommandGroup(dispatcher)
{
    m_sysgrp = sysgrp;

    com_finishprog =    new EDPrgCom_FinishProg     ("FinishProg");
    com_prepforprog =   new EDPrgCom_PrepForProg    ("PrepForProg");
    com_read =          new EDPrgCom_Read           ("Read");
    com_readdescr =     new EDPrgCom_ReadDescr      ("ReadDescr");
    com_readid =        new EDPrgCom_ReadID         ("ReadID");
    com_reqdevnum =     new EDPrgCom_ReqDevNum      ("ReqDevNum");
    com_restart =       new EDPrgCom_Restart        ("Restart");
    com_write =         new EDPrgCom_Write          ("Write");

    m_supported.FinishProg  = false;
    m_supported.PrepForProg = false;
    m_supported.Read        = false;
    m_supported.ReadDescr   = false;
    m_supported.ReadID      = false;
    m_supported.ReqDevNum   = false;
    m_supported.Restart     = false;
    m_supported.Write       = false;

    m_descr_list.clear();
}
//------------------------------------------------------------------------------------------------
EDPrgGrp::~EDPrgGrp()
{
    delete com_finishprog;
    delete com_prepforprog;
    delete com_read;
    delete com_readdescr;
    delete com_readid;
    delete com_reqdevnum;
    delete com_restart;
    delete com_write;
}
//------------------------------------------------------------------------------------------------
void EDPrgGrp::Scan()
{
    char devnum = 0;

    if(m_sysgrp == nullptr) return;
    if(!m_sysgrp->Available())
    {
        logger().WriteLn("System command group not supported. Impossible to use program commands.", Qt::red);
        return;
    }

    logger().WriteLn("******* Scan programming commands *******", Qt::darkMagenta);

    m_sysgrp->SetScanMode(true);

    // request number of devices
    if(!(m_supported.ReqDevNum = ReqDevNum(devnum)))
    {
        logger().WriteLn("Command ReqDevNum not supported. Impossible to continue...", Qt::red);
        m_sysgrp->SetScanMode(false);
        return;
    }
    if(devnum == 0)
    {
        logger().WriteLn("There are no devices detected for program ", Qt::red);
        m_sysgrp->SetScanMode(false);
        return;
    }
    else
    {
        logger().WriteLn("Devices number: " + QString::number(devnum), Qt::blue);
    }

    // read descriptors of all devices
    m_descr_list.clear();
    EDDeviceDescriptor descr;
    logger().WriteLn("Start read devices descriptors", Qt::black);
    for(int i = 0; i < devnum; i++)
    {
        if( !(m_supported.ReadDescr = ReadDescr(i, descr) ))
        {
            logger().WriteLn("Error while reading device descriptor - " + QString::number(i), Qt::red);
            m_sysgrp->SetScanMode(false);
            return;
        }else
        {
            m_descr_list.append(descr);
            logger().WriteLn("Device: " + descr.name, Qt::gray);
            logger().WriteLn("    wr_block_size: " + QString::number(descr.wr_block_size),   Qt::gray);
            logger().WriteLn("    rd_block_size: " + QString::number(descr.rd_block_size),   Qt::gray);
            logger().WriteLn("    prog_size    : " + QString::number(descr.prog_size),       Qt::gray);
            logger().WriteLn("    wr_timeout   : " + QString::number(descr.wr_timeout),      Qt::gray);
            logger().WriteLn("    prep_timeout : " + QString::number(descr.prep_timeout),    Qt::gray);
            logger().WriteLn("    restart      : " + QString::number(descr.restart),         Qt::gray);
        }
    }

    if(m_supported.FinishProg = FinishProg(0))
        if(m_supported.PrepForProg = PrepForProg(0, false))
        {
            QByteArray tmp_rd;
            if(m_supported.Read = Read(0, tmp_rd))
            {
                QByteArray tmp_wr;
                tmp_wr.resize(m_descr_list[0].wr_block_size);
                m_supported.Write = Write(0, tmp_wr);
            }
        }

    if(Available())
    {
        uint tmp = 0;
        m_supported.ReadID = ReadID(0, tmp);
        m_supported.Restart = Restart(0);
    }
    else
    {
        logger().WriteLn("Programming impossible: all required commands not found", Qt::red);
    }

    m_sysgrp->SetScanMode(false);
}
//------------------------------------------------------------------------------------------------
void EDPrgGrp::ReInit()
{
    m_supported.FinishProg  = false;
    m_supported.PrepForProg = false;
    m_supported.Read        = false;
    m_supported.ReadDescr   = false;
    m_supported.ReadID      = false;
    m_supported.ReqDevNum   = false;
    m_supported.Restart     = false;
    m_supported.Write       = false;
}
//------------------------------------------------------------------------------------------------
const EDPrgGrp_Supported &EDPrgGrp::SuppotedCommand() const
{
    return m_supported;
}
//------------------------------------------------------------------------------------------------
bool EDPrgGrp::Available() const
{
    return m_supported.ReadDescr && m_supported.FinishProg && m_supported.PrepForProg && m_supported.Read &&
            m_supported.ReqDevNum && m_supported.Write;
}
//------------------------------------------------------------------------------------------------
const QList<EDDeviceDescriptor> &EDPrgGrp::Descriptors() const
{
    return m_descr_list;
}
//------------------------------------------------------------------------------------------------
bool EDPrgGrp::Erase(char device_id)
{
    logger().Write("Erasing...", Qt::black);
    logger().setPauseLogging(true);
        bool res = PrepForProg(device_id, true);
    logger().setPauseLogging(false);
    if(res) logger().WriteLn("OK", Qt::green);
    else logger().WriteLn("Error", Qt::red);

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDPrgGrp::Write(char device_id, const EDFirmware &firmware, EDPrgProgress &progress)
{
    uint firmware_size = firmware.getData().size();
    bool res = true;

    if(firmware_size == 0)
    {
        logger().WriteLnMessageCritical("Firmware data is empty/ Impossible to write it.", Qt::red);
        return false;
    }

    EDDeviceDescriptor descr = m_descr_list[device_id];

    if(descr.prog_size < firmware_size)
    {
        logger().WriteLnMessageCritical("Firmware size is bigger than specified in descriptor.", Qt::red);
        return false;
    }

    logger().Write("Writing firmware...", Qt::black);

    QByteArray tmp;
    tmp.resize(descr.wr_block_size);
    uint size = 0;

    progress.setProgressParam(0, firmware_size);
    progress.setProgressVisible(true);
    progress.setProgressValue(0);

    logger().setPauseLogging(true);

    if( !PrepForProg(device_id, false) ) return false;

        m_dispatcher->setActive(true);
            while(size < firmware_size)
            {
                for(uint i = 0; i < descr.wr_block_size; i++)
                {
                    if(size <= (firmware_size - 1) ) tmp[i] = firmware.getData().at(size);
                    else tmp[i] = 0;
                    size++;
                }
                if( !(res &= Write(device_id, tmp)) ) break;
                progress.setProgressValue(size);
            }
        m_dispatcher->setActive(false);

        progress.setProgressVisible(false);

    FinishProg(device_id);

    logger().setPauseLogging(false);

    if(res) logger().WriteLn("OK", Qt::green);
    else logger().WriteLn("Error", Qt::red);

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDPrgGrp::ReadAll(char device_id, EDFirmware &firmware, EDPrgProgress &progress)
{
    bool res = true;

    EDDeviceDescriptor descr = m_descr_list[device_id];

    QByteArray tmp;
    QByteArray dump;
    uint size = 0;

    logger().Write("Read all program memory...", Qt::black);

    progress.setProgressParam(0, descr.prog_size);
    progress.setProgressVisible(true);
    progress.setProgressValue(0);

    logger().setPauseLogging(true);

    if( !PrepForProg(device_id, false) ) return false;

    m_dispatcher->setActive(true);
        while(size < descr.prog_size)
        {
            if( !(res &= Read(device_id, tmp)) ) break;
            for(uint i = 0; i < descr.rd_block_size; i++)
            {
                if( size < descr.prog_size )
                {
                    dump.append(tmp.at(i));
                    size++;
                }
            }
            progress.setProgressValue(size);
        }
    m_dispatcher->setActive(false);

    if(res) firmware.setData(dump);

    progress.setProgressVisible(false);
    logger().setPauseLogging(false);

    if(res) logger().WriteLn("OK", Qt::green);
    else logger().WriteLn("Error", Qt::red);

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDPrgGrp::Read(char device_id, EDFirmware &firmware, uint size_to_read, EDPrgProgress &progress)
{
    bool res = true;

    EDDeviceDescriptor descr = m_descr_list[device_id];

    logger().Write("Read program memory...", Qt::black);

    QByteArray tmp;
    QByteArray dump;
    uint size = 0;

    progress.setProgressParam(0, size_to_read);
    progress.setProgressVisible(true);
    progress.setProgressValue(0);

    logger().setPauseLogging(true);

        if( !PrepForProg(device_id, false) ) return false;

        m_dispatcher->setActive(true);
            while(size < size_to_read)
            {
                if( !(res &= Read(device_id, tmp)) ) break;
                for(uint i = 0; i < descr.rd_block_size; i++)
                {
                    if( size < size_to_read )
                    {
                        dump.append(tmp.at(i));
                        size++;
                    }
                }
                progress.setProgressValue(size);
            }
        m_dispatcher->setActive(false);

        if(res) firmware.setData(dump);

        progress.setProgressVisible(false);

    logger().setPauseLogging(false);

    if(res) logger().WriteLn("OK", Qt::green);
    else logger().WriteLn("Error", Qt::red);

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDPrgGrp::FinishProg(char device_id)
{
    com_finishprog->setDeviceID(device_id);
	EDCommand::EDCommandResult result;
    bool res = Run(com_finishprog,result);

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDPrgGrp::PrepForProg(char device_id, bool erase)
{
    com_prepforprog->setDeviceID(device_id);
    com_prepforprog->setErase(erase);
    com_prepforprog->setPrepTimeout(m_descr_list[device_id].prep_timeout*1000);
	EDCommand::EDCommandResult result;
    bool res = Run(com_prepforprog,result);

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDPrgGrp::Read(char device_id, QByteArray &data)
{
    com_read->setDeviceID(device_id);
    com_read->setRdBlockSize(m_descr_list[device_id].rd_block_size);
	EDCommand::EDCommandResult result;
    bool res = Run(com_read,result);

    if(res) data = com_read->getRxData();
    else data.clear();

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDPrgGrp::ReadDescr(char device_id, EDDeviceDescriptor &descr)
{
    com_readdescr->setDeviceID(device_id);
	EDCommand::EDCommandResult result;
    bool res = Run(com_readdescr,result);
    if(res) descr = com_readdescr->Descriptor();

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDPrgGrp::ReadID(char device_id, uint &id)
{
    com_readid->setDeviceID(device_id);
	EDCommand::EDCommandResult result;
    bool res = Run(com_readid,result);

    if(res) id = com_readid->getID();
    else id = 0;

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDPrgGrp::ReqDevNum(char &devnum)
{
	EDCommand::EDCommandResult result;
    bool res = Run(com_reqdevnum,result);

    if(res) devnum = com_reqdevnum->getDevNum();

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDPrgGrp::Restart(char device_id)
{
    com_restart->setDeviceID(device_id);

    //logger().Write("Restart...", Qt::black);

    //logger().setPauseLogging(true);
	EDCommand::EDCommandResult result;
        bool res = Run(com_restart,result);
    //logger().setPauseLogging(false);

    //if(res) logger().WriteLn("OK", Qt::green);
    //else logger().WriteLn("Error", Qt::red);

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDPrgGrp::Write(char device_id, const QByteArray &data)
{
    com_write->setDeviceID(device_id);
    com_write->setWrTimeout(m_descr_list[device_id].wr_timeout*1000);
    com_write->setWrData(data);
	EDCommand::EDCommandResult result;
    bool res = Run(com_write,result);

    return res;
}

