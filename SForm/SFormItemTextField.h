#ifndef SFORMITEMTEXTFIELD_H
#define SFORMITEMTEXTFIELD_H

#include "SFormItem.h"

class SFormItemTextField : public SFormItem
{
    Q_OBJECT

public:
    explicit SFormItemTextField(ED *ed, QuickItemControl itemcontrol, QQuickItem *quick_item, EDSignal *psignal, QObject *parent = 0);

    virtual void read();
    virtual void write();

public slots:
    virtual void clicked();
    virtual void changed();
};

#endif // SFORMITEMTEXTFIELD_H
