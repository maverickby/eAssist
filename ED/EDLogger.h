#ifndef EDLOGGER_H
#define EDLOGGER_H

#include <QMutex>
#include "EDILogger.h"

// Allow thread safe logging
class EDLogger : public EDILogger
{
    friend EDILogger &logger();

public:
    EDLogger();

    virtual void Write(const QString &value, Qt::GlobalColor color);
    virtual void WriteLn(const QString &value, Qt::GlobalColor color);
    virtual void WriteLnMessage(const QString &value, Qt::GlobalColor color);
    virtual void WriteLnMessageCritical(const QString &value, Qt::GlobalColor color);
    virtual void WriteCommandResult(EDCommand::EDCommandResult result);
    virtual void setPauseLogging(bool state);
    virtual bool getPauseLogging();

    virtual void WritePercent(int value, Qt::GlobalColor color);
    virtual void UpdatePercent(int value, Qt::GlobalColor color);

    virtual void WriteSignalInfo(const EDSignal &value);

private:
    QMutex mutex;
};

// Singletone
class EDLoggerSingleton
{
private:
    EDLoggerSingleton();
    EDLoggerSingleton(EDLoggerSingleton&);
    EDLoggerSingleton & operator = (EDLoggerSingleton&);

    static EDLogger m_logger;    
};

// gloabal friend function to access logger singletone
EDILogger &logger();

#endif // EDLOGGER_H
