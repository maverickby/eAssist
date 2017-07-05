#ifndef SFORMITEMTEXT_H
#define SFORMITEMTEXT_H

#include "SFormItem.h"

class SFormItemText : public SFormItem
{
    Q_OBJECT
public:
    explicit SFormItemText(ED *ed, QuickItemControl itemcontrol, QQuickItem *quick_item, EDSignal *psignal, QObject *parent = 0);

    virtual void read();
    virtual void write();

signals:

public slots:
    virtual void clicked();
    virtual void changed();

};

#endif // SFORMITEMTEXT_H
