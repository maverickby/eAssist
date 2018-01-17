#include "EDCommandGroup.h"
#include "EDLogger.h"
#include "threadtelreaddata.h"

//---------------------------------------------------------------------------------------------
EDCommandGroup::EDCommandGroup(EDCommandDispatcher *dispatcher) :
    m_dispatcher(dispatcher)
{

}
//---------------------------------------------------------------------------------------------
bool EDCommandGroup::Run(EDCommand *command, EDCommand::EDCommandResult& result, bool logging)
{
    //%%%QMutexLocker locker(&mutex);
	/*if (threadTelReadData != NULL)
	{
		connect(threadTelReadData, SIGNAL(SignalLoggerWrite(const QString &, Qt::GlobalColor)), this, SLOT(loggerWrite(const QString &, Qt::GlobalColor)), Qt::QueuedConnection);
		connect(threadTelReadData, SIGNAL(SignalLoggerWriteCommandResult(EDCommand::EDCommandResult)), this, SLOT(loggerWriteCommandResult(EDCommand::EDCommandResult)), Qt::QueuedConnection);
	}*/	
    
	EDCommand::EDCommandResult com_result =  m_dispatcher->Run(command);
	result = com_result;

	//m_dispatcher->setCommand(command);
	//m_dispatcher->start(QThread::TimeCriticalPriority);

	//EDCommand::EDCommandResult com_result = m_dispatcher->GetCommandResult();
	//result = com_result;

	//НЕ логгировать здесь в случае ошибки, это осуществялется при помощи Telemetry::Start()-->
	//connect(&m_threadTelReadData, SIGNAL(SignalLoggerWrite(const QString &, Qt::GlobalColor)), this, SLOT(loggerWrite(const QString &, Qt::GlobalColor)), Qt::QueuedConnection);
	// и далее в методе ThreadTelReadData::run()
    if( logging /*|| (com_result.state != EDCommand::CR_Success)*/ ) // in case off error - show it in log
    {		
		logger().Write(command->getName() + "...", Qt::black);
		logger().WriteCommandResult(com_result);		
    }

    return (com_result.state == EDCommand::CR_Success);
	//return true;
}

// void EDCommandGroup::loggerWrite(const QString &str, Qt::GlobalColor color)
// {
// 	logger().Write(str, color);
// }
// 
// void EDCommandGroup::loggerWriteCommandResult(EDCommand::EDCommandResult result)
// {
// 	logger().WriteCommandResult(result);
// }
