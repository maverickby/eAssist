#ifndef EDILOGGER_H
#define EDILOGGER_H

#include <QString>
#include "EDCommand.h"
#include "SIG/EDSignal.h"

class EDILogger
{
public:
    EDILogger(EDILogger *pLogger);

    static void setLogger(EDILogger *pLogger);

    virtual void Write(const QString &value, Qt::GlobalColor color) = 0;
    virtual void WriteLn(const QString &value, Qt::GlobalColor color) = 0;
    virtual void WriteLnMessage(const QString &value, Qt::GlobalColor color) = 0;
    virtual void WriteLnMessageCritical(const QString &value, Qt::GlobalColor color) = 0;
    virtual void WriteCommandResult(EDCommand::EDCommandResult result) = 0;
    virtual void setPauseLogging(bool state) = 0;
    virtual bool getPauseLogging() = 0;

    virtual void WritePercent(int value, Qt::GlobalColor color) = 0;
    virtual void UpdatePercent(int value, Qt::GlobalColor color) = 0;

    virtual void WriteSignalInfo(const EDSignal &value) = 0;

protected:
    static EDILogger *m_pLogger;
};

#endif // EDILOGGER_H
