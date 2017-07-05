#include "EDILogger.h"

//----------------------------------------------------------------------------------------
EDILogger *EDILogger::m_pLogger = nullptr;
//----------------------------------------------------------------------------------------
EDILogger::EDILogger(EDILogger *pLogger)
{
    setLogger(pLogger);
}
//----------------------------------------------------------------------------------------
void EDILogger::setLogger(EDILogger *pLogger)
{
    if(pLogger != nullptr) m_pLogger = pLogger;
    else throw "nullptr pointer to EDILogger object";
}

