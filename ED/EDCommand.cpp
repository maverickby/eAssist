#include "EDCommand.h"

//---------------------------------------------------------------------------------
EDCommand::EDCommand(const QString &name, EDFinishType finishtype,
                     int rx_data_size, int timeout_ms):
    m_name(name),
    m_lastresult(CR_Unknown),
    m_use_checksum(true),
    m_timeout_ms(timeout_ms),
    m_finishtype(finishtype),
    m_rx_data_size(rx_data_size)
{}
//-------------------------------------------------------------------------------------------
const QByteArray &EDCommand::PrepareToTxData()
{
    get_tx_data(m_buftx);

    m_bufrx.clear();
    m_lastresult = CR_Unknown;

    if(m_use_checksum) append_checksum(m_buftx);

    return m_buftx;
}
//-------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDCommand::ProcessData(const QByteArray &data, int elapsed_time)
{
    EDCommandResultState res;
    res = CR_Waiting;

    bool timeout =  elapsed_time > m_timeout_ms;

    if( (data.size() > 0) || timeout )
    {
        if(data.size() > 0)
        {
            m_bufrx.append(data);
        }

        switch(m_finishtype)
        {
        case FT_NODATA:
            res = process_rx_nodata(m_bufrx, timeout);
            break;

        case FT_FIXED_DATA:
            res = process_rx_fixed_data(m_bufrx, timeout);
            break;

        case FT_ZERO_ENDING:
            res = process_rx_zero_ending(m_bufrx, timeout);
            break;

        case FT_TIMEOUT:
            res = process_rx_timeout(m_bufrx, timeout);
            break;

        default:
            res = CR_Unknown;
            break;
        }
    }

    if(timeout && (res == CR_Waiting)) return m_lastresult = CR_TimeOut;

    if(res != CR_Waiting) m_lastresult = res;

    return res;
}
//---------------------------------------------------------------------------------
 const QString &EDCommand::getName() const
 {
     return m_name;
 }
