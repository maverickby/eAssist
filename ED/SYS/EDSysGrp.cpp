#include "EDSysGrp.h"
#include "../EDLogger.h"

//------------------------------------------------------------------------------------------------
EDSysGrp::EDSysGrp(EDCommandDispatcher *dispatcher) :
        EDCommandGroup(dispatcher)
{
    com_checkchannel = new EDSysCom_CheckChannel    ("CheckChannel");
    com_reqdevinfo   = new EDSysCom_ReqDevInfo      ("RequestDeviceInfo");
    com_reset        = new EDSysCom_Reset           ("Reset");
    com_setscanmode  = new EDSysCom_SetScanMode     ("SetScanMode");
    com_selftest     = new EDSysCom_SelfTest        ("SelfTest");
    com_savesettings = new EDSysCom_SaveSettings    ("SaveSettings");
    com_testchanel   = new EDSysCom_TestChanel      ("TestChannel");

    m_supported.CheckChannel    = false;
    m_supported.DevInfo         = false;
    m_supported.Reset           = false;
    m_supported.SetScanMode     = false;
    m_supported.SelfTest        = false;
    m_supported.SaveSettings    = false;
    m_supported.TestChannel     = false;

    m_scanmode = false;
}
//------------------------------------------------------------------------------------------------
EDSysGrp::~EDSysGrp()
{
    delete com_checkchannel;
    delete com_reqdevinfo;
    delete com_reset;
    delete com_setscanmode;
    delete com_selftest;
    delete com_savesettings;
    delete com_testchanel;
}
//------------------------------------------------------------------------------------------------
void EDSysGrp::Scan()
{
    QString tmp;

    logger().WriteLn("******* Scan system commands *******", Qt::darkMagenta);

    if(m_supported.SetScanMode = SetScanMode(false))
        if(m_supported.CheckChannel = CheckChannel())
            m_supported.DevInfo = ReqDevInfo(tmp);

    if(Available())
    {
        SetScanMode(true);
            m_supported.Reset = Reset();
            m_supported.SelfTest = SelfTest(tmp);
            m_supported.SaveSettings = SaveSettings();
            m_supported.TestChannel = TestChannel(QByteArray("zzz"));
        SetScanMode(false);
    }
    else
    {
        logger().WriteLn("Device does not support required commands to continue...", Qt::red);
    }
}
//------------------------------------------------------------------------------------------------
void EDSysGrp::ReInit()
{
    m_supported.CheckChannel    = false;
    m_supported.DevInfo         = false;
    m_supported.Reset           = false;
    m_supported.SetScanMode     = false;
    m_supported.SelfTest        = false;
    m_supported.SaveSettings    = false;
    m_supported.TestChannel     = false;
}
//------------------------------------------------------------------------------------------------
const EDSysGrp_Supported &EDSysGrp::SuppotedCommand() const
{
    return m_supported;
}
//------------------------------------------------------------------------------------------------
bool EDSysGrp::Available() const
{
    return m_supported.CheckChannel && m_supported.SetScanMode && m_supported.DevInfo;
}
//------------------------------------------------------------------------------------------------
bool EDSysGrp::CheckChannel()
{
	EDCommand::EDCommandResult result;
    bool res = Run(com_checkchannel,result);

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDSysGrp::ReqDevInfo(QString &info)
{
	EDCommand::EDCommandResult result;
    bool res = Run(com_reqdevinfo,result);

    if(com_reqdevinfo->getLastResult() == EDCommand::CR_Success)
    {
        logger().WriteLn("Found device: " + com_reqdevinfo->DeviceInfo(), Qt::blue);
        info = com_reqdevinfo->DeviceInfo();
    }

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDSysGrp::Reset()
{
	EDCommand::EDCommandResult result;
    bool res = Run(com_reset,result);

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDSysGrp::SetScanMode(bool state)
{
    if(state) logger().WriteLn("Turn on scan mode", Qt::gray);
    else logger().WriteLn("Turn off scan mode", Qt::gray);

    com_setscanmode->setScanMode(state);

	EDCommand::EDCommandResult result;
    bool res = Run(com_setscanmode,result);
    if(res) m_scanmode = state;

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDSysGrp::isScanMode() const
{
    return m_scanmode;
}
//------------------------------------------------------------------------------------------------
bool EDSysGrp::SelfTest(QString &result)
{
	EDCommand::EDCommandResult result_;
    bool res = Run(com_selftest,result_);

    if(com_selftest->getLastResult() == EDCommand::CR_Success)
    {
        logger().WriteLn("Self test result: " + com_selftest->getSelfTestResult(), Qt::gray);
        result = com_selftest->getSelfTestResult();
    }

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDSysGrp::SaveSettings()
{
	EDCommand::EDCommandResult result;
    bool res = Run(com_savesettings,result);

    return res;
}
//------------------------------------------------------------------------------------------------
bool EDSysGrp::TestChannel(const QByteArray &data)
{
    com_testchanel->setTestSignature(data);
	EDCommand::EDCommandResult result;
    bool res = Run(com_testchanel,result);

    return res;
}
