#ifndef SIGNALSDOCKWIDGET_H
#define SIGNALSDOCKWIDGET_H

#include <QMainWindow>
#include <QToolBar>
#include <QDockWidget>
#include <QAction>
#include <QScrollArea>
#include <QCloseEvent>
#include "ED/ed.h"
#include "SignalsWidget.h"

class SignalsDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit SignalsDockWidget(ED *ed, QWidget *parent = nullptr);
	inline SignalsWidget   *getSygnalsWidget() {return m_container;}
	inline QAction *getAct_cycleread() {return act_cycleread;};
    
public slots:
    void setSignals(QVector<EDSignal*> &signals_list);

private slots:
    void read();
    void save();
    void cycleread();
    void hex();

private:
    QMainWindow     *m_window;
    QToolBar        *m_toolbar;
    QScrollArea     *m_scrollarea;
    SignalsWidget   *m_container;

    QAction *act_read;
    QAction *act_save;
    QAction *act_cycleread;
    QAction *act_hex;
    
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // SIGNALSDOCKWIDGET_H
