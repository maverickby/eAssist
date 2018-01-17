#pragma once
#ifndef THREADTELREADDATA_H
#define THREADTELREADDATA_H

#include <QThread>
#include <QMutex>
#include <stdint.h>
#include "ED/TEL/EDTelGrp.h"
#include "ED/TEL/EDTel_SignalDataBuffer.h"

//поток для телеметрии сигналов
class ThreadTelReadData : public QThread
{
    Q_OBJECT
public:
    ThreadTelReadData(EDTel *telgrp);

    void run() Q_DECL_OVERRIDE;

    TelStatistics getStatistics();
    void ResetStatistics();
    void lock();
    void unlock();
    void stop();
    void setPeriod(uint32_t value);

private:
    EDTel *m_telgrp;
    QMutex mutex;
    TelStatistics m_statistics;
    bool m_stop_request;
    uint32_t m_period;
signals:
	void SignalLoggerWrite(const QString &, Qt::GlobalColor);
	void SignalLoggerWriteCommandResult(EDCommand::EDCommandResult);
};

#endif // THREADTELREADDATA_H
