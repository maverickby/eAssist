#include "EDTel_SignalData.h"

//----------------------------------------------------------------------------------------------
EDTel_SignalData::EDTel_SignalData(uint signal_id, uint value_size)
{
    m_signal_id = signal_id;
    m_value_size = value_size;
    m_numsamples = 0;
    m_data = nullptr;
}
//----------------------------------------------------------------------------------------------
EDTel_SignalData::~EDTel_SignalData()
{
    ClearSamples();
}
//----------------------------------------------------------------------------------------------
uint EDTel_SignalData::getSignalID() const
{
    return m_signal_id;
}
//----------------------------------------------------------------------------------------------
uint EDTel_SignalData::getValueSize() const
{
    return m_value_size;
}
//----------------------------------------------------------------------------------------------
void EDTel_SignalData::setNumSamples(uint value)
{
    ClearSamples();

    m_data = new char[value*m_value_size];
    m_numsamples = value;
}
//----------------------------------------------------------------------------------------------
uint EDTel_SignalData::getNumSamples() const
{
    return m_numsamples;
}
//----------------------------------------------------------------------------------------------
void EDTel_SignalData::ClearSamples()
{
    if(m_data != nullptr)
    {
        delete [] m_data;
        m_data = nullptr;
        m_numsamples = 0;
    }
}
//----------------------------------------------------------------------------------------------
char *EDTel_SignalData::data() const
{
    return m_data;
}