//---------------------------------------------------------------------------------
 EDCommand::EDCommandResultState EDCommand::getLastResult() const
 {
     return m_lastresult;
 }
 //---------------------------------------------------------------------------------
 bool EDCommand::isCheckSum() const
 {
     return m_use_checksum;
 }
 //---------------------------------------------------------------------------------
 void EDCommand::setCheckSum(bool value)
 {
     m_use_checksum = value;
 }
 //---------------------------------------------------------------------------------
 void EDCommand::setTimeout(int value)
 {
     m_timeout_ms = value;
 }
 //---------------------------------------------------------------------------------
 int EDCommand::getTimeout() const
 {
     return m_timeout_ms;
 }
 //---------------------------------------------------------------------------------
 void EDCommand::setFinishType(EDFinishType value)
 {
    m_finishtype = value;
 }
 //---------------------------------------------------------------------------------
 EDCommand::EDFinishType EDCommand::getFinishType() const
 {
     return m_finishtype;
 }
 //---------------------------------------------------------------------------------
 void EDCommand::setRxDataSize(int value)
 {
     m_rx_data_size = value;
 }
 //---------------------------------------------------------------------------------
 int EDCommand::getRxDataSize() const
 {
     return m_rx_data_size;
 }
 //---------------------------------------------------------------------------------
 bool EDCommand::isReadyFinish(const QByteArray &data) const
 {
     (void)data;
     return false;
 }
 //---------------------------------------------------------------------------------
 void EDCommand::append_checksum(QByteArray &data)
 {
     char chk = 0;

     foreach (char val, data) {
         chk ^= val;
     }

     data.append(chk);
 }
 //---------------------------------------------------------------------------------
 EDCommand::EDCommandResultState EDCommand::process_rx_nodata(const QByteArray &data, bool timeout)
 {
     if(data.size() > 0)
     {
         int rx_size;
         if(isCheckSum()) rx_size = 1 + 1;
         else rx_size = 1;

         // check responce byte
         if (static_cast<unsigned char>(data[0]) == 0xFF) return CR_NotSuccess;
         if ((static_cast<unsigned char>(data[0]) != 0xFF) && (data[0] != 0x00)) return CR_BadAck;

         if(data.size() > rx_size) return CR_Overflow; // overflow
         else if(data.size() == rx_size)
         {
             // checksum
             if(isCheckSum())
             {
                 if(!verify_checksum(data)) return CR_BadCheckSum;
             }

             QByteArray tmp = data;
             tmp.remove(0, 1); // remove responce byte
             if(isCheckSum()) tmp.remove(tmp.size() - 1, 1);// remove checksum

             return process_rx_data(tmp);
         }
     }

     if(timeout) return CR_TimeOut;

     return CR_Waiting;
 }
 //---------------------------------------------------------------------------------
 EDCommand::EDCommandResultState EDCommand::process_rx_fixed_data(const QByteArray &data, bool timeout)
 {
     if(data.size() > 0)
     {
         int rx_size;
         if(isCheckSum()) rx_size = 1 + m_rx_data_size + 1;
         else rx_size = 1 + m_rx_data_size;

         // check responce byte
         if (static_cast<unsigned char>(data[0]) == 0xFF) return CR_NotSuccess;
         if ((static_cast<unsigned char>(data[0]) != 0xFF) && (data[0] != 0x00)) return CR_BadAck;

         if(data.size() > rx_size)
             return CR_Overflow; // overflow
         else if(data.size() == rx_size)
         {
             // checksum
             if(isCheckSum())
             {
                 if(!verify_checksum(data)) return CR_BadCheckSum;
             }

             QByteArray tmp = data;
             tmp.remove(0, 1); // remove responce byte
             if(isCheckSum()) tmp.remove(tmp.size() - 1, 1);// remove checksum

             return process_rx_data(tmp);
         }
     }

     if(timeout) return CR_TimeOut;

     return CR_Waiting;
 }
 //---------------------------------------------------------------------------------
 EDCommand::EDCommandResultState EDCommand::process_rx_zero_ending(const QByteArray &data, bool timeout)
 {
     if(data.size() > 0)
     {
         int min_rx_size;
         if(isCheckSum()) min_rx_size = 1 /*+ 1*/ + 1; //%%%отключена проверка zero-ending
         else min_rx_size = 1 /*+ 1*/;//%%%отключена проверка zero-ending

         // check responce byte
         if (static_cast<unsigned char>(data[0]) == 0xFF) return CR_NotSuccess;
         if ((static_cast<unsigned char>(data[0]) != 0xFF) && (data[0] != 0x00)) return CR_BadAck;

         if( timeout || (data[data.size() - 2] == '\0') ) // timeout or zero ending
         {
             if(data.size() < min_rx_size) return CR_BadData;
             //%%% if(data[data.size() - 2] != (char)0) return CR_BadData; - отключена проверка на заверщающий ноль(в DSPAssist он не передается в команде чтения)

             // checksum
             if(isCheckSum())
             {
                 if(!verify_checksum(data)) return CR_BadCheckSum;
             }

             QByteArray tmp = data;
             tmp.remove(0, 1); // remove responce byte
             if(isCheckSum()) tmp.remove(tmp.size() - 1, 1);// remove checksum

             return process_rx_data(tmp);
         }
     }

     return CR_Waiting;
 }
 //---------------------------------------------------------------------------------
 EDCommand::EDCommandResultState EDCommand::process_rx_timeout(const QByteArray &data, bool timeout)
 {
     if(data.size() > 0)
     {
         int min_rx_size;
         if(isCheckSum()) min_rx_size = 1 + 2 + 1;
         else min_rx_size = 1 + 2;

         // check responce byte
         if (static_cast<unsigned char>(data[0]) == 0xFF) return CR_NotSuccess;
         if ((static_cast<unsigned char>(data[0]) != 0xFF) && (data[0] != 0x00)) return CR_BadAck;

         if( timeout || isReadyFinish(data)) // timeout
         {
             if(data.size() < min_rx_size) return CR_BadData;

             // checksum
             if(isCheckSum())
             {
                 if(!verify_checksum(data)) return CR_BadCheckSum;
             }

             QByteArray tmp = data;
             tmp.remove(0, 1); // remove responce byte
             if(isCheckSum()) tmp.remove(tmp.size() - 1, 1);// remove checksum

             return process_rx_data(tmp);
         }
     }

     return CR_Waiting;
 }
 //---------------------------------------------------------------------------------
 bool EDCommand::verify_checksum(const QByteArray &data)
 {
    char chk = 0;
    int size = data.size();

    for(int i = 0; i < size - 1; i++) chk ^= data[i];

    return (data[size - 1] == chk);
 }
