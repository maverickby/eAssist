#ifndef EDTELCOM_SETMODE_H
#define EDTELCOM_SETMODE_H

#include "../EDCommand.h"

typedef enum {TM_STOP = 0, TM_STREAM = 1, TM_FRAMING = 2, TM_BUFFERING = 3, TM_PAUSE = 4} EDTelMode;

class EDTelCom_SetMode : public EDCommand
{
public:
    EDTelCom_SetMode(const QString &name);

    void setMode(EDTelMode value);
    EDTelMode getMode() const;

    void setDivCoef(ulong value);
    ulong getDivCoef() const;

    void setFrameSize(uint value);
    uint getFramesize() const;

protected:
    virtual void get_tx_data(QByteArray &data);
    virtual EDCommandResultState process_rx_data(const QByteArray &data);

private:
    EDTelMode   m_mode;
    ulong       m_div_coef;
    uint        m_frame_size;

};

#endif // EDTELCOM_SETMODE_H
