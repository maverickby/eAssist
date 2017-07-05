#include "SignalsDockWidget.h"

//----------------------------------------------------------------------------------------------
SignalsDockWidget::SignalsDockWidget(ED *ed, QWidget *parent) :
    QDockWidget("Signals control", parent)
{
    m_container = new SignalsWidget(ed, this);

    this->setMinimumWidth(200);

    act_read = new QAction(tr("Read"), this);
    connect(act_read, SIGNAL(triggered()), this, SLOT(read()));

    act_save = new QAction(tr("Save"), this);
    connect(act_save, SIGNAL(triggered()), this, SLOT(save()));

    act_cycleread = new QAction(tr("Cycle read"), this);
    act_cycleread->setCheckable(true);
    connect(act_cycleread, SIGNAL(triggered()), this, SLOT(cycleread()));

    act_hex = new QAction(tr("Hex"), this);
    act_hex->setCheckable(true);
    connect(act_hex, SIGNAL(triggered()), this, SLOT(hex()));

    m_window = new QMainWindow(nullptr);
    m_toolbar = new QToolBar(m_window);
    m_toolbar->setFloatable(false);
    m_toolbar->setMovable(false);
    m_toolbar->addAction(act_read);
    m_toolbar->addAction(act_save);
    m_toolbar->addAction(act_cycleread);
    m_toolbar->addAction(act_hex);

    m_scrollarea = new QScrollArea(m_window);
    m_container->setParent(m_scrollarea);
    m_scrollarea->setWidget(m_container);
    m_scrollarea->setWidgetResizable(true);
    m_scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_window->addToolBar(m_toolbar);
    m_window->setCentralWidget(m_scrollarea);

    m_window->setParent(this);
    this->setWidget(m_window);
}
//----------------------------------------------------------------------------------------------
void SignalsDockWidget::setSignals(QVector<EDSignal*> &signals_list) //-V2009
{
    m_container->setSignals(signals_list);
    m_container->readSignals();
}
//----------------------------------------------------------------------------------------------
void SignalsDockWidget::read()
{
    act_read->setEnabled(false);

    m_container->readSignals();

    act_read->setEnabled(true);
}
//----------------------------------------------------------------------------------------------
void SignalsDockWidget::save()
{
    m_container->saveSignals();
}
//----------------------------------------------------------------------------------------------
void SignalsDockWidget::cycleread()
{
    m_container->setCycleRead(act_cycleread->isChecked());
}
//----------------------------------------------------------------------------------------------
void SignalsDockWidget::hex()
{
    m_container->setViewHex(act_hex->isChecked());
    if(!act_cycleread->isChecked()) m_container->readSignals();
}
//----------------------------------------------------------------------------------------------
void SignalsDockWidget::closeEvent(QCloseEvent *event)
{
    m_container->setCycleRead(false);
    act_cycleread->setChecked(false);
    event->accept();
}
