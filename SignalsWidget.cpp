#include <cstdint>
#include "SignalsWidget.h"
#include "ED/EDLogger.h"

#define TIMER_PERIOD 300

//-------------------------------------------------------------------------------------------
SignalsWidget::SignalsWidget(ED *ed, QWidget *parent) :
    QWidget(parent),
    m_ed(ed)
{
    m_viewhex = false;

    m_hlayout =         new QHBoxLayout(this);
    m_vlayout_labels =  new QVBoxLayout(this);
    m_vlayout_ctrls =   new QVBoxLayout(this);
    m_splitter =        new QSplitter(this);
    m_widget_labels =   new QWidget(this);
    m_widget_ctrls =    new QWidget(this);

    m_vlayout_labels->addStretch();
    m_vlayout_ctrls->addStretch();

    m_widget_labels->setLayout(m_vlayout_labels);
    m_widget_ctrls->setLayout(m_vlayout_ctrls);

    m_splitter->setOrientation(Qt::Horizontal);
    m_splitter->addWidget(m_widget_labels);
    m_splitter->addWidget(m_widget_ctrls);

    m_hlayout->addWidget(m_splitter);

    this->setLayout(m_hlayout);

    m_timer = new QTimer(this);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timer_tick()));

    // signal mapping
    m_signalMapper = new QSignalMapper(this);

    connect(&m_ed->Signals(), SIGNAL(SignalValueChanged(EDSignal*)), this, SLOT(processSignalValueChanged(EDSignal*)));
    connect(&m_ed->Signals(), SIGNAL(NewSignalsList()), this, SLOT(NewSignals()));
}
//-------------------------------------------------------------------------------------------
SignalsWidget::~SignalsWidget()
{
    clear();
}
//-------------------------------------------------------------------------------------------
void SignalsWidget::setSignals(const QVector<EDSignal*> &signals_list)
{
    EDSignal *sig;
    QLabel *pLabel;
    QWidget *pWidgetCtrl;

    if(signals_list.size() == 0) return;

    bool cycleread_enabled = m_timer->isActive();
    m_timer->stop();

    clear();

    m_signals_list = signals_list;

    int size = m_signals_list.size();
    if(m_signals_list.size() > 0)
    {
        for(int i = 0; i < size; i++)
        {
            sig = m_signals_list[i];
            pLabel = new QLabel(sig->getDescriptor().name, this);
            pLabel->setMaximumHeight(20);
            pLabel->setMinimumHeight(20);
            if(sig->getDescriptor().type_class == SCL_STD)
            {
                if(sig->getDescriptor().type == STYPE_BOOL)
                {
                    pWidgetCtrl = new QCheckBox("", this);
                    connect(pWidgetCtrl, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
                    m_signalMapper->setMapping(pWidgetCtrl, pWidgetCtrl);
                }
                else
                {
                   pWidgetCtrl = new QLineEdit("", this);
                   connect(pWidgetCtrl, SIGNAL(textEdited(const QString &)), m_signalMapper, SLOT(map()));
                   connect(pWidgetCtrl, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
                   m_signalMapper->setMapping(pWidgetCtrl, pWidgetCtrl);
                }
            }
            pWidgetCtrl->setMaximumHeight(20);
            pWidgetCtrl->setMinimumHeight(20);

            m_vlayout_labels->insertWidget(i, pLabel);
            m_list_labels.append(pLabel);
            m_vlayout_ctrls->insertWidget(i, pWidgetCtrl);
            m_list_ctrls.append(pWidgetCtrl);
        }
    }

    changed_clear();

    if(cycleread_enabled) m_timer->start(TIMER_PERIOD);
}
//-------------------------------------------------------------------------------------------
void SignalsWidget::processSignalValueChanged(EDSignal* signal)
{
    int size = m_signals_list.size();
    QWidget *pWidgetCtrl;
    QString name;
    QString tmp_value;

    for(int i = 0; i < size;  i++)
    {
        if(m_signals_list.at(i) == signal) // update value
        {
            pWidgetCtrl = m_list_ctrls.value(i);
            name = pWidgetCtrl->metaObject()->className();

            tmp_value = EDSignal::SignalValueToString(*signal, m_viewhex);
            if(name.toLatin1() == "QLineEdit")
            {
                static_cast<QLineEdit*>(pWidgetCtrl)->setText(tmp_value);
            }
            else if(name.toLatin1() == "QCheckBox")
            {
				static_cast<QCheckBox*>(pWidgetCtrl)->setChecked(*static_cast<const bool*>(signal->getValue()));
            }
            break;
        }
    }
}
//-------------------------------------------------------------------------------------------
void SignalsWidget::readSignals()
{
    int size = m_signals_list.size();
    EDSignal *sig;
    QString name;
    QWidget *pWidgetCtrl;
    QString tmp_value;

    disconnect(m_signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(ctrl_changed(QWidget*)));

    if(m_signals_list.size() > 0)
    {
        m_ed->setActiveInterface(true);
        for(int i = 0; i < m_signals_list.size(); i++)
        {
            sig = m_signals_list[i];
            //pWidgetCtrl = m_list_ctrls.value(i);
            //name = pWidgetCtrl->metaObject()->className();

            bool pause_tmp = logger().getPauseLogging();
            logger().setPauseLogging(true);
            bool res = m_ed->Signals().getSignalValueStr(*sig, tmp_value, m_viewhex);

            pWidgetCtrl = m_list_ctrls.value(i);

            if(pWidgetCtrl == 0 || m_list_ctrls.size()<size || m_signals_list.size()<size)
            {
                m_ed->setActiveInterface(false);
                connect(m_signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(ctrl_changed(QWidget*)));
                changed_clear();
                return;
            }

            name = pWidgetCtrl->metaObject()->className();
            logger().setPauseLogging(pause_tmp);

            if(res)
            {
                if(qobject_cast<QLineEdit *>(pWidgetCtrl) && name.toLatin1() == "QLineEdit")
                {
					static_cast<QLineEdit*>(pWidgetCtrl)->setText(tmp_value);
                }
                else if(qobject_cast<QCheckBox *>(pWidgetCtrl) && name.toLatin1() == "QCheckBox")
                {
					static_cast<QCheckBox*>(pWidgetCtrl)->setChecked(*static_cast<const bool*>(sig->getValue()));
                }
            } else
            {
				//if (name.toLatin1() == "QLineEdit") static_cast<QLineEdit*>(pWidgetCtrl)->setText("----");
				//else if (name.toLatin1() == "QCheckBox") static_cast<QCheckBox*>(pWidgetCtrl)->setChecked(false);
            }
            pWidgetCtrl=0;
        }
        //m_ed->setActiveInterface(false);
    }

    connect(m_signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(ctrl_changed(QWidget*)));

    changed_clear();
}
//-------------------------------------------------------------------------------------------
void SignalsWidget::saveSignals()
{
    int size = m_list_changed.size();
    EDSignal *sig;
    int signal_id, id;
    QWidget *pWidgetCtrl;

    for(int i = 0; i < size; i++)
    {
        signal_id = m_list_changed.at(i);
        sig = &(m_ed->Signals()[signal_id]);
        for(int k = 0; k < m_signals_list.size(); k++) // find control id
        {
            if(m_signals_list.at(k)->getSignal_id() == signal_id)
            {
                id = k;
                break;
            }
        }
        pWidgetCtrl = m_list_ctrls.value(id);

        QString name = pWidgetCtrl->metaObject()->className();

        if(name.toLatin1() == "QLineEdit")
        {
            int base = (m_viewhex)?16:10;
			QString tmp_str = static_cast<QLineEdit*>(pWidgetCtrl)->text();
            bool ok = m_ed->Signals().setSignalValueStr(*sig, &tmp_str, base);
            if(!ok) logger().WriteLn("Error while convert-save value from string", Qt::red);

            // read
            if(ok) // when convert-save was successfull
            {
                QString tmp_value;
                ok &= m_ed->Signals().getSignalValueStr(*sig, tmp_value, m_viewhex);
				if (ok) static_cast<QLineEdit*>(pWidgetCtrl)->setText(tmp_value);
            }

            if(!ok) // highlight when error
            {
                m_list_ctrls.at(id)->setStyleSheet("background-color: red");
            } else // to normal color
            {
                m_list_ctrls.at(id)->setStyleSheet("background-color: white");
            }
        }
    }

    m_list_changed.clear();
}
//-------------------------------------------------------------------------------------------
void SignalsWidget::setCycleRead(bool value)
{
    if(m_signals_list.size() == 0) return;

    if(value)
    {
        m_timer->start(TIMER_PERIOD);
        logger().WriteLn("Start cycle read signals", Qt::black);
    }
    else
    {
        m_timer->stop();
        logger().WriteLn("Stop cycle read signals", Qt::black);
    }
}
//-------------------------------------------------------------------------------------------
void SignalsWidget::setViewHex(bool value)
{
    m_viewhex = value;;
}
//-------------------------------------------------------------------------------------------
void SignalsWidget::NewSignals()
{
    clear();
}
//-------------------------------------------------------------------------------------------
void SignalsWidget::clear()
{
    QWidget *pWidget;

    // labels list
    int size = m_list_labels.size();
    for(int i = 0; i < size; i++)
    {
        pWidget = m_list_labels.at(i);
        m_vlayout_labels->removeWidget(pWidget);
        if(pWidget != nullptr) delete pWidget;
    }
    m_list_labels.clear();

    // ctrls list
    size = m_list_ctrls.size();
    for(int i = 0; i < size; i++)
    {
        pWidget = m_list_ctrls.at(i);
        m_vlayout_ctrls->removeWidget(pWidget);
        m_signalMapper->removeMappings(pWidget);// remove mapping signal from this widget
        if(pWidget != nullptr) delete pWidget;
    }
    m_list_ctrls.clear();
}
//-------------------------------------------------------------------------------------------
void SignalsWidget::changed_add(int id, int signal_id)
{
    m_list_changed.append(signal_id);
    m_list_ctrls[id]->setStyleSheet("background-color: yellow");
}
//-------------------------------------------------------------------------------------------
void SignalsWidget::changed_clear()
{
   int size = m_list_ctrls.size();
   for(int i = 0; i < size; i++)
   {
       if(QString(m_list_ctrls.at(i)->metaObject()->className()).toLatin1() != "QCheckBox")
           m_list_ctrls.at(i)->setStyleSheet("background-color: white");
   }
   m_list_changed.clear();
}
//-------------------------------------------------------------------------------------------
bool SignalsWidget::changed_is_added(int signal_id)
{
    return qFind(m_list_changed, signal_id) != m_list_changed.end();
}
//-------------------------------------------------------------------------------------------
void SignalsWidget::timer_tick()
{
    readSignals();
}
//-------------------------------------------------------------------------------------------
void SignalsWidget::ctrl_changed(QWidget *sender)
{
    int id;
    int signal_id;

    int size = m_list_ctrls.size();

    // get control index in list
    bool found = false;
    for(int i = 0; i < size; i++)
    {
        if(m_list_ctrls.at(i) == sender)
        {
            id = i;
            signal_id = m_signals_list.at(i)->getSignal_id();
            found = true;
            break;
        }
    }
    if(!found) return;

    EDSignal &tmp_signal = m_ed->Signals()[signal_id];

    if(tmp_signal.getDescriptor().type == STYPE_BOOL) // do write/read for bool signals here
    {
        // set signal value
		bool tmp_bool = static_cast<QCheckBox*>(sender)->isChecked();
        bool ok = m_ed->Signals().setSignalValue(tmp_signal, &tmp_bool);

        // read signal value
        ok &= m_ed->Signals().getSignalValue(tmp_signal);
		static_cast<QCheckBox*>(sender)->setChecked(*static_cast<const bool*>(tmp_signal.getValue()));

    } else
    {
        if(!changed_is_added(signal_id)) changed_add(id, signal_id);
    }
}
//-------------------------------------------------------------------------------------------
void SignalsWidget::returnPressed()
{
    saveSignals();
}
