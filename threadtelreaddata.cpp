#include "threadtelreaddata.h"

//---------------------------------------------------------------------------------------------------------
ThreadTelReadData::ThreadTelReadData(EDTel *telgrp):
    m_stop_request(false)
{
    m_telgrp = telgrp;

    m_statistics.datarequests = 0;
    m_statistics.num_samples = 0;
    m_statistics.errors = 0;
    m_statistics.state_overflow = 0;
    m_statistics.state_wait = 0;

    setTerminationEnabled(false);
	qRegisterMetaType<Qt::GlobalColor>("Qt::GlobalColor");
	qRegisterMetaType<EDCommand::EDCommandResult>("EDCommand::EDCommandResult");

}
//---------------------------------------------------------------------------------------------------------
void ThreadTelReadData::run()
{
	QString command;
	EDCommand::EDCommandResult result;
    while(!m_stop_request)
    {
        mutex.lock();
		if (m_telgrp->ReadDataToBuffer(m_statistics, command, result, false) != true)// in case off error - show it in log
		{
			emit SignalLoggerWrite(command + "...", Qt::black);
			emit SignalLoggerWriteCommandResult(result);
		}
        mutex.unlock();

        if(m_period > 0) this->usleep(m_period);
    }

    m_stop_request = false;
}
//---------------------------------------------------------------------------------------------------------
TelStatistics ThreadTelReadData::getStatistics()
{
    TelStatistics res = m_statistics;
    m_statistics.num_samples = 0;
    return res;
}
//---------------------------------------------------------------------------------------------------------
void ThreadTelReadData::ResetStatistics()
{
    m_statistics.datarequests = 0;
    m_statistics.num_samples = 0;
    m_statistics.errors = 0;
    m_statistics.state_overflow = 0;
    m_statistics.state_wait = 0;
}
//---------------------------------------------------------------------------------------------------------
void ThreadTelReadData::lock()
{
    mutex.lock();
}
//---------------------------------------------------------------------------------------------------------
void ThreadTelReadData::unlock()
{
    mutex.unlock();
}
//---------------------------------------------------------------------------------------------------------
void ThreadTelReadData::stop()
{
    m_stop_request = true;
}
//---------------------------------------------------------------------------------------------------------
void ThreadTelReadData::setPeriod(uint32_t value)
{
    m_period = value;
}

