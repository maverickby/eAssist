#include "SFormItemText.h"

//-----------------------------------------------------------------------------------------------------
SFormItemText::SFormItemText(ED *ed, QuickItemControl itemcontrol, QQuickItem *quick_item, EDSignal *psignal, QObject *parent) :
    SFormItem(ed, itemcontrol, quick_item, psignal, parent)
{
}
//-----------------------------------------------------------------------------------------------------
void SFormItemText::read()
{
    QString value;

    if(m_ed->Signals().getSignalValueStr(*m_psignal, value, false))
    {
        m_quick_item->setProperty( "text", value );
    }
}
//-----------------------------------------------------------------------------------------------------
// not used
void SFormItemText::write()
{

}
//-----------------------------------------------------------------------------------------------------
// not used
void SFormItemText::clicked()
{

}
//-----------------------------------------------------------------------------------------------------
// not used
void SFormItemText::changed()
{

}
