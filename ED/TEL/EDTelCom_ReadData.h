#ifndef EDTELCOM_READDATA_H
#define EDTELCOM_READDATA_H

#include "../EDCommand.h"

#define TEL_STATUS_NO_ERROR     0x01
#define TEL_STATUS_OVERFLOW     0x02
#define TEL_STATUS_ERROR1       0x04
#define TEL_STATUS_ERROR2       0x08
#define TEL_STATUS_ERROR3       0x10
#define TEL_STATUS_ERROR4       0x20
#define TEL_STATUS_WAIT         0x40
#define TEL_STATUS_FRAME_ERR    0x80

class EDTelCom_ReadData : public EDCommand
{
public:
    EDTelCom_ReadData(const QString &name);

    uint getNumSamples() const;
    uchar getStatus() const;
    const QByteArray &Samples() const;

    void setSampleSize(uint value);
    uint getSampleSize() const;

protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);
    virtual bool isReadyFinish(const QByteArray &data) const;

private:
   uint         m_numsamples;
   uchar        m_status;
   QByteArray   m_samples;
   uint         m_sample_size;
};

#endif // EDTELCOM_READDATA_H
