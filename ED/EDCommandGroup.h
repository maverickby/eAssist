#pragma once
#ifndef EDCOMMANDGROUP_H
#define EDCOMMANDGROUP_H

#include <QObject>
#include "EDCommandDispatcher.h"
//#include "threadtelreaddata.h"

class ThreadTelReadData;

class EDCommandGroup : public QObject
{
	Q_OBJECT
public:
    EDCommandGroup(EDCommandDispatcher *dispatcher);
    virtual void Scan() = 0;
    virtual void ReInit() = 0; // set to initial state

    bool Run(EDCommand *command, EDCommand::EDCommandResult& result, bool logging = true);

protected:
    EDCommandDispatcher *m_dispatcher;

private:
    QMutex mutex;
private:

// signals:
// 	void SignalLoggerWrite(const QString &, Qt::GlobalColor);
// 	void SignalLoggerWriteCommandResult(EDCommand::EDCommandResult);

// private slots:
// void loggerWrite(const QString &str, Qt::GlobalColor color);
// void loggerWriteCommandResult(EDCommand::EDCommandResult result);
};

#endif // EDCOMMANDGROUP_H
