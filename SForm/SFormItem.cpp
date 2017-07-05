#include "SFormItem.h"

//---------------------------------------------------------------------------------------------------------
SFormItem::SFormItem(ED *ed, QuickItemControl itemcontrol, QQuickItem *quick_item, EDSignal *psignal, QObject *parent) :
    QObject(parent),
    m_quick_item(quick_item),
    m_psignal(psignal),
    m_readonly(false),
    m_itemcontrol(itemcontrol),
    m_ed(ed)
{

}
//---------------------------------------------------------------------------------------------------------
SFormItem::~SFormItem()
{

}

