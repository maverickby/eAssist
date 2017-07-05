#include "EDLogger.h"

//--------------------------------------------------------------------------------------------------
EDLogger::EDLogger():
    EDILogger(this)
{
}
//--------------------------------------------------------------------------------------------------
void EDLogger::Write(const QString &value, Qt::GlobalColor color)
{
    if(m_pLogger != this)
    {
        QMutexLocker locker(&mutex);
        m_pLogger->Write(value, color);
    }
}
//--------------------------------------------------------------------------------------------------
void EDLogger::WriteLn(const QString &value, Qt::GlobalColor color)
{
    if(m_pLogger != this)
    {
        QMutexLocker locker(&mutex);
        WriteLn(value, color);
    }
}
//--------------------------------------------------------------------------------------------------
void EDLogger::WriteLnMessage(const QString &value, Qt::GlobalColor color)
{
    if(m_pLogger != this)
    {
        QMutexLocker locker(&mutex);
        WriteLnMessage(value, color);
    }
}
//--------------------------------------------------------------------------------------------------
void EDLogger::WriteLnMessageCritical(const QString &value, Qt::GlobalColor color)
{
    if(m_pLogger != this)
    {
        QMutexLocker locker(&mutex);
        WriteLnMessageCritical(value, color);
    }
}
//--------------------------------------------------------------------------------------------------
void EDLogger::WriteCommandResult(EDCommand::EDCommandResult result)
{
    if(m_pLogger != this)
    {
        QMutexLocker locker(&mutex);
        WriteCommandResult(result);
    }
}
//--------------------------------------------------------------------------------------------------
void EDLogger::setPauseLogging(bool state)
{
    if(m_pLogger != this)
    {
        QMutexLocker locker(&mutex);
        setPauseLogging(state);
    }
}
//--------------------------------------------------------------------------------------------------
bool EDLogger::getPauseLogging()
{
    if(m_pLogger != this)
    {
        QMutexLocker locker(&mutex);
        return getPauseLogging();
    }
    return false;
}
//--------------------------------------------------------------------------------------------------
void EDLogger::WritePercent(int value, Qt::GlobalColor color)
{
    if(m_pLogger != this)
    {
        QMutexLocker locker(&mutex);
        WritePercent(value, color);
    }
}
//--------------------------------------------------------------------------------------------------
void EDLogger::UpdatePercent(int value, Qt::GlobalColor color)
{
    if(m_pLogger != this)
    {
        QMutexLocker locker(&mutex);
        UpdatePercent(value, color);
    }
}
//--------------------------------------------------------------------------------------------------
void EDLogger::WriteSignalInfo(const EDSignal &value)
{
    if(m_pLogger != this)
    {
        QMutexLocker locker(&mutex);
        WriteSignalInfo(value);
    }
}
//--------------------------------------------------------------------------------------------------
EDLogger EDLoggerSingleton::m_logger; // init static variables
//--------------------------------------------------------------------------------------------------
EDILogger &logger()
{
    return *EDLogger::m_pLogger;
}

