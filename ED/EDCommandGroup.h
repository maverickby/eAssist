#ifndef EDCOMMANDGROUP_H
#define EDCOMMANDGROUP_H

#include "EDCommandDispatcher.h"

class EDCommandGroup
{
public:
    EDCommandGroup(EDCommandDispatcher *dispatcher);
    virtual void Scan() = 0;
    virtual void ReInit() = 0; // set to initial state

    bool Run(EDCommand *command, bool logging = true);

protected:
    EDCommandDispatcher *m_dispatcher;

private:
    QMutex mutex;
};

#endif // EDCOMMANDGROUP_H
