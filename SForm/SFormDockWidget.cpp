#include "SFormDockWidget.h"
#include <QApplication>
#include <QDesktopWidget>

//--------------------------------------------------------------------------------------------------
SFormDockWidget::SFormDockWidget(ED *ed, QString filename, QWidget *parent) :
    QDockWidget(parent),
    m_ed(ed),
    m_sform(ed, this),
    m_filename(filename)
{
    QRect rect = m_sform.ReadQML(filename);

    QFileInfo fileinfo(filename);
    setWindowTitle("QML form: " + fileinfo.fileName() + " (" + QString::number(m_sform.getNumItems()) + ")");

    rect.setWidth(rect.width() + 20);
    rect.setHeight(rect.height() + 40);

    m_opened_files.append(filename);

    setMinimumWidth(rect.width());
    setMinimumHeight(rect.height());
    move(QApplication::desktop()->screen()->rect().center() - this->rect().center());

    // toolbar
    m_window = new QMainWindow(nullptr);
    m_toolbar = new QToolBar(m_window);
    m_toolbar->setFloatable(false);
    m_toolbar->setMovable(false);

    m_window->addToolBar(m_toolbar);
    m_window->setCentralWidget(&m_sform);

    m_window->setParent(this);
    this->setWidget(m_window);

    act_active = new QAction(tr("Active"), this);
    act_active->setCheckable(true);
    connect(act_active, SIGNAL(triggered()), this, SLOT(active()));

    act_read = new QAction(tr("Read"), this);
    connect(act_read, SIGNAL(triggered()), this, SLOT(read()));

    act_write = new QAction(tr("Write"), this);
    connect(act_write, SIGNAL(triggered()), this, SLOT(write()));

    m_toolbar->addAction(act_active);
    m_toolbar->addAction(act_read);
    m_toolbar->addAction(act_write);
}
//--------------------------------------------------------------------------------------------------
QList<QString> SFormDockWidget::m_opened_files;

SFormDockWidget::~SFormDockWidget()
{
   m_opened_files.removeOne(m_filename);
}
//--------------------------------------------------------------------------------------------------
bool SFormDockWidget::isOpened(const QString &filename)
{
    return (m_opened_files.indexOf(filename) != -1);
}
//--------------------------------------------------------------------------------------------------
void SFormDockWidget::active()
{
    m_sform.setActive(act_active->isChecked());

    act_read->setEnabled(!act_active->isChecked());
}
//--------------------------------------------------------------------------------------------------
void SFormDockWidget::read()
{
    m_sform.force_signals_read(true);
}
//--------------------------------------------------------------------------------------------------
void SFormDockWidget::write()
{
    m_sform.force_signals_write(true);
}

