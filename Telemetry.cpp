#include <cstdint>
#include "Telemetry.h"
#include "ED/EDLogger.h"
#include "Settings.h"
#include "threadtelreaddata.h"

#define TIMER_INTERVAL 40

//--------------------------------------------------------------------------------------------
Telemetry::Telemetry(ED *ed, QObject *parent) :
    QObject(parent),
    m_ed(ed),
    m_running(false),
    m_wrapmode(true),
    request_to_stop(false),
    m_numpoints(4000),
    m_threadTelReadData(&m_ed->Tel())
{
    // timer
    m_timer.setInterval(TIMER_INTERVAL);
    m_timer.setSingleShot(true);
    m_timer.setTimerType(Qt::PreciseTimer);

    connect( &m_timer,  SIGNAL(timeout()),
             this,      SLOT(tick()) );
}
//--------------------------------------------------------------------------------------------
Telemetry::~Telemetry()
{
    m_threadTelReadData.stop();
    Clear();
}
//--------------------------------------------------------------------------------------------
void Telemetry::setSignalMode(const EDSignal *signal)
{
    if(signal->getIntelemetry())
    {
        if(qFind(m_tel_signals, signal->getSignal_id()) == m_tel_signals.end())
            m_tel_signals.append(signal->getSignal_id());
    } else
    {
        if(qFind(m_tel_signals, signal->getSignal_id()) != m_tel_signals.end())
            m_tel_signals.removeOne(signal->getSignal_id());
    }

    setup_tel_data();
}
//--------------------------------------------------------------------------------------------
void Telemetry::setNumPoints(int numpoints)
{
    m_numpoints = numpoints;

    setup_tel_data();
}
//--------------------------------------------------------------------------------------------
void Telemetry::setup_tel_data()
{
    m_tel_data.clear();
    foreach (TelData *item, m_tel_data) delete item;

    int size = m_tel_signals.size();

    for(int i = 0; i < size; i++)
    {
        m_tel_data.append(new TelData(m_numpoints));
        m_tel_data.last()->setWrapMode(m_wrapmode);
    }
}
//--------------------------------------------------------------------------------------------
void Telemetry::Clear()
{
    m_tel_signals.clear();
    m_tel_data.clear();
    foreach (TelData *item, m_tel_data) delete item;
}
//--------------------------------------------------------------------------------------------
bool Telemetry::isRunning() const
{
    return m_running;
}
//--------------------------------------------------------------------------------------------
int Telemetry::getNumSignals() const
{
    return m_tel_signals.size();
}
//--------------------------------------------------------------------------------------------
void Telemetry::setWrapMode(bool mode)
{
    m_wrapmode = mode;

    int size = m_tel_data.size();

    for(int i = 0; i < size; i++)
    {
        m_tel_data.at(i)->setWrapMode(m_wrapmode);
    }
}
//--------------------------------------------------------------------------------------------
const QVector<TelData *> *Telemetry::getTelData()
{
    return &m_tel_data;
}
//--------------------------------------------------------------------------------------------
void Telemetry::getSignalNames(QVector<QString> &names)
{
    int size = m_tel_signals.size();

    names.clear();

    for(int i = 0; i < size; i++)
    {
        names << m_ed->Signals().at(m_tel_signals.at(i)).getDescriptor().name;
    }
}
//--------------------------------------------------------------------------------------------
void Telemetry::tick()
{
    int num_signals = m_tel_signals.size();

    m_threadTelReadData.lock();
        m_statistics = m_threadTelReadData.getStatistics();
    m_threadTelReadData.unlock();

	if (tel_buffer->size() > 0)//Добавление в телеметрию сигнала при запущенной телеметрии другого сигнала приводило к падению приложения
	{
		for (int s = 0; s < num_signals && s < tel_buffer->size(); s++)
		{
			EDTel_SignalDataBuffer *buffer = &(*tel_buffer)[s];

			m_threadTelReadData.lock();
			int num_samples = buffer->getNumSamples();
			for (int i = 0; i < num_samples; i++)
			{
				switch (m_ed->Signals().at(m_tel_signals.at(s)).getDescriptor().type)
				{
				case STYPE_BOOL:  m_tel_data[s]->addData(buffer->getValue<bool>(i));   break;
				case STYPE_BYTE:  m_tel_data[s]->addData(buffer->getValue<uint8_t>(i));   break;
				case STYPE_UINT:  m_tel_data[s]->addData(buffer->getValue<uint16_t>(i));   break;
				case STYPE_INT:   m_tel_data[s]->addData(buffer->getValue<int16_t>(i));   break;
				case STYPE_LONG:  m_tel_data[s]->addData(buffer->getValue<int32_t>(i));   break;
				case STYPE_ULONG: m_tel_data[s]->addData(buffer->getValue<uint32_t>(i));   break;
				case STYPE_FLOAT: m_tel_data[s]->addData(buffer->getValue<float>(i));   break;
				}
			}
			buffer->ClearSamples(); // clear buffered data
			m_threadTelReadData.unlock();
		}
	}

    emit DataReady();
    emit Statistics(m_statistics);

    if(request_to_stop)
    {
        m_threadTelReadData.stop();
        m_ed->Tel().Stop();
        m_running = false;
        request_to_stop = false;
    }
    else m_timer.start(TIMER_INTERVAL);
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void Telemetry::Start(ulong coef)
{
    if(m_running) return;

    int size = m_tel_data.size();
    for(int i = 0; i < size; i++) m_tel_data[i]->clear();
    m_threadTelReadData.ResetStatistics();

	EDTel &tel = m_ed->Tel();

    tel.Stop(); // stop telemetry for case when it started from previous session

    if(m_tel_signals.size() == 0)
    {
        logger().WriteLn("Can not start telemetry without signals", Qt::red);
        return;
    }

    m_statistics.datarequests = 0;
    m_statistics.num_samples = 0;
    m_statistics.errors = 0;
    m_statistics.state_overflow = 0;
    m_statistics.state_wait = 0;

    // add signals to remote site telemetry list
    size = m_tel_signals.size();
    for(int i = 0; i < size; i++)
    {
        tel.AddSignal(m_tel_signals.at(i));
    }

    // get pointer to data buffer
    tel_buffer = &tel.getDataBuffer();

    // start
    tel.Start(coef);

    m_running = true;

    request_to_stop = false;

    m_timer.start(TIMER_INTERVAL);

	bool connectRes = false;
	connectRes= connect(&m_threadTelReadData, SIGNAL(SignalLoggerWrite(const QString &, Qt::GlobalColor)), this, SLOT(loggerWrite(const QString &, Qt::GlobalColor)), Qt::QueuedConnection);
	connectRes = connect(&m_threadTelReadData, SIGNAL(SignalLoggerWriteCommandResult(EDCommand::EDCommandResult)), this, SLOT(loggerWriteCommandResult(EDCommand::EDCommandResult)), Qt::QueuedConnection);

    m_threadTelReadData.start(QThread::TimeCriticalPriority);
}

void Telemetry::loggerWrite(const QString &str, Qt::GlobalColor color)
{
	logger().Write(str, color);
}

void Telemetry::loggerWriteCommandResult(EDCommand::EDCommandResult result)
{
	logger().WriteCommandResult(result);
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void Telemetry::Pause()
{
    if(!m_running) return;

    m_threadTelReadData.stop();

    m_timer.stop();

    m_ed->Tel().Pause();
    m_running = false;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void Telemetry::Stop()
{
    request_to_stop = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void Telemetry::setPeriod(uint32_t value)
{
    m_threadTelReadData.setPeriod(value);
}


void Telemetry::StopThreadTelemetryReadData()
{
	m_threadTelReadData.stop();
	m_ed->Tel().Stop();
	m_running = false;
	request_to_stop = false;	
}