#include <QLayout>
#include "SFormWidget.h"
#include "SFormItemBool.h"
#include "SFormItemValue.h"
#include "SFormItemText.h"
#include "SFormItemTextField.h"
#include "../ED/EDLogger.h"

#define ACTIVE_TIMER_INTERVAL 50

// Currently supporting controls:
//  Buuton      ->   Button_QMLTYPE
//  CheckBox    ->   CheckBox_QMLTYPE
//  Label       ->   QQuickText
//  ProgressBar ->   ProgressBar_QMLTYPE
//  Slider      ->   Slider_QMLTYPE
//  SpinBox     ->   SpinBox_QMLTYPE
//  ToolButton  ->   BasicButton_QMLTYPE
//  TextField   ->   TextField_QMLTYPE

#define QML_BUTTON      "Button_QMLTYPE"
#define QML_CHECKBOX    "CheckBox_QMLTYPE"
#define QML_LABEL       "QQuickText"
#define QML_PROGRESSBAR "ProgressBar_QMLTYPE"
#define QML_SLIDER      "Slider_QMLTYPE"
#define QML_SPINBOX     "SpinBox_QMLTYPE"
#define QML_TOOLBUTTON  "BasicButton_QMLTYPE"
#define QML_TEXTFIELD   "TextField_QMLTYPE"

