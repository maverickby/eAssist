#ifndef SFORMITEMVALUE_H
#define SFORMITEMVALUE_H

#include "SFormItem.h"

class SFormItemValue : public SFormItem
{
    Q_OBJECT
public:
    explicit SFormItemValue(ED *ed, QuickItemControl itemcontrol, QQuickItem *quick_item, EDSignal *psignal, QObject *parent = 0);

    virtual void read();
    virtual void write();

signals:

public slots:
    virtual void clicked();
    virtual void changed();

};

#endif // SFORMITEMVALUE_H
