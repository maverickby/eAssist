#include "EDPrgCom_ReadDescr.h"

//--------------------------------------------------------------------------------------------------------
EDPrgCom_ReadDescr::EDPrgCom_ReadDescr(const QString &name):
    EDCommand(name)
{
    m_descriptor.name = "not yet read";
    m_descriptor.wr_block_size = 0;
    m_descriptor.rd_block_size = 0;
    m_descriptor.prog_size = 0;
    m_descriptor.wr_timeout = 0;
    m_descriptor.prep_timeout = 0;
    m_descriptor.restart = false;

    m_device_id = -1;

    setFinishType(FT_TIMEOUT);
    setTimeout(1000);
}
//--------------------------------------------------------------------------------------------------------
const EDDeviceDescriptor & EDPrgCom_ReadDescr::Descriptor() const
{
    return m_descriptor;
}
//--------------------------------------------------------------------------------------------------------
char EDPrgCom_ReadDescr::getDeviceID() const
{
    return m_device_id;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_ReadDescr::setDeviceID(char value)
{
    m_device_id = value;
}
//--------------------------------------------------------------------------------------------------------
void EDPrgCom_ReadDescr::get_tx_data(QByteArray &data)
{
    data.clear();

    data.resize(3);

    data[0] = 0x02;
    data[1] = 0x02;
    data[2] = m_device_id;
}
//--------------------------------------------------------------------------------------------------------
EDCommand::EDCommandResultState EDPrgCom_ReadDescr::process_rx_data(const QByteArray &data)
{
    const int fixed_data_size = 17;
    int size = data.size();
    if(size > 256) return CR_Overflow;

    int pos = 0;

    //name
    m_descriptor.name.clear();
    for(int i = 0; i < size; i++)
    {
        if( ((size-1) - pos) < fixed_data_size) return CR_BadData;
        if(data[pos] != 0) m_descriptor.name.append(data[pos]);
        else
        {
            pos++;
            break;
        }
        pos++;
    }

    if( (size - pos) != fixed_data_size) return CR_BadData;

    //wr_block_size
    m_descriptor.wr_block_size = ((static_cast<uint>(data[pos + 3]) & 0xFF) << (3 * 8))	|
                                 ((static_cast<uint>(data[pos + 2]) & 0xFF) << (2 * 8))	|
                                 ((static_cast<uint>(data[pos + 1]) & 0xFF) << (1 * 8))	|
                                 ((static_cast<uint>(data[pos + 0]) & 0xFF) << (0 * 8));
    pos += 4;

    //rd_block_size
    m_descriptor.rd_block_size = ((static_cast<uint>(data[pos + 3]) & 0xFF) << (3 * 8))	|
                                 ((static_cast<uint>(data[pos + 2]) & 0xFF) << (2 * 8))	|
                                 ((static_cast<uint>(data[pos + 1]) & 0xFF) << (1 * 8))	|
                                 ((static_cast<uint>(data[pos + 0]) & 0xFF) << (0 * 8));
    pos += 4;

    //prog_size
    m_descriptor.prog_size = ((static_cast<uint>(data[pos + 3]) & 0xFF) << (3 * 8))	|
                             ((static_cast<uint>(data[pos + 2]) & 0xFF) << (2 * 8))	|
                             ((static_cast<uint>(data[pos + 1]) & 0xFF) << (1 * 8))	|
                             ((static_cast<uint>(data[pos + 0]) & 0xFF) << (0 * 8));
    pos += 4;

    //wr_timeout
    m_descriptor.wr_timeout = ((static_cast<uint>(data[pos + 1]) & 0xFF) << 1*8) | (static_cast<uint>(data[pos + 0]) << 0*8);
    pos += 2;

    //prep_timeout
    m_descriptor.prep_timeout = ((static_cast<uint>(data[pos + 1]) & 0xFF) << 1*8) | ((static_cast<uint>(data[pos + 0]) & 0xFF) << 0*8);
    pos += 2;

    //restart
    m_descriptor.restart = (data[pos] == 0x01);
    pos++;

    if( size != pos ) return CR_BadData;

    return CR_Success;
}
