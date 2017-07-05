#include "EDSysCom_SelfTest.h"

//-------------------------------------------------------------------------------------------
EDSysCom_SelfTest::EDSysCom_SelfTest(const QString &name):
    EDCommand(name)
{
    m_result_selftest = "not yet perfomed";
    setTimeout(3000);
    setFinishType(FT_ZERO_ENDING);
    setRxDataSize(0);
}
//-------------------------------------------------------------------------------------------
void EDSysCom_SelfTest::get_tx_data(QByteArray &data)
{
    data.clear();

    data.resize(2);

    data[0] = 0x01;
    data[1] = 0x05;
}
//-------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDSysCom_SelfTest::process_rx_data(const QByteArray &data)
{
    if(data.size() < 2) return CR_Waiting;

    m_result_selftest = QString(data);
    return CR_Success;
}
//-------------------------------------------------------------------------------------------
const QString &EDSysCom_SelfTest::getSelfTestResult() const
{
    return m_result_selftest;
}




