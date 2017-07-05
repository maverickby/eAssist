#ifndef EDTEL_SIGNALDATABUFFER_H
#define EDTEL_SIGNALDATABUFFER_H

#include <QByteArray>

class EDTel_SignalDataBuffer
{
public:
    EDTel_SignalDataBuffer(uint signal_id, uint value_size, uint reserved_size_samples = 10000);

    uint getSignalID() const;
    uint getValueSize() const;

    uint getNumSamples() const;
    void ClearSamples();

    template<typename T>
    T getValue(int index) const
    {
        return *static_cast<const T*>(static_cast<const void*>(&m_data.data()[index*m_value_size]));
    }

    QByteArray &data();

private:
    uint m_signal_id;
    uint m_value_size;
    uint m_numsamples;

    uint m_reserved_size_samples;

    QByteArray m_data;
};

#endif // EDTEL_SIGNALDATABUFFER_H
