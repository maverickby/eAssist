#ifndef TELEMETRYWIDGET_H
#define TELEMETRYWIDGET_H

#include <QWidget>
#include <QList>
#include <QVBoxLayout>
#include <QToolBar>
#include <QStatusBar>
#include <QComboBox>
#include <QAction>
#include <QSpinBox>
#include <QTimer>
#include <QLabel>
#include "ED/ed.h"
#include "TelemetryPlot.h"
#include "Telemetry.h"
//#include "mainwindow.h"

#define DRAW_TIMER_PERIOD 25

class MainWindow;

// Telemetry_t - bridge from DSPA to Plot subsystem
class TelemetryWidget: public QWidget
{
    Q_OBJECT
public:
    explicit TelemetryWidget(QWidget *parent, ED & ed, MainWindow* mainWin);
    ~TelemetryWidget();

    void setPeriod(uint32_t value);
	void stop();
	Telemetry* getTelemetry();
	

private:
    void Clear();
    void setup_toolbar();
    void update_plot();
    float getCoef();
    double to_ms(double value);

    // controls
    QVBoxLayout    *m_vboxlayout;
    QToolBar        m_toolbar;
    QComboBox      *m_cb_samplerate;
    QSpinBox       *m_sb_numpoints;
    QComboBox      *m_cb_graphtype;
    TelemetryPlot  *m_plot;
    Telemetry       m_telemetry;

    QLabel      *m_label_statistics;

    // actions
    QAction *m_act_mode;
    QAction *m_act_export_csv;
    QAction *m_act_autoscale;
    QAction *m_act_runtime_autoscale;

    QLabel *m_label_left_marker;
    QLabel *m_label_right_marker;
    QLabel *m_label_delta;
    QLabel *m_label_freq;

    TelStatistics m_statistics;

    ED & m_ed;
	MainWindow* m_mainWindow;
    int m_numpoints;

private slots:
    void NewSignals();
    void setSignalMode(EDSignal *signal);
    void Statistics(const TelStatistics & statistics);
    void NewDescriptor();

    void toggle_mode();
    void export_csv();
    void samplerate_changed(int value);
    void numpoints_changed(int value);
    void changeWrapMode(int value);


    void update_markers_info(double marker_left_pos, double marker_right_pos);

    void Start();
    void Pause();
    void Stop();
    
};

#endif // TELEMETRYWIDGET_H
