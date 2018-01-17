#ifndef EDCOMMANDDISPATCHER_H
#define EDCOMMANDDISPATCHER_H

#include <QIODevice>
#include <QMutex>
#include <stdint.h>
#include "EDCommand.h"
#include <QThread>

class EDCommandDispatcher : public QThread
{
public:
    explicit EDCommandDispatcher(QIODevice *device);

    EDCommand::EDCommandResult Run(EDCommand *command);
	void setCommand(EDCommand *command);
	void run() Q_DECL_OVERRIDE;
    void setInterface(QIODevice *device);

    void setActive(bool value);
    bool isActive() const;

    void Enable_Wrapper();
    void Disable_Wrapper();
    bool isEnabledWrapper() const;

    void setWrpLocalAddress(unsigned char value);
    unsigned char getWrpLocalAddress() const;
    void setWrpRemoteAddress(unsigned char value);
    unsigned char getWrpRemoteAddress() const;
	EDCommand::EDCommandResult GetCommandResult();
	inline QMutex* getMutex() { return &mutex; }
	void stop();

protected:
    QByteArray wrap(const QByteArray &data);
    uint16_t CalcCS(uint16_t* data, uint32_t len);

private:
    QIODevice *m_device;

    static const quint64 max_answer_size = 10000;

    QMutex mutex;
	bool m_stop_request;

    bool m_wrp_enable;
    unsigned char m_wrp_local_addr;
    unsigned char m_wrp_remote_addr;

	EDCommand *m_command;
	EDCommand::EDCommandResult m_commandResult;

};

#endif // EDCOMMANDDISPATCHER_H
