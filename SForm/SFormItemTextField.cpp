#include <stdint.h>
#include "SFormItemTextField.h"
#include "../ED/EDLogger.h"

//----------------------------------------------------------------------------------------------------------------
SFormItemTextField::SFormItemTextField(ED *ed, QuickItemControl itemcontrol, QQuickItem *quick_item, EDSignal *psignal, QObject *parent):
    SFormItem(ed, itemcontrol, quick_item, psignal, parent)
{
    connect (m_quick_item, SIGNAL(changed()), this, SLOT(changed()) );
}
//----------------------------------------------------------------------------------------------------------------
void SFormItemTextField::read()
{
    QVariant v;

    if( m_quick_item->property("editing").toBool() == true) return; // when user edit value in TextField we do not have to touch value

    bool pause_tmp = logger().getPauseLogging();
    logger().setPauseLogging(true);

        if(m_ed->Signals().getSignalValue(*m_psignal))
        {
           switch(m_psignal->getDescriptor().type)
            {
            case STYPE_STRING:
                v = QString(static_cast<char*>(m_psignal->getValue()));
                break;

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

           m_quick_item->setProperty( "text", v );
           m_quick_item->setProperty( "editing", false );
        }

    logger().setPauseLogging(pause_tmp);
}
//----------------------------------------------------------------------------------------------------------------
void SFormItemTextField::write()
{
    m_quick_item->setProperty( "editing", false ); // set normal color whrn finish editing

    QString str_tmp = m_quick_item->property("text").toString();

    if(m_psignal->getDescriptor().type == STYPE_STRING)  // TEXT
    {
        m_ed->Signals().setSignalValueStr(*m_psignal, &str_tmp);
    } else // VALUE
    {
        uint8_t     tmp_uint8;
        uint16_t    tmp_uint16;
        int16_t     tmp_int16;
        int32_t     tmp_int32;
        uint32_t    tmp_uint32;
        float       tmp_float;
        void        *pvalue = nullptr;

        bool ok;
        double v = str_tmp.toDouble(&ok);

        if(ok)
        {
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

            read(); // get current real value of signal

        } else
        {
            logger().WriteLn("Error while convert qml item value to signal " + m_psignal->getDescriptor().name, Qt::red);
        }
    }
}
//----------------------------------------------------------------------------------------------------------------
// NOT USED
void SFormItemTextField::clicked()
{

}
//----------------------------------------------------------------------------------------------------------------
void SFormItemTextField::changed()
{
    disconnect (m_quick_item, SIGNAL(changed()), this, SLOT(changed()) );
        write();
    connect (m_quick_item, SIGNAL(changed()), this, SLOT(changed()) );
}
