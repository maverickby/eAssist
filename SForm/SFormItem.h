#ifndef SFORMITEM_H
#define SFORMITEM_H

#include <QObject>
#include <QQuickItem>
#include "../ED/ED.h"

enum QuickItemControl {BUTTON, CHECKBOX, LABEL, PROGRESSBAR, SLIDER, SPINBOX, TOOLBUTTON, TEXTFIELD};

class SFormItem : public QObject
{
    Q_OBJECT
public:
    explicit SFormItem(ED *ed, QuickItemControl itemcontrol, QQuickItem *quick_item, EDSignal *psignal, QObject *parent = 0);
    virtual ~SFormItem();

    virtual void read() = 0;
    virtual void write() = 0;

    QQuickItem       *m_quick_item;
    EDSignal         *m_psignal;
    bool              m_readonly;
    QuickItemControl  m_itemcontrol;
    ED               *m_ed;

signals:

public slots:
    virtual void clicked() = 0;
    virtual void changed() = 0;

};

#endif // SFORMITEM_H
