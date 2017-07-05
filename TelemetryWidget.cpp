#include <cstdint>
#include <float.h>
#include <QFileDialog>
#include "TelemetryWidget.h"
#include "ED/EDLogger.h"

static const int coefs[] = {1, 2, 5, 10, 20, 25, 50, 100, 200};

//------------------------------------------------------------------------------------------------------------------------------------------------
TelemetryWidget::TelemetryWidget(QWidget *parent, ED & ed) :
    QWidget(parent),
    m_ed(ed),
    m_telemetry(&ed, this),
    m_numpoints(1000)
{
    // GUI setup
    m_vboxlayout = new QVBoxLayout(this);
    m_plot = new TelemetryPlot(this);
    setup_toolbar();
    m_vboxlayout->addWidget(reinterpret_cast<QWidget*>(m_plot));
    m_plot->setVisible(false); // untill start adding signals
    m_toolbar.setVisible(false);

    connect(reinterpret_cast<QWidget*>(m_plot), SIGNAL(MarkersChange(double, double)),
            this,                               SLOT(update_markers_info(double, double)));

    connect(&m_telemetry,                       SIGNAL(DataReady()),
            reinterpret_cast<QWidget*>(m_plot), SLOT(replot()));

    connect(&m_telemetry, SIGNAL(Statistics(const TelStatistics&)),
            this,         SLOT  (Statistics(const TelStatistics&)));

    // connect to ED callbacks
    connect(&m_ed.Signals(),  SIGNAL(SignalTelModeChanged(EDSignal*)),
            this,             SLOT(setSignalMode(EDSignal*)) );

    connect(&m_ed.Signals(),  SIGNAL(NewSignalsList()),
            this,             SLOT(NewSignals()) );

    connect(&m_ed,            SIGNAL(NewTeldescriptor()),
            this,             SLOT(NewDescriptor()));
}
//------------------------------------------------------------------------------------------------------------------------------------------------
TelemetryWidget::~TelemetryWidget()
{
    Clear();
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void TelemetryWidget::setPeriod(uint32_t value)
{
    m_telemetry.setPeriod(value);    
}
//------------------------------------------------------------------------------------------------------------------------------------------------
// clear and return to initial state
void TelemetryWidget::Clear()
{
    Stop();

    m_telemetry.Clear();
    m_plot->Clear();

    m_plot->setVisible(false);
    m_toolbar.setVisible(false);
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void TelemetryWidget::setup_toolbar()
{
    // start-stop button
    m_act_mode =  new QAction(this);
    m_act_mode->setIcon(QIcon(":/images/start.png"));
    connect(m_act_mode, SIGNAL(triggered()), this, SLOT(toggle_mode()));
    m_toolbar.addAction(m_act_mode);

    // export to CSV button
    m_act_export_csv =  new QAction(this);
    m_act_export_csv->setIcon(QIcon(":/images/save_csv.png"));
    connect(m_act_export_csv, SIGNAL(triggered()), this, SLOT(export_csv()));
    m_toolbar.addAction(m_act_export_csv);
    m_act_export_csv->setEnabled(false);

    m_toolbar.addSeparator();

    // Sample rate selector
    m_toolbar.addWidget(new QLabel("Sample rate: ", this));
    m_cb_samplerate = new QComboBox(this);
    m_toolbar.addWidget(m_cb_samplerate);
    connect(m_cb_samplerate, SIGNAL(currentIndexChanged(int)), this, SLOT(samplerate_changed(int)));
    m_toolbar.addSeparator();

    // Num points
    m_toolbar.addWidget(new QLabel("Num points: ", this));
    m_sb_numpoints = new QSpinBox(this);
    m_sb_numpoints->setMaximum(1000000);
    m_sb_numpoints->setMinimum(100);
    m_sb_numpoints->setValue(m_numpoints);
    m_sb_numpoints->setAccelerated(true);
    m_sb_numpoints->setSingleStep(500);
    connect(m_sb_numpoints, SIGNAL(valueChanged(int)), this, SLOT(numpoints_changed(int)));
    m_toolbar.addWidget(m_sb_numpoints);
    m_toolbar.addSeparator();

    // Graph type
    m_toolbar.addWidget(new QLabel("Graph type: ", this));
    m_cb_graphtype = new QComboBox(this);
    QStringList type_list;
    type_list << "auto" << "roll";
    m_cb_graphtype->addItems(type_list);
    m_toolbar.addWidget(m_cb_graphtype);
    connect(m_cb_graphtype, SIGNAL(currentIndexChanged(int)), this, SLOT(changeWrapMode(int)));
    m_toolbar.addSeparator();

    //Autoscale
    m_act_autoscale = new QAction(this);
    m_act_autoscale->setIcon(QIcon(":/images/autoscale.png"));
    connect(m_act_autoscale, SIGNAL(triggered()), reinterpret_cast<QWidget*>(m_plot), SLOT(autoscale()));
    m_toolbar.addAction(m_act_autoscale);

    //Autoscale in run state
    m_act_runtime_autoscale = new QAction(this);
    m_act_runtime_autoscale->setText("auto");
    m_act_runtime_autoscale->setCheckable(true);
    m_toolbar.addAction(m_act_runtime_autoscale);
    connect(m_act_runtime_autoscale, SIGNAL(triggered(bool)), reinterpret_cast<QWidget*>(m_plot), SLOT(runtime_autoscale(bool)));
    m_toolbar.addSeparator();

    //Markers information
    m_label_left_marker = new QLabel(this);
    m_label_left_marker->setFixedWidth(70);
    m_toolbar.addWidget(m_label_left_marker);

    m_label_right_marker = new QLabel(this);
    m_label_right_marker->setFixedWidth(70);
    m_toolbar.addWidget(m_label_right_marker);

    m_label_delta = new QLabel(this);
    m_label_delta->setFixedWidth(80);
    m_toolbar.addWidget(m_label_delta);

    m_label_freq = new QLabel(this);
    m_label_freq->setFixedWidth(100);
    m_toolbar.addWidget(m_label_freq);

    update_markers_info(m_plot->getMarkerLeftPos(), m_plot->getMarkerRightPos());

    //Statistics
    m_label_statistics = new QLabel(this);
    m_label_statistics->setText("");
    m_toolbar.addWidget(m_label_statistics);

    m_vboxlayout->addWidget(&m_toolbar); // add toolbar to layout
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void TelemetryWidget::update_plot()
{
    m_telemetry.setNumPoints(m_numpoints);

    QVector<QString> vect_tmp;
    m_telemetry.getSignalNames(vect_tmp);
    m_plot->SetupPlot(vect_tmp, m_numpoints, *m_telemetry.getTelData());

    int size = m_telemetry.getNumSignals();
    m_plot->setVisible(size > 0);
    m_toolbar.setVisible(size > 0);
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void TelemetryWidget::setSignalMode(EDSignal *signal) //-V2009
{
    bool tmp_running = m_telemetry.isRunning();

    // stop
    m_telemetry.Stop();

    m_telemetry.setSignalMode(signal);

    update_plot();

    m_act_export_csv->setEnabled(false);

    // start if needed
    if(tmp_running && m_telemetry.getNumSignals() > 0) Start();
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void TelemetryWidget::Statistics(const TelStatistics &statistics)
{
    m_statistics = statistics;

    QString tmp = QString("Requests: ") + QString::number(m_statistics.datarequests, 10) + " " +
                  QString("Samples: ")  + QString::number(m_statistics.num_samples, 10) + " " +
                  QString("Errors: ")   + QString::number(m_statistics.errors, 10) + " " +
                  QString("Overflow: ") + QString::number(m_statistics.state_overflow, 10) + " " +
                  QString("Wait: ")     + QString::number(m_statistics.state_wait, 10);
    m_label_statistics->setText(tmp);
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void TelemetryWidget::NewDescriptor()
{
    QStringList samplerate_list;

    for(int i = 0; i < sizeof(coefs)/sizeof(coefs[0]); i++)
    {
        unsigned int freq = 100000000/(m_ed.Tel().Descriptor().period*coefs[i]);
        samplerate_list << QString::number(freq);
    }
    m_cb_samplerate->clear();
    m_cb_samplerate->addItems(samplerate_list);
    m_cb_samplerate->setCurrentIndex(0);

    m_act_export_csv->setEnabled(false);
}
//------------------------------------------------------------------------------------------------------------------------------------------------
float TelemetryWidget::getCoef()
{
    float res = coefs[m_cb_samplerate->currentIndex()];
    return res;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void TelemetryWidget::update_markers_info(double marker_left_pos, double marker_right_pos)
{
    double delta = fabs(marker_right_pos - marker_left_pos);
    double freq = 1.0/to_ms(delta)*1000.0;

    m_label_left_marker->   setText("x1: " + QString::number(to_ms(marker_left_pos), 'f', 3));
    m_label_right_marker->  setText("x2: " + QString::number(to_ms(marker_right_pos), 'f', 3));
    m_label_delta->         setText("delta: " + QString::number(to_ms(delta), 'f', 3));
    m_label_freq->          setText("freq: " + QString::number(freq, 'f', 3));
}

//------------------------------------------------------------------------------------------------------------------------------------------------
double TelemetryWidget::to_ms(double value)
{
    double res = value*static_cast<double>(m_ed.Tel().Descriptor().period)*0.01*getCoef()/1000.0;
    return res;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void TelemetryWidget::NewSignals()
{
    Clear();
    m_act_export_csv->setEnabled(false);
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void TelemetryWidget::Start()
{
    if(m_telemetry.isRunning()) return;

    // start
    m_telemetry.Start(getCoef());

    m_plot->setAntialiasedPlots(false);

    m_act_mode->setIcon(QIcon(":/images/stop.png"));
    m_cb_samplerate->setEnabled(false);
    m_sb_numpoints->setEnabled(false);
    m_cb_graphtype->setEnabled(false);

    m_act_export_csv->setEnabled(false);
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void TelemetryWidget::Pause()
{
    if(!m_telemetry.isRunning()) return;

    m_telemetry.Pause();
    m_plot->setAntialiasedPlots(true);

    m_act_mode->setIcon(QIcon(":/images/start.png"));
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void TelemetryWidget::Stop()
{
    if(!m_telemetry.isRunning()) return;
    m_telemetry.Stop();
    m_plot->setAntialiasedPlots(true);

    m_act_mode->setIcon(QIcon(":/images/start.png"));
    m_cb_samplerate->setEnabled(true);
    m_sb_numpoints->setEnabled(true);
    m_cb_graphtype->setEnabled(true);

    m_act_export_csv->setEnabled(true);
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void TelemetryWidget::toggle_mode()
{
    if(m_telemetry.isRunning())  Stop();
    else Start();
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void TelemetryWidget::export_csv()
{
    QFileDialog dialog;

    QString filtr = "*.csv";
    dialog.setFileMode(QFileDialog::ExistingFile);
    QString filename = dialog.getSaveFileName(this, "Export to CSV file", "d:/", "CSV files (*.csv)", &filtr);
    if(filename.isEmpty()) return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);

    // write out data
    QVector<QString> tmp_names;
    m_telemetry.getSignalNames(tmp_names);
    QVector<TelData *> tmp = *m_telemetry.getTelData();

    // names
    out << "TIME";
    for(int i = 0; i < tmp_names.size(); i++)
    {
        out << "," << tmp_names[i];
    }
    out << '\n';

    // signals value
    float time_delta = 0.00000001f*m_ed.Tel().Descriptor().period*getCoef();
    QVector<QVector<double>> values;
    values.reserve(tmp.size());
    for(int i = 0; i < tmp.size(); i++)
        values.append(tmp[i]->getDataVector());

    for(int i = 0; i < values[0].size(); i++)
    {
        out << time_delta*i;
        for(int k = 0; k < values.size(); k++)
        {
            out << "," << values[k][i];
        }
        out << '\n';
    }

    file.close();
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void TelemetryWidget::samplerate_changed(int value)
{
    (void)value;

    update_markers_info(m_plot->getMarkerLeftPos(), m_plot->getMarkerRightPos());

    m_act_export_csv->setEnabled(false);
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void TelemetryWidget::numpoints_changed(int value)
{
    m_numpoints = value;
    update_plot();

    m_act_export_csv->setEnabled(false);
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void TelemetryWidget::changeWrapMode(int value)
{
    bool wrap_mode = value == 0;
    m_telemetry.setWrapMode(wrap_mode);
}


