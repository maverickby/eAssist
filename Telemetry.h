#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <QObject>
#include <QElapsedTimer>
#include "TelData.h"
#include "threadtelreaddata.h"
#include "ED/ED.h"

class Telemetry : public QObject
{
    Q_OBJECT
public:
    explicit Telemetry(ED *ed, QObject *parent = 0);
    ~Telemetry();

    void Start(ulong coef);
    void Pause();
    void Stop();
    void setPeriod(uint32_t value);
	void StopThreadTelemetryReadData();

    void setSignalMode(const EDSignal *signal);
    void Clear();
    bool isRunning() const;
    int getNumSignals() const;

    void setWrapMode(bool mode);

    void setNumPoints(int numpoints);
    const QVector<TelData*> *getTelData();
    void getSignalNames(QVector<QString> &names);

private:
    void setup_tel_data();

    // application data
    QList<int> m_tel_signals;
    ED *m_ed;
    bool m_running;
    QVector<TelData*> m_tel_data;
    bool m_wrapmode;
    int m_numpoints;

    QTimer m_timer;
    bool request_to_stop;

    ThreadTelReadData m_threadTelReadData;//поток для телеметрии сигналов

    TelStatistics m_statistics;

    QList<EDTel_SignalDataBuffer> *tel_buffer;

signals:
    void DataReady();
    void Statistics(const TelStatistics & statistics);

public slots:

private slots:
    void tick();

private slots:
void loggerWrite(const QString &str, Qt::GlobalColor color);
void loggerWriteCommandResult(EDCommand::EDCommandResult result);

};

#endif // TELEMETRY_H
