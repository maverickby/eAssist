#include <float.h>
#include "TelemetryPlot.h"

//-----------------------------------------------------------------------------------------------------
TelemetryPlot::TelemetryPlot(QWidget *parent) :
    QCustomPlot(parent),
    m_marker_mode(false),
    m_mouse_down(false),
    m_marker_left_position(-DBL_MAX),
    m_marker_right_position(DBL_MAX),
    m_numpoints(1000),
    m_runtime_autoscale(false)
{
    setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    //%%%setBackground(QBrush(QColor(qRgb(46, 74, 95))));

    // left marker
    m_marker_left = new QCPItemStraightLine(this);
    m_marker_left->setPen(QPen(QBrush(Qt::darkGreen), 1, Qt::DashLine));
    addItem(m_marker_left);
    m_marker_left->setVisible(false);

    // right marker
    m_marker_right = new QCPItemStraightLine(this);
    m_marker_right->setPen(QPen(QBrush(Qt::darkRed), 1, Qt::DashLine));
    addItem(m_marker_right);
    m_marker_right->setVisible(false);
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::Clear()
{
    plotLayout()->clear();
    clearGraphs();
    TracersClear(m_tracers_left);
    TracersClear(m_tracers_right);
    LabelsClear();
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::SetupPlot(const QVector<QString> &names, int numpoints, const QVector<TelData*> &tel_data, const float axisX_coef)
{
    m_numpoints = numpoints;
    m_tel_data = &tel_data;

    int size = names.size();

    const int c_num_ticks = 10;
    QVector<QString> labelx;
    if(axisX_coef > 0) for(int i = 0; i < c_num_ticks; i++) labelx.append( QString::number((static_cast<float>(numpoints)*i/static_cast<float>(c_num_ticks))*axisX_coef) );

    // cleanup
    Clear();

    if(size == 0) return;

    // create
    for(int i = 0; i < size; i++)
    {
        QCPAxisRect *tmp = new QCPAxisRect(this);
        tmp->axis(QCPAxis::atLeft, 0)->axisRect()->setAutoMargins(QCP::msNone);
        if(i < (size - 1)) // remove x axes ticks
        {
            tmp->axis(QCPAxis::atBottom, 0)->setTicks(false);
            tmp->axis(QCPAxis::atBottom, 0)->setTickLabels(false);
            tmp->axis(QCPAxis::atLeft, 0)->axisRect()->setMargins(QMargins(50, 2, 0, 0));
        } else
        {
            tmp->axis(QCPAxis::atLeft, 0)->axisRect()->setMargins(QMargins(50, 2, 0, 15));
            m_marker_left->point1->setAxes(tmp->axis(QCPAxis::atBottom, 0), tmp->axis(QCPAxis::atLeft, 0));
            m_marker_left->point2->setAxes(tmp->axis(QCPAxis::atBottom, 0), tmp->axis(QCPAxis::atLeft, 0));
            m_marker_right->point1->setAxes(tmp->axis(QCPAxis::atBottom, 0), tmp->axis(QCPAxis::atLeft, 0));
            m_marker_right->point2->setAxes(tmp->axis(QCPAxis::atBottom, 0), tmp->axis(QCPAxis::atLeft, 0));

            if(axisX_coef > 0)
            {
				QCPAxis *axis = tmp->axis(QCPAxis::atBottom, 0);

                axis->setAutoTickLabels(false);
				axis->setAutoTickStep(false);
				axis->setTickStep(numpoints / c_num_ticks);
				axis->setTickVectorLabels(labelx);
            }
        }
        tmp->axis(QCPAxis::atLeft, 0)->setLabel(names[i]);
        tmp->axis(QCPAxis::atBottom, 0)->setRange(0, m_numpoints);
        addGraph(tmp->axis(QCPAxis::atBottom, 0), tmp->axis(QCPAxis::atLeft, 0));
        //m_plot->graph()->setLineStyle(QCPGraph::lsStepLeft);
        plotLayout()->addElement(i, 0, tmp);
        graph()->setData(&tel_data[i]->getDataMap(), false);

        QCPItemTracer *tracer = new QCPItemTracer(this);
        m_tracers_left.append(tracer);
        tracer->setGraph(graph());
        tracer->setClipAxisRect(axisRect(i));
        tracer->setGraphKey(100*i);
        tracer->setInterpolating(true);
        tracer->setStyle(QCPItemTracer::tsCircle);
        tracer->setPen(QPen(Qt::red));
        tracer->setBrush(Qt::red);
        tracer->setSize(2);
        tracer->setVisible(false);
        addItem(tracer);

        tracer = new QCPItemTracer(this);
        m_tracers_right.append(tracer);
        tracer->setGraph(graph());
        tracer->setClipAxisRect(axisRect(i));
        tracer->setGraphKey(100*i);
        tracer->setInterpolating(true);
        tracer->setStyle(QCPItemTracer::tsCircle);
        tracer->setPen(QPen(Qt::red));
        tracer->setBrush(Qt::red);
        tracer->setSize(2);
        tracer->setVisible(false);
        addItem(tracer);

        QCPItemText *label = new QCPItemText(this);
        m_plot_labels.append(label);
        label->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
        label->position->setAxisRect(axisRect(i));
        label->setClipAxisRect(axisRect(i));
        label->position->setType(QCPItemPosition::ptAxisRectRatio);
        label->position->setCoords(0.8, 0);
        label->setText("0");
        label->setFont(QFont("Courier new", 10));
        label->setColor(Qt::red);
        label->setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
        label->setVisible(false);
        addItem(label);
    }

    // connect scaling on x axes all together
    for(int i = 0; i < axisRects().size(); i++)
    {
        for(int k = 0; k < axisRects().size(); k++)
        {
            if(i != k) connect( axisRects().at(i)->axis(QCPAxis::atBottom, 0), SIGNAL(rangeChanged(QCPRange)),
                                axisRects().at(k)->axis(QCPAxis::atBottom, 0), SLOT(setRange(QCPRange)));
        }
    }

    m_xdata.clear();
    for(int i = 0; i < m_numpoints; i++)
    {
        m_xdata.append(i);
    }

    setMarkerLeft(-DBL_MAX);
    setMarkerRight(DBL_MAX);

    setAntialiasedPlots(true);

    replot();
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::setTracersValue(QVector<QCPItemTracer*> &tracers, double position)
{
    int size = tracers.size();
    for(int i = 0; i < size; i++)
    {
        tracers[i]->setGraphKey(position);
    }
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::setTracersVisible(QVector<QCPItemTracer*> &tracers, bool value)
{
    int size = tracers.size();
    for(int i = 0; i < size; i++)
    {
        tracers[i]->setVisible(value);
    }
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::TracersClear(QVector<QCPItemTracer *> &tracers)
{
    int size = tracers.size();

    for(int i = 0; i < size; i++)
            removeItem(tracers.at(i));

    tracers.clear();
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::LabelsClear()
{
    int size = m_plot_labels.size();

    for(int i = 0; i < size; i++)
    {
         removeItem(m_plot_labels.at(i));
    }
    m_plot_labels.clear();
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::setLablesVisible(bool value)
{
    int size = m_plot_labels.size();

    for(int i = 0; i < size; i++)
    {
         m_plot_labels.at(i)->setVisible(value);
    }
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::setMarkerLeft(double position)
{
    m_marker_left_position = position;
    m_marker_left->point1->setCoords(position, 1);
    m_marker_left->point2->setCoords(position, 2);

    setTracersValue(m_tracers_left, position);
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::setMarkerRight(double position)
{
    m_marker_right_position = position;
    m_marker_right->point1->setCoords(position, 1);
    m_marker_right->point2->setCoords(position, 2);

    setTracersValue(m_tracers_right, position);
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::setAntialiasedPlots(bool value)
{
    int size = graphCount();

    for(int i = 0; i < size; i++) graph(i)->setAntialiased(value);

    replot();
}
//-----------------------------------------------------------------------------------------------------
double TelemetryPlot::getMarkerLeftPos() const
{
    return m_marker_left_position;
}
//-----------------------------------------------------------------------------------------------------
double TelemetryPlot::getMarkerRightPos() const
{
    return m_marker_right_position;
}
//-----------------------------------------------------------------------------------------------------
bool TelemetryPlot::getRuntimeAutoscale() const
{
    return m_runtime_autoscale;
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::setVisible(bool value)
{
    QCustomPlot::setVisible(value);
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::update_labels_info()
{
    int size = m_plot_labels.size();

    for(int i = 0; i < size; i++)
    {
        double pos1 = m_tracers_left.at(i)->position->value();
        double pos2 = m_tracers_right.at(i)->position->value();
        double delta = fabs(pos1 - pos2);
        m_plot_labels[i]->setText(QString::number(pos1) + " : " +
                                  QString::number(pos2) + " : " +
                                  QString::number(delta));
    }
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::replot()
{
    // scaling
    int size = m_tel_data->size();
    for(int i = 0; i < size; i++)
    {
        axisRects().at(i)->axis(QCPAxis::atBottom, 0)->setRange( m_tel_data->at(i)->getXRange());

        if(m_runtime_autoscale)
        {
            QCPRange tmp = m_tel_data->at(i)->getYRange();
            double delta = 0.2*fabs(tmp.upper - tmp.lower);
            if( (axisRects().at(i)->axis(QCPAxis::atLeft, 0)->range().upper < tmp.upper) ||
                (axisRects().at(i)->axis(QCPAxis::atLeft, 0)->range().lower > tmp.lower) )
            {
                tmp.upper = tmp.upper + delta;
                tmp.lower = tmp.lower - delta;
                axisRects().at(i)->axis(QCPAxis::atLeft, 0)->setRange(tmp);
            }
        }
    }

    QCustomPlot::replot();
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::mouseMoveEvent(QMouseEvent * event)
{
    QCustomPlot::mouseMoveEvent(event);

    if(m_mouse_down)
    {
        double value = axisRects().at(0)->axis(QCPAxis::atBottom, 0)->pixelToCoord(event->x());

        if(m_marker_mode)
        {
            if( (fabs(value - m_marker_left_position)) > (fabs(value - m_marker_right_position)) ) setMarkerRight(value);
            else setMarkerLeft(value);
        } else
        {
            setMarkerLeft(value);
        }
        emit MarkersChange(m_marker_left_position, m_marker_right_position);
        update_labels_info();
        replot();
    }
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::mousePressEvent(QMouseEvent *event)
{
    QCustomPlot::mousePressEvent(event);

    if(event->button() == Qt::RightButton)
    {
        m_mouse_down = true;

        if(!m_marker_mode)
        {
            m_marker_left->setVisible(true);
            setTracersVisible(m_tracers_left, true);
            setLablesVisible(true);
        }
        replot();
    }
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::mouseReleaseEvent(QMouseEvent *event)
{
    QCustomPlot::mouseReleaseEvent(event);

    if(event->button() == Qt::RightButton)
    {
        m_mouse_down = false;

        if(!m_marker_mode)
        {
            m_marker_left->setVisible(false);
            setTracersVisible(m_tracers_left, false);
            setLablesVisible(false);
        }
        replot();
    }
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::mouseDoubleClickEvent(QMouseEvent *event)
{
    (void) event;

    m_marker_mode = !m_marker_mode;

    m_marker_left->setVisible(m_marker_mode);
    m_marker_right->setVisible(m_marker_mode);
    setTracersVisible(m_tracers_left, m_marker_mode);
    setTracersVisible(m_tracers_right, m_marker_mode);
    setLablesVisible(m_marker_mode);
    update_labels_info();

    // change markers position if it outside of view
    QCPRange range = axisRects().at(0)->axis(QCPAxis::atBottom, 0)->range();

    if(range.lower > m_marker_left_position) setMarkerLeft(range.lower + (range.upper - range.lower)/10);
    if(range.upper < m_marker_right_position) setMarkerRight(range.upper - (range.upper - range.lower)/10);

    replot();
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::autoscale()
{
    rescaleAxes();

    int size = m_tel_data->size();
    for(int i = 0; i < size; i++)
    {
        QCPRange tmp = m_tel_data->at(i)->getYRange(true);
        axisRects().at(i)->axis(QCPAxis::atBottom, 0)->setRange( m_tel_data->at(i)->getXRange());
        axisRects().at(i)->axis(QCPAxis::atLeft, 0)->setRange( tmp );
    }

    replot();
}
//-----------------------------------------------------------------------------------------------------
void TelemetryPlot::runtime_autoscale(bool value)
{
    m_runtime_autoscale = value;
}
