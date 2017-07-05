#include "EDTel_SignalDataBuffer.h"

//--------------------------------------------------------------------------------------------------------------------
EDTel_SignalDataBuffer::EDTel_SignalDataBuffer(uint signal_id, uint value_size, uint reserved_size_samples)
{
    m_signal_id = signal_id;
    m_value_size = value_size;
    m_numsamples = 0;
    m_data.reserve(reserved_size_samples);
    m_reserved_size_samples = reserved_size_samples;
}
//--------------------------------------------------------------------------------------------------------------------
uint EDTel_SignalDataBuffer::getSignalID() const
{
    return m_signal_id;
}
//--------------------------------------------------------------------------------------------------------------------
uint EDTel_SignalDataBuffer::getValueSize() const
{
    return m_value_size;
}
//--------------------------------------------------------------------------------------------------------------------
uint EDTel_SignalDataBuffer::getNumSamples() const
{
    return m_data.size()/m_value_size;
}
//--------------------------------------------------------------------------------------------------------------------
void EDTel_SignalDataBuffer::ClearSamples()
{
    m_data.clear();
    m_data.reserve(m_reserved_size_samples);
}
//--------------------------------------------------------------------------------------------------------------------
QByteArray &EDTel_SignalDataBuffer::data()
{
    return m_data;
}
