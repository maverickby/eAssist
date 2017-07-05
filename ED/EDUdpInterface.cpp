#include <QtNetwork>
#include "EDUdpInterface.h"

//-----------------------------------------------------------------------------------
EDUdpInterface::EDUdpInterface(const QString &hostname, quint16 port) :
    m_hostname(hostname),
    m_port(port),
    QUdpSocket()
{}
//-----------------------------------------------------------------------------------
bool EDUdpInterface::open(OpenMode mode)
{
    QUdpSocket::open(mode);

    connectToHost(m_hostname, m_port, mode);
    bool res = waitForConnected(2000);
    if(!res) QUdpSocket::close();

    return res;
}
//-----------------------------------------------------------------------------------
void EDUdpInterface::close()
{
    QUdpSocket::close();

    disconnectFromHost();
}