//----------------------------------------------------------------------------------------------------
SFormWidget::SFormWidget(ED *ed, QWidget *parent) :
    QWidget(parent),
    m_ed(ed),
    m_active(false),
    m_cycle_read(false),
    m_view(nullptr),
    m_container(nullptr),
    m_vlayout(nullptr)
{
    m_timer.setTimerType(Qt::PreciseTimer);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timer_tick()));
}
//----------------------------------------------------------------------------------------------------
SFormWidget::~SFormWidget()
{
    clear();
}
//----------------------------------------------------------------------------------------------------
void SFormWidget::setActive(bool value)
{
    m_active = value;

    if(m_active) m_timer.start(ACTIVE_TIMER_INTERVAL);
    else m_timer.stop();
}
//----------------------------------------------------------------------------------------------------
bool SFormWidget::isActive() const
{
    return m_active;
}
//----------------------------------------------------------------------------------------------------
void SFormWidget::setCycleRead(bool value)
{
    m_cycle_read = value;
}
//----------------------------------------------------------------------------------------------------
bool SFormWidget::isCycleRead() const
{
    return m_cycle_read;
}
//----------------------------------------------------------------------------------------------------
QRect SFormWidget::ReadQML(const QString &filename)
{
    m_timer.stop();

    clear();

    // create controls
    m_view = new QQuickView();
    m_container = QWidget::createWindowContainer(m_view, this);
    m_vlayout = new QVBoxLayout(this);
    m_vlayout->addWidget(m_container);

    // open qml
    m_view->setSource(QUrl::fromLocalFile(filename));

    // validate controls for corespondence to signals
    validate_signals_in_qml();

    // populate items list
    populate_items();

    return m_view->frameGeometry();
}
//----------------------------------------------------------------------------------------------------
unsigned int SFormWidget::getNumItems() const
{
    return m_items.size();
}
//----------------------------------------------------------------------------------------------------
void SFormWidget::clear()
{
    setActive(false);// stop avtivity over ED

    // delete and clear items list
    foreach (SFormItem *item, m_items) {
        delete item;
    }
    m_items.clear();

    delete m_view;
    delete m_vlayout;
}
//----------------------------------------------------------------------------------------------------
void SFormWidget::force_signals_read(bool show_log)
{
    bool pause_tmp = logger().getPauseLogging();

    if(!show_log) logger().setPauseLogging(true);
        foreach (SFormItem *item, m_items)
        {
            item->read();
        }
    logger().setPauseLogging(pause_tmp);
}
//----------------------------------------------------------------------------------------------------
void SFormWidget::force_signals_write(bool show_log)
{
    if(!show_log) logger().setPauseLogging(true);
        foreach (SFormItem *item, m_items)
        {
            item->write();
        }
    logger().setPauseLogging(false);
}
//----------------------------------------------------------------------------------------------------
void SFormWidget::timer_tick()
{
    force_signals_read();
}
//----------------------------------------------------------------------------------------------------
QuickItemControl SFormWidget::get_itemcontrol(const QQuickItem *item)
{
    QString item_type = item->metaObject()->className();

    if(item_type.contains(QML_BUTTON,       Qt::CaseInsensitive))  return QuickItemControl::BUTTON;
    else if(item_type.contains(QML_CHECKBOX,     Qt::CaseInsensitive))  return QuickItemControl::CHECKBOX;
    else if(item_type.contains(QML_TOOLBUTTON,   Qt::CaseInsensitive))  return QuickItemControl::TOOLBUTTON;
    else if(item_type.contains(QML_LABEL,        Qt::CaseInsensitive))  return QuickItemControl::LABEL;
    else if(item_type.contains(QML_PROGRESSBAR,  Qt::CaseInsensitive))  return QuickItemControl::PROGRESSBAR;
    else if(item_type.contains(QML_SLIDER,       Qt::CaseInsensitive))  return QuickItemControl::SLIDER;
    else if(item_type.contains(QML_SPINBOX,      Qt::CaseInsensitive))  return QuickItemControl::SPINBOX;
    else if(item_type.contains(QML_TEXTFIELD,    Qt::CaseInsensitive))  return QuickItemControl::TEXTFIELD;

    return QuickItemControl::LABEL;
}
//----------------------------------------------------------------------------------------------------
void SFormWidget::validate_signals_in_qml()
{
    logger().WriteLn("Validating QML file for correspondnce to signals:", Qt::blue);

	QList<QQuickItem*> items = m_view->rootObject()->childItems();

    foreach (QQuickItem *item, items)
    {
        QVariant tmp = item->property("objectName");
        if(tmp.isValid())
        {
            QString signal_name = tmp.toString();
            if(!signal_name.isEmpty())
            {

                logger().Write("    " + signal_name + ":", Qt::black);
                if(m_ed->Signals().FullnameToSignal(signal_name) != nullptr)
                {
                    logger().WriteLn("OK", Qt::green);
                } else
                {
                    logger().WriteLn("error", Qt::red);
                    item->setProperty("enabled", "false");
                }
            }
        }
    }
}
//----------------------------------------------------------------------------------------------------
void SFormWidget::populate_items()
{
	QList<QQuickItem*> items = m_view->rootObject()->childItems();

    foreach (QQuickItem *item, items)
    {
        QVariant tmp = item->property("objectName");
        if(tmp.isValid())
        {
            QString signal_name = tmp.toString();
            if(!signal_name.isEmpty())
            {
                EDSignal *signal_tmp = m_ed->Signals().FullnameToSignal(signal_name);
                if(signal_tmp != nullptr) // valid item is here
                {
					const EDSignalDescriptor &descr = signal_tmp->getDescriptor();
					const QuickItemControl itemcontrol = get_itemcontrol(item);
                    QString item_type = item->metaObject()->className();
                    if((itemcontrol == QuickItemControl::BUTTON)    ||
                       (itemcontrol == QuickItemControl::CHECKBOX)  ||
                       (itemcontrol == QuickItemControl::TOOLBUTTON))           // *** BOOL
                    {
                        if(descr.type == STYPE_BOOL)
                            m_items.append(new SFormItemBool(m_ed, itemcontrol, item, signal_tmp));
                        else
                            logger().WriteLn("Can not assign control" + item_type + " for non bool signal " + descr.name, Qt::red);
                    } else

                    if(itemcontrol == QuickItemControl::LABEL)                  // *** BOOL, VALUE, TEXT
                    {
                        m_items.append(new SFormItemText(m_ed, itemcontrol, item, signal_tmp));
                    } else

                    if((itemcontrol == QuickItemControl::PROGRESSBAR) ||
                       (itemcontrol == QuickItemControl::SLIDER)      ||
                       (itemcontrol == QuickItemControl::SPINBOX))              // *** VALUE
                    {
                        if( (descr.type == STYPE_BYTE) || (descr.type == STYPE_UINT) || (descr.type == STYPE_INT) ||
                            (descr.type == STYPE_LONG) || (descr.type == STYPE_ULONG)|| (descr.type == STYPE_FLOAT))
                            m_items.append(new SFormItemValue(m_ed, itemcontrol, item, signal_tmp));
                        else
                            logger().WriteLn("Can not assign control" + item_type + " for non integer and non real signal " + descr.name, Qt::red);
                    } else

                    if(itemcontrol == QuickItemControl::TEXTFIELD)              // *** TEXT, VALUE
                    {
                        if((descr.type == STYPE_BYTE) || (descr.type == STYPE_UINT) || (descr.type == STYPE_INT)   ||
                           (descr.type == STYPE_LONG) || (descr.type == STYPE_ULONG)|| (descr.type == STYPE_FLOAT) ||
                           (descr.type == STYPE_STRING))
                            m_items.append(new SFormItemTextField(m_ed, itemcontrol, item, signal_tmp));
                        else
                            logger().WriteLn("Can not assign control" + item_type + " for non integer, non real and string signal " + descr.name, Qt::red);
                    }
                }
            }
        }
    }
}
