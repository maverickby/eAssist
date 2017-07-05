#include "EDCommandGroup.h"
#include "EDLogger.h"

//---------------------------------------------------------------------------------------------
EDCommandGroup::EDCommandGroup(EDCommandDispatcher *dispatcher) :
    m_dispatcher(dispatcher)
{}
//---------------------------------------------------------------------------------------------
bool EDCommandGroup::Run(EDCommand *command, bool logging)
{
    //%%%QMutexLocker locker(&mutex);

    EDCommand::EDCommandResult com_result =  m_dispatcher->Run(command);

    if( logging || (com_result.state != EDCommand::CR_Success) ) // in case off error - show it in log
    {
        logger().Write(command->getName() + "...", Qt::black);
        logger().WriteCommandResult(com_result);
    }

    return (com_result.state == EDCommand::CR_Success);
}
