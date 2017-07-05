#ifndef EDTEL_SIGNALDATA_H
#define EDTEL_SIGNALDATA_H

#include <Qt>

class EDTel_SignalData
{
public:
    EDTel_SignalData(uint signal_id, uint value_size);
    ~EDTel_SignalData();

    uint getSignalID() const;
    uint getValueSize() const;

    void setNumSamples(uint value);
    uint getNumSamples() const;

    void ClearSamples();

    template<typename T>
    T getValue(int index) const
    {
        return *static_cast<T*>(&m_data[index*m_value_size]);
    }

    char* data() const;

private:
    uint m_signal_id;
    uint m_value_size;
    uint m_numsamples;

    char* m_data;

};

#endif // EDTEL_SIGNALDATA_H
