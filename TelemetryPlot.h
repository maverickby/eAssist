#ifndef TELEMETRYPLOT_H
#define TELEMETRYPLOT_H

#include "QCP/qcustomplot.h"
#include "TelData.h"

class TelemetryPlot : protected QCustomPlot
{
    Q_OBJECT
public:
    explicit TelemetryPlot(QWidget *parent = 0);

    // when axisX_coef > 0: xlabels count = 10 and they values scaled with axisX_coef
    void SetupPlot(const QVector<QString> &names, int numpoints, const QVector<TelData*> &tel_data, const float axisX_coef = 0);
    void Clear();
    void setAntialiasedPlots(bool value);

    double getMarkerLeftPos() const;
    double getMarkerRightPos() const;

    bool getRuntimeAutoscale() const;

    void setVisible(bool value);

protected:
    virtual void mouseMoveEvent         (QMouseEvent *event);
    virtual void mousePressEvent        (QMouseEvent *event);
    virtual void mouseReleaseEvent      (QMouseEvent *event);
    virtual void mouseDoubleClickEvent  (QMouseEvent *event);

private:
    void setTracersValue(QVector<QCPItemTracer*> &tracers, double position);
    void setTracersVisible(QVector<QCPItemTracer*> &tracers, bool value);
    void TracersClear(QVector<QCPItemTracer*> &tracers);

    void LabelsClear();
    void setLablesVisible(bool value);

    void setMarkerLeft(double position);
    void setMarkerRight(double position);

    void update_labels_info();

    QVector<QCPItemTracer*> m_tracers_left;
    QVector<QCPItemTracer*> m_tracers_right;
    QVector<QCPItemText*>   m_plot_labels;
    QCPItemStraightLine     *m_marker_left;
    QCPItemStraightLine     *m_marker_right;
    bool    m_marker_mode;
    bool    m_mouse_down;
    double  m_marker_left_position;
    double  m_marker_right_position;

    int m_numpoints;
    const QVector<TelData*> *m_tel_data;
    QVector<double> m_xdata;
    bool m_runtime_autoscale;

signals:
    void MarkersChange(double marker_left_pos, double marler);

public slots:
    void replot();
    void autoscale();
    void runtime_autoscale(bool value);

};

#endif // TELEMETRYPLOT_H
