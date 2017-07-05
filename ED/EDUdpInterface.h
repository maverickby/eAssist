#ifndef EDUDPINTERFACE_H
#define EDUDPINTERFACE_H

#include <QtNetwork>

class EDUdpInterface : public QUdpSocket
{
    Q_OBJECT
public:
    explicit EDUdpInterface(const QString &hostname, quint16 port = 1011);

    virtual bool	open(OpenMode mode);
    virtual void	close();

private:

    QString m_hostname;
    quint16 m_port;
    
signals:
    
public slots:

};

#endif // EDUDPINTERFACE_H
