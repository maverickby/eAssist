#ifndef SFORMWIDGET_H
#define SFORMWIDGET_H

#include <QWidget>
#include <QQuickView>
#include <QVBoxLayout>
#include <QTimer>
#include "../ED/ed.h"
#include "SFormItem.h"

class SFormWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SFormWidget(ED *ed, QWidget *parent = 0);
    ~SFormWidget();

    void setActive(bool value);
    bool isActive() const;

    void setCycleRead(bool value);
    bool isCycleRead() const;

    QRect ReadQML(const QString &filename);

    unsigned int getNumItems() const;

public slots:
    void clear();

    void force_signals_read(bool show_log = false);
    void force_signals_write(bool show_log = false);

private slots:
    void timer_tick();

private:
    QuickItemControl get_itemcontrol(const QQuickItem *item);

    QTimer m_timer;

    ED *m_ed;

    bool m_active;
    bool m_cycle_read;

    QVector<SFormItem*> m_items;

    QQuickView  *m_view;
    QWidget     *m_container;
    QVBoxLayout *m_vlayout;

    void validate_signals_in_qml();
    void populate_items();
};

#endif // SFORMWIDGET_H
