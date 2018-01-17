#ifndef EDCOMMAND_H
#define EDCOMMAND_H

#include <QByteArray>
#include <QString>

class EDCommand
{
public:

    typedef enum { CR_Success, CR_NotSuccess, CR_BadAck, CR_TimeOut, CR_BadData, CR_Overflow,
		CR_BadCheckSum, CR_Waiting, CR_Unknown, CR_BadWrapper, CR_E_READ_FAILED } EDCommandResultState;

    typedef struct
    {
        EDCommandResultState state;
        QByteArray responce;
    } EDCommandResult;

    typedef enum { FT_NODATA, FT_FIXED_DATA, FT_ZERO_ENDING, FT_TIMEOUT } EDFinishType;

    // constructor
    explicit EDCommand(const QString &name, EDFinishType finishtype = FT_NODATA,
                       int rx_data_size = 0, int timeout_ms = 1000);

    virtual const QByteArray &PrepareToTxData();
    virtual EDCommandResultState ProcessData(const QByteArray &data, int elapsed_time);

    virtual const QString &getName() const;

    virtual EDCommandResultState getLastResult() const;

    bool isCheckSum() const;
    void setCheckSum(bool value);

    int getTimeout() const;
    void setTimeout(int value);

protected:
    // functions to override in derived classes
    virtual void get_tx_data(QByteArray &data) = 0;                             // generate data to transmit in command
    virtual EDCommandResultState process_rx_data(const QByteArray &data) = 0;   // process command responce and make decision about command result
    virtual bool isReadyFinish(const QByteArray &data) const;                   // used for commands with finishtype == FT_TIMEOUT? in order to

    void setFinishType(EDFinishType value);
    EDFinishType getFinishType() const;

    void setRxDataSize(int value);
    int getRxDataSize() const;

private:
    void append_checksum(QByteArray &data);
    bool verify_checksum(const QByteArray &data);

    EDCommandResultState process_rx_nodata       (const QByteArray &data, bool timeout);
    EDCommandResultState process_rx_fixed_data   (const QByteArray &data, bool timeout);
    EDCommandResultState process_rx_zero_ending  (const QByteArray &data, bool timeout);
    EDCommandResultState process_rx_timeout      (const QByteArray &data, bool timeout);

    QString m_name;
    EDCommandResultState m_lastresult;

    QByteArray m_bufrx;
    QByteArray m_buftx;
    bool m_use_checksum;
    int m_timeout_ms;
    EDFinishType m_finishtype;
    int m_rx_data_size;
};

#endif // EDCOMMAND_H
