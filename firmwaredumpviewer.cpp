#include "firmwaredumpviewer.h"
#include "ui_firmwaredumpviewer.h"

//---------------------------------------------------------------------------------------------
FirmwareDumpViewer::FirmwareDumpViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FirmwareDumpViewer)
{
    ui->setupUi(this);

    hexedit_left = new QHexEdit(this);
    hexedit_left->setReadOnly(true);
    hexedit_left->setHighlightingColor(Qt::red);
    ui->horizontalLayout_hexedit->addWidget(hexedit_left);

    hexedit_right = new QHexEdit(this);
    hexedit_right->setReadOnly(true);
    hexedit_right->setHighlightingColor(Qt::red);
    ui->horizontalLayout_hexedit->addWidget(hexedit_right);

    m_vscrollbar_right = hexedit_right->verticalScrollBar();
    m_vscrollbar_left = hexedit_left->verticalScrollBar();
    connect(m_vscrollbar_right, SIGNAL(sliderMoved(int)), m_vscrollbar_left, SLOT(setValue(int)));
    connect(m_vscrollbar_left, SIGNAL(sliderMoved(int)), m_vscrollbar_right, SLOT(setValue(int)));

    m_hscrollbar_right = hexedit_right->horizontalScrollBar();
    m_hscrollbar_left = hexedit_left->horizontalScrollBar();
    connect(m_hscrollbar_right, SIGNAL(sliderMoved(int)), m_hscrollbar_left, SLOT(setValue(int)));
    connect(m_hscrollbar_left, SIGNAL(sliderMoved(int)), m_hscrollbar_right, SLOT(setValue(int)));
}
//---------------------------------------------------------------------------------------------
FirmwareDumpViewer::~FirmwareDumpViewer()
{
    delete ui;

    delete hexedit_left;
    delete hexedit_right;
}
//---------------------------------------------------------------------------------------------
void FirmwareDumpViewer::setFirmware(const QByteArray &value)
{
    m_firmware = value;
}
//---------------------------------------------------------------------------------------------
void FirmwareDumpViewer::setDump(const QByteArray &value)
{
    m_dump = value;
}
//---------------------------------------------------------------------------------------------
void FirmwareDumpViewer::on_pushButton_ok_clicked()
{
    this->close();
}
//---------------------------------------------------------------------------------------------
void FirmwareDumpViewer::on_pushButton_compare_clicked()
{
    QList<uint> bad_data;

    if(m_dump.size() < m_firmware.size())
    {
        QMessageBox::critical(this, "Error", "Error: firmware sise is larger than dump.");
        return;
    }

    uint size = m_firmware.size();
    uint num_errors = 0;

    for(uint i  = 0; i < size; i++)
    {
        if(m_firmware.at(i) != m_dump.at(i))
        {
            num_errors++;
            bad_data.append(i);
        }
    }

    if(num_errors > 0)
    {
        QMessageBox::critical(this, "Error", "Found " + QString::number(num_errors, 10) + " error");
        hexedit_left->setDataHighlighted(bad_data, true);
        hexedit_right->setDataHighlighted(bad_data, true);

    }
}
//---------------------------------------------------------------------------------------------
void FirmwareDumpViewer::show()
{
    QDialog::show();

    if(m_firmware.size() > 0) hexedit_left->setData(m_firmware);
    if(m_dump.size() > 0) hexedit_right->setData(m_dump);
}
