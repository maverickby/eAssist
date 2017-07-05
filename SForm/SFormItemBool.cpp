#include "SFormItemBool.h"
#include "../ED/Sig/EDSignal.h"

//-----------------------------------------------------------------------------------------------------
SFormItemBool::SFormItemBool(ED *ed, QuickItemControl itemcontrol, QQuickItem *quick_item, EDSignal *psignal, QObject *parent) :
    SFormItem(ed, itemcontrol, quick_item, psignal, parent),
    m_checkable(false)
{
    // estimate m_checkable
    if(itemcontrol == QuickItemControl::CHECKBOX) m_checkable = true;
    else if((itemcontrol == QuickItemControl::BUTTON) || (itemcontrol == QuickItemControl::TOOLBUTTON))
        m_checkable = m_quick_item->property("checkable").toBool();

    connect( m_quick_item, SIGNAL(clicked()), this, SLOT(clicked()) );
}
//-----------------------------------------------------------------------------------------------------
void SFormItemBool::read()
{
    if(m_checkable)
    {
        if(m_ed->Signals().getSignalValue(*m_psignal))
			m_quick_item->setProperty("checked", *(static_cast<bool*>(m_psignal->getValue())));
    }
}
//-----------------------------------------------------------------------------------------------------
void SFormItemBool::write()
{
    if(m_checkable)
    {
        bool checked = m_quick_item->property("checked").toBool();
        m_ed->Signals().setSignalValue( *m_psignal, &checked );
        read();
    }
}
//-----------------------------------------------------------------------------------------------------
void SFormItemBool::clicked()
{
    if(m_checkable)
    {
        bool checked = m_quick_item->property("checked").toBool();
        m_ed->Signals().setSignalValue( *m_psignal, &checked );
        read();
    } else
    {
        bool checked = true;
        m_ed->Signals().setSignalValue( *m_psignal, &checked );
        checked = false;
        m_ed->Signals().setSignalValue( *m_psignal, &checked );
    }
}
//-----------------------------------------------------------------------------------------------------
// not used
void SFormItemBool::changed()
{

}
