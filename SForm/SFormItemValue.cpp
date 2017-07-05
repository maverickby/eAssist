#include <stdint.h>
#include "SFormItemValue.h"
#include "../ED/ED.h"
#include "../ED/EDLogger.h"

//-----------------------------------------------------------------------------------------------------
SFormItemValue::SFormItemValue(ED *ed, QuickItemControl itemcontrol, QQuickItem *quick_item, EDSignal *psignal, QObject *parent) :
    SFormItem(ed, itemcontrol, quick_item, psignal, parent)
{
    m_readonly = (itemcontrol == QuickItemControl::PROGRESSBAR);

    if(!m_readonly)
        connect (m_quick_item, SIGNAL(changed()), this, SLOT(changed()) );
}
//-----------------------------------------------------------------------------------------------------
void SFormItemValue::read()
{
    double v;

    bool pause_tmp = logger().getPauseLogging();
    logger().setPauseLogging(true);

        if(m_ed->Signals().getSignalValue(*m_psignal))
        {
           switch(m_psignal->getDescriptor().type)
            {
            case STYPE_BYTE:
                v = *static_cast<uint8_t*>(m_psignal->getValue());
                break;

            case STYPE_UINT:
                v = *static_cast<const uint16_t*>(m_psignal->getValue());
                break;

            case STYPE_INT:
                v = *static_cast<const int16_t*>(m_psignal->getValue());
                break;

            case STYPE_LONG:
                v = *static_cast<const int32_t*>(m_psignal->getValue());
                break;

            case STYPE_ULONG:
                v = *static_cast<const uint32_t*>(m_psignal->getValue());
                break;

            case STYPE_FLOAT:
                v = *static_cast<const float*>(m_psignal->getValue());
                break;

            default:
                v = 0;
                break;
            }

           m_quick_item->setProperty( "value", v );
        }

    logger().setPauseLogging(pause_tmp);
}
//-----------------------------------------------------------------------------------------------------
void SFormItemValue::write()
{
    if(m_readonly) return;

    uint8_t     tmp_uint8;
    uint16_t    tmp_uint16;
    int16_t     tmp_int16;
    int32_t     tmp_int32;
    uint32_t    tmp_uint32;
    float       tmp_float;

    void    *pvalue = nullptr;

    double v = m_quick_item->property("value").toDouble();

    switch(m_psignal->getDescriptor().type)
     {
     case STYPE_BYTE:
             tmp_uint8 = v;
             pvalue = &tmp_uint8;
         break;

     case STYPE_UINT:
            tmp_uint16 = v;
            pvalue = &tmp_uint16;
         break;

     case STYPE_INT:
            tmp_int16 = v;
            pvalue = &tmp_int16;
         break;

     case STYPE_LONG:
            tmp_int32 = v;
            pvalue = &tmp_int32;
         break;

     case STYPE_ULONG:
             tmp_uint32 = v;
             pvalue = &tmp_uint32;
         break;

     case STYPE_FLOAT:
            tmp_float = v;
            pvalue = &tmp_float;
         break;

     default:
             tmp_uint32 = v;
             pvalue = &tmp_uint32;
         break;
     }

    bool pause_tmp = logger().getPauseLogging();
    logger().setPauseLogging(true);
        m_ed->Signals().setSignalValue(*m_psignal, pvalue);
    logger().setPauseLogging(pause_tmp);

    if(m_itemcontrol != QuickItemControl::SLIDER) read();
}
//-----------------------------------------------------------------------------------------------------
// NOT USED
void SFormItemValue::clicked()
{
}
//-----------------------------------------------------------------------------------------------------
void SFormItemValue::changed()
{
    disconnect (m_quick_item, SIGNAL(changed()), this, SLOT(changed()) );
        write();
    connect (m_quick_item, SIGNAL(changed()), this, SLOT(changed()) );
}
