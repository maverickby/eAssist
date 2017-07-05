#ifndef SFORMITEMBOOL_H
#define SFORMITEMBOOL_H

#include "SFormItem.h"
#include "../ED/ED.h"

class SFormItemBool : public SFormItem
{
    Q_OBJECT
public:
    explicit SFormItemBool(ED *ed, QuickItemControl itemcontrol, QQuickItem *quick_item, EDSignal *psignal, QObject *parent = 0);

    virtual void read();
    virtual void write();

signals:

public slots:
    virtual void clicked();
    virtual void changed();

private:
    bool m_checkable;

};

#endif // SFORMITEMBOOL_H
