#ifndef EDTELCOM_READDESCR_H
#define EDTELCOM_READDESCR_H

#include "../EDCommand.h"

typedef struct
{
    ulong   period;
    uint    max_num_signals;
    uint    max_frame_size;
    bool    en_stream;
    bool    en_frames;
    bool    en_buffering;
} EDTelDescriptor;

class EDTelCom_ReadDescr : public EDCommand
{
public:
    EDTelCom_ReadDescr(const QString &name);

    const EDTelDescriptor &Descriptor() const;

protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);

private:
    EDTelDescriptor m_descriptor;
};

#endif // EDTELCOM_READDESCR_H
