#include <cstdint>
#include "DockLogger.h"
#include "Settings.h"

//---------------------------------------------------------------------------------------------------------
DockLogger::DockLogger(QWidget *parent):
    EDILogger(this),
    QDockWidget("Log", parent),
    m_num_last_chars(0)
{
    text_browser = new QTextBrowser(this);

    this->setWidget(text_browser);

    text_browser->setReadOnly(true);
    text_browser->setWordWrapMode(QTextOption::WrapAnywhere);
    text_browser->setFont(QFont("Courier new", 10));
    text_browser->document()->setMaximumBlockCount(MAX_NUM_RECORDS);

    m_pause = false;
}
//---------------------------------------------------------------------------------------------------------
DockLogger::~DockLogger()
{
}
//-----------------------------------------------------------------------------------------------------
void DockLogger::Write(const QString &value, Qt::GlobalColor color)
{
    if(m_pause)  return;

    text_browser->setTextColor(QColor(color));
    text_browser->insertPlainText(value);
}
//-----------------------------------------------------------------------------------------------------
void DockLogger::WriteLn(const QString &value, Qt::GlobalColor color)
{
    if(m_pause)  return;

    text_browser->moveCursor(QTextCursor::End); // move to the end of document
    text_browser->setTextColor(QColor(color));
    text_browser->insertPlainText(value);
    text_browser->append("");
}
//-----------------------------------------------------------------------------------------------------
void DockLogger::WriteLnMessage(const QString &value, Qt::GlobalColor color)
{
    if(m_pause)  return;

    WriteLn(value, color);
    QMessageBox::information(text_browser, "Message", value, QMessageBox::Ok);
}
//-----------------------------------------------------------------------------------------------------
void DockLogger::WriteLnMessageCritical(const QString &value, Qt::GlobalColor color)
{
    if(m_pause)  return;

    WriteLn(value, color);
    QMessageBox::critical(text_browser, "Message", value);
}
//-----------------------------------------------------------------------------------------------------
void DockLogger::WriteCommandResult(EDCommand::EDCommandResult result)
{
    if(m_pause)  return;

    QString tmp;
    Qt::GlobalColor col;
    int size = result.responce.size();

    switch(result.state)
    {
        case EDCommand::CR_Success:        tmp = "OK";              col = Qt::green; break;
        case EDCommand::CR_NotSuccess:     tmp = "error responce";  col = Qt::red;   break;
        case EDCommand::CR_BadAck:         tmp = "bad acknowledge"; col = Qt::red;   break;
        case EDCommand::CR_TimeOut:        tmp = "timeout";         col = Qt::red;   break;
        case EDCommand::CR_BadData:        tmp = "bad data";        col = Qt::red;   break;
        case EDCommand::CR_Overflow:       tmp = "overflow";        col = Qt::red;   break;
        case EDCommand::CR_BadCheckSum:    tmp = "bad checksum";    col = Qt::red;   break;
        case EDCommand::CR_Waiting:        tmp = "waiting";         col = Qt::yellow;break;
        case EDCommand::CR_Unknown:        tmp = "unknown";         col = Qt::yellow;break;
        case EDCommand::CR_BadWrapper:     tmp = "bad wrapper";     col = Qt::red;   break;
    default:                    tmp = "...";            col = Qt::yellow;break;
    }

    if( (size > 0) && settings().isShowResponce())
    {
        Write(tmp, col);
        QString str_value = " <";
        QString hex_value = result.responce.toHex();
        for(int i = 0; i < size - 1; i++) hex_value.insert(i*2 + 2 + i, " ");
        str_value.append(hex_value);
        str_value.append(">");
        WriteLn(str_value, Qt::gray);
    } else
    {
        WriteLn(tmp, col);
    }
}
//-----------------------------------------------------------------------------------------------------
void DockLogger::setPauseLogging(bool state)
{
    m_pause = state;
}
//-----------------------------------------------------------------------------------------------------
bool DockLogger::getPauseLogging()
{
    return m_pause;
}
//-----------------------------------------------------------------------------------------------------
void DockLogger::WritePercent(int value, Qt::GlobalColor color)
{
    QString tmp = QString::number(value, 10) + "%";
    m_num_last_chars = tmp.size();
    Write(tmp, color);
}
//-----------------------------------------------------------------------------------------------------
void DockLogger::UpdatePercent(int value, Qt::GlobalColor color)
{
    for(int i = 0; i < m_num_last_chars; i++) text_browser->textCursor().deletePreviousChar();

    WritePercent(value, color);
}
//-----------------------------------------------------------------------------------------------------
void DockLogger::WriteSignalInfo(const EDSignal &value)
{
    QString tmp;

	const EDSignalDescriptor &descr_tmp = value.getDescriptor();

	WriteLn(descr_tmp.name, Qt::darkGreen);

	switch (descr_tmp.type_class)
    {
    case SCL_STD:       tmp = "STD";      break;
    case SCL_ENUM:      tmp = "ENUM";     break;
    case SCL_ARRAY1:    tmp = "ARRAY1";   break;
    case SCL_ARRAY2:    tmp = "ARRAY2";   break;
    }
    WriteLn("    Type class: " + tmp, Qt::darkGreen);

	switch (descr_tmp.type)
    {
    case STYPE_STRING:  tmp = "STRING";    break;
    case STYPE_BOOL:    tmp = "BOOL";      break;
    case STYPE_BYTE:    tmp = "BYTE";      break;
    case STYPE_UINT:    tmp = "UINT";      break;
    case STYPE_INT:     tmp = "INT";       break;
    case STYPE_LONG:    tmp = "LONG";      break;
    case STYPE_ULONG:   tmp = "ULONG";     break;
    case STYPE_FLOAT:   tmp = "FLOAT";     break;
    }
    WriteLn("    Type: " + tmp, Qt::darkGreen);

	if (descr_tmp.type_class == SCL_STD)
    {
		switch (descr_tmp.type)
        {
        case STYPE_STRING:
            tmp = QString(static_cast<char*>(value.getValue()));
            break;

        case STYPE_BOOL:
            if(*(static_cast<const bool*>(value.getValue()))) tmp = "true";
            else tmp = "false";
            break;

        case STYPE_BYTE:
            tmp = QString::number(*static_cast<const uint8_t*>(value.getValue()), 10);
            break;

        case STYPE_UINT:
            tmp = QString::number(*static_cast<const uint16_t*>(value.getValue()), 10);
            break;

        case STYPE_INT:
            tmp = QString::number(*static_cast<const int16_t*>(value.getValue()), 10);
            break;

        case STYPE_LONG:
            tmp = QString::number(*static_cast<const int32_t*>(value.getValue()), 10);
            break;

        case STYPE_ULONG:
            tmp = QString::number(*static_cast<const uint32_t*>(value.getValue()), 10);
            break;

        case STYPE_FLOAT:
            tmp = QString::number(*static_cast<const float*>(value.getValue()));
            break;

        default:
            tmp.clear();
            break;
        }
    }else
    {
        tmp = "error";
    }
    WriteLn("    Value: " + tmp, Qt::darkGreen);

    if(value.getCtrl_mode() == CM_FIXED) tmp = "yes";
    else tmp = "no";
    WriteLn("    Control: " + tmp, Qt::darkGreen);

    // telemetry control checkbox
    if(value.getIntelemetry()) tmp = "yes";
    else tmp = "no";
    WriteLn("    Telemetry: " + tmp, Qt::darkGreen);

	WriteLn("    Period: " + QString::number(descr_tmp.period, 10), Qt::darkGreen);

    QStringList list_measure_type;
    list_measure_type << "NONE" << "VOLTAGE" << "AMPERE" << "ANG_DEGREE" <<
            "RADIAN" << "SMT_METER" << "VOLT_ANG_DEGREE" << "METER_SECOND" << "CELSIUS" << "ANG_DEGREE_SECOND";
	WriteLn("    Measure: " + list_measure_type.at(static_cast<int>(descr_tmp.measure_type)), Qt::darkGreen);

	if (descr_tmp.en_read) tmp = "yes";
    else tmp = "no";
    WriteLn("    Support read: " + tmp, Qt::darkGreen);

	if (descr_tmp.en_setting) tmp = "yes";
    else tmp = "no";
    WriteLn("    Support settings: " + tmp, Qt::darkGreen);

	if (descr_tmp.en_ctrl) tmp = "yes";
    else tmp = "no";
    WriteLn("    Support ctrl: " + tmp, Qt::darkGreen);

	if (descr_tmp.en_telemetry) tmp = "yes";
    else tmp = "no";
    WriteLn("    Support telemetry: " + tmp, Qt::darkGreen);

	if (descr_tmp.en_signature) tmp = "yes";
    else tmp = "no";
    WriteLn("    Support signature: " + tmp, Qt::darkGreen);

	WriteLn("    Coefficient: " + QString::number(descr_tmp.coef, 'g', 10), Qt::darkGreen);
}
//-----------------------------------------------------------------------------------------------------
void DockLogger::Clear()
{
    text_browser->clear();
}
