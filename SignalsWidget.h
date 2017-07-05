#ifndef SIGNALSCONTAINER_H
#define SIGNALSCONTAINER_H

#include <QWidget>
#include <QList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QTimer>
#include <QSignalMapper>
#include <QMutex>
#include "ED/ed.h"

class SignalsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SignalsWidget(ED *ed, QWidget *parent = nullptr);
    ~SignalsWidget();
    
public slots:
    void setSignals(const QVector<EDSignal*> &signals_list);

    void readSignals();
    void saveSignals();
    void setCycleRead(bool value);
    void setViewHex(bool value);

private slots:
    void NewSignals();
    void processSignalValueChanged(EDSignal* signal);

    void timer_tick();
    void ctrl_changed(QWidget *sender);
    void returnPressed();

private:
    void clear();
    void changed_add(int id, int signal_id);
    void changed_clear();
    bool changed_is_added(int signal_id);

    ED *m_ed;

    QVector<EDSignal*> m_signals_list;
    QList<QWidget*> m_list_labels;
    QList<QWidget*> m_list_ctrls;

    QHBoxLayout *m_hlayout;
    QVBoxLayout *m_vlayout_labels;
    QVBoxLayout *m_vlayout_ctrls;
    QSplitter   *m_splitter;
    QWidget     *m_widget_labels;
    QWidget     *m_widget_ctrls;

    QTimer      *m_timer;

    QSignalMapper *m_signalMapper;

    bool m_viewhex;

    QList<int> m_list_changed;

};

#endif // SIGNALSCONTAINER_H
