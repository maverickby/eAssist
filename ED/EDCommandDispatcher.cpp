#include <QElapsedTimer>
#include <QCoreApplication>
#include "EDCommandDispatcher.h"
#include "EDLogger.h"

//--------------------------------------------------------------------------------------------------------1
EDCommandDispatcher::EDCommandDispatcher(QIODevice *device):
    m_device(device)
{}
//--------------------------------------------------------------------------------------------------------
// Send command data, controlling timeout and give control to EDCommand.ProcessData() to make decission about command result
EDCommand::EDCommandResult EDCommandDispatcher::Run(EDCommand *command)
{
    mutex.lock();

    EDCommand::EDCommandResult res;
    QByteArray data;

    if(m_device != nullptr)
    {
        bool opened = m_device->isOpen();

        if(!opened)
        {
            if(!m_device->open(QIODevice::ReadWrite))
            {
                logger().Write("<Can not open device>", Qt::red);
                res.state = EDCommand::CR_NotSuccess;
                return res;
            }
        }

        m_device->readAll(); // flush input buffer

        if(!m_wrp_enable)   m_device->write(command->PrepareToTxData());
        else
        {
            QByteArray tmp = wrap(command->PrepareToTxData());
            m_device->write(tmp);
        }

        QElapsedTimer elapsedtimer;
        elapsedtimer.start();
        bool done = false;
        bool wrp_ok = false;

        if(m_wrp_enable) // receive wrapper with command
        {
            // receive wrapper
            QByteArray tmp;
            while(!done)
            {
                data = m_device->read(max_answer_size);
                if(data.size() > 0) tmp.append(data);
                if(tmp.size() >= 8) // at least empty wrapper comming
                {
                    uint16_t cs = CalcCS((uint16_t*)tmp.data(), 2);
                    uint16_t cs_real = ((uint8_t)tmp.at(4)<<8 ) | (uint8_t)tmp.at(5);

                    if( (tmp.at(0) != 0x5A) || (tmp.at(1) != m_wrp_local_addr) || (cs != cs_real))
                        {res.state = EDCommand::CR_BadWrapper; done = true;}
                    else // header is valid
                    {
                        uint16_t size16 =   (((static_cast<uint16_t>(tmp.at(3)) & 0xFF) << 8 ) |
                                            (static_cast<uint16_t>(tmp.at(2)) & 0xFF) ) & 0xFFF;
                        if(tmp.size() >= 8 + size16*2) //full wrapper is here
                        {
                            if(tmp.at(3) & 0x80) data = tmp.mid(6, size16*2 - 1);
                            else                 data = tmp.mid(6, size16*2);
                            done = true;
                            wrp_ok = true;
                        }// else continue waiting remaining data
                    }

                }
                if(elapsedtimer.elapsed() > command->getTimeout()) // finish on timeout
                    {res.state = EDCommand::CR_TimeOut; done = true;}
            }

            // receive command
            if(wrp_ok)
            {
                done = false;
                while(!done)
                {
                    if( data.size() > 0 ) res.responce.append(data);
                    EDCommand::EDCommandResultState com_res = command->ProcessData(data, elapsedtimer.elapsed());
                    data.clear();
                    if(com_res != EDCommand::CR_Waiting) done = true;
                }
            }
        } else
        {
            // receive command
            done = false;
            while(!done)
            {
                data = m_device->read(max_answer_size);
                if(data.size() > 0) res.responce.append(data);
                EDCommand::EDCommandResultState com_res = command->ProcessData(data, elapsedtimer.elapsed());
                if(com_res != EDCommand::CR_Waiting) done = true;
            }
        }

        if(!opened) m_device->close();

        if( (m_wrp_enable && wrp_ok) || !m_wrp_enable) res.state = command->getLastResult();

        mutex.unlock();

        QCoreApplication::instance()->processEvents();// this help unfreez gui while proccessing a lot of siquental commands

        return res;
    } else
    {
        logger().Write("<Device not assigned>", Qt::red);
        res.state = EDCommand::CR_NotSuccess;

        mutex.unlock();

        return res;
    }
}
//--------------------------------------------------------------------------------------------------------
void EDCommandDispatcher::setInterface(QIODevice *device)
{
    m_device = device;
}
//--------------------------------------------------------------------------------------------------------
void EDCommandDispatcher::setActive(bool value)
{
    if(m_device == nullptr) return;

    if(value)
    {
        if(!m_device->isOpen()) m_device->open(QIODevice::ReadWrite);
    }
    else
    {
        if(m_device->isOpen()) m_device->close();
    }
}
//--------------------------------------------------------------------------------------------------------
bool EDCommandDispatcher::isActive() const
{
    if(m_device == nullptr) return false;

    return m_device->isOpen();
}
//--------------------------------------------------------------------------------------------------------
void EDCommandDispatcher::Enable_Wrapper()
{
    m_wrp_enable = true;
}
//--------------------------------------------------------------------------------------------------------
void EDCommandDispatcher::Disable_Wrapper()
{
    m_wrp_enable = false;
}
//--------------------------------------------------------------------------------------------------------
bool EDCommandDispatcher::isEnabledWrapper() const
{
    return m_wrp_enable;
}
//--------------------------------------------------------------------------------------------------------
void EDCommandDispatcher::setWrpLocalAddress(unsigned char value)
{
    m_wrp_local_addr = value;
}
//--------------------------------------------------------------------------------------------------------
unsigned char EDCommandDispatcher::getWrpLocalAddress() const
{
    return m_wrp_local_addr;
}
//--------------------------------------------------------------------------------------------------------
void EDCommandDispatcher::setWrpRemoteAddress(unsigned char value)
{
    m_wrp_remote_addr = value;
}
//--------------------------------------------------------------------------------------------------------
unsigned char EDCommandDispatcher::getWrpRemoteAddress() const
{
    return m_wrp_remote_addr;
}
//--------------------------------------------------------------------------------------------------------
QByteArray EDCommandDispatcher::wrap(const QByteArray &data)
{
    QByteArray tmp;
    uint16_t size = data.size();
    bool padding = (size & 0x0001) != 0;
    uint16_t chksum;

    if(padding) size += 1;
    size = size/2;

    tmp.append(0x5A);
    tmp.append(m_wrp_remote_addr);
    tmp.append(size);
    tmp.append(size >> 8);

    if(padding) tmp[3] = tmp[3] | 0x80;

    chksum = CalcCS((uint16_t*)tmp.data(), 2);
    tmp.append(chksum >> 8);
    tmp.append(chksum);

    tmp.append(data);
    if(padding) tmp.append((char)0x00);

    size += 3;
    chksum = CalcCS((uint16_t*)tmp.data(), size);
    tmp.append(chksum >> 8);
    tmp.append(chksum);

    return std::move(tmp);
}
//--------------------------------------------------------------------------------------------------------
#define LITTLE_ENDIAN 1
uint16_t EDCommandDispatcher::CalcCS(uint16_t* data, uint32_t len)
{
    uint32_t cs = 0;
    uint32_t i;
    uint16_t* dataptr = data;
    for (i = 0; i < len; i++)
    {
        #if LITTLE_ENDIAN
            cs += ((*dataptr & 0xff) << 8) | ((*dataptr & 0xff00) >> 8);
        #else
            cs += *dataptr;
        #endif
        dataptr++;
    }
    uint32_t carry;
    do
    {
        carry = cs >> 16;
        cs = (cs & 0xFFFF) + carry;
    } while (carry != 0);
    return ~cs;
}
