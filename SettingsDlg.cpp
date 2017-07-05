#include "SettingsDlg.h"
#include "ui_settings.h"

//---------------------------------------------------------------------------------------------------
SettingsDlg::SettingsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDlg)
{
    ui->setupUi(this);

    setFixedSize(size());
    setModal(true);

    connect(ui->groupBoxComPort, SIGNAL(clicked(bool)), this, SLOT(groupboxComPort_clicked(bool)));
    connect(ui->groupBoxUDP,     SIGNAL(clicked(bool)), this, SLOT(groupboxUDP_clicked(bool)));
}
//---------------------------------------------------------------------------------------------------
SettingsDlg::~SettingsDlg()
{
    delete ui;
}
//---------------------------------------------------------------------------------------------------
bool SettingsDlg::Execute()
{
    bool res = false;
    unsigned int tmp_baudrate[] = {9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};

    // ******* settings -> dialog controls
	Settings &set = settings();

    ui->checkBox_ShowResponce->setChecked(set.isShowResponce());

	ui->groupBoxComPort->setChecked(set.m_interface_type == IT_COMPORT);
	ui->groupBoxUDP->setChecked(set.m_interface_type == IT_UDP);

    for(unsigned int i = 0; i < sizeof(tmp_baudrate)/sizeof(tmp_baudrate[0]); i++)
    {
		if (set.m_comport_settings.baudrate == tmp_baudrate[i])
        {
            ui->comboBox_ComPort_BaudRate->setCurrentIndex(i);
            break;
        }
    }
	ui->spinBox_ComPortNumber->setValue(set.m_comport_settings.port);
    switch(set.m_comport_settings.parity)
    {
        case PAR_NONE: ui->comboBox_ComPort_Parity->setCurrentIndex(0); break;
        case PAR_EVEN: ui->comboBox_ComPort_Parity->setCurrentIndex(1); break;
        case PAR_ODD:  ui->comboBox_ComPort_Parity->setCurrentIndex(2); break;
        case PAR_MARK:
        case PAR_SPACE:  break;
    }

	ui->lineEdit_UDP_Address->setText(set.m_udp_settings.ip_address);
	ui->spinBox_UDP_Port->setValue(set.m_udp_settings.port);

    ui->groupBoxWrapper->setChecked(set.m_wrapper_settings.enable);
    ui->spinBox_local_addr->setValue(set.m_wrapper_settings.local_address);
    ui->spinBox_remote_addr->setValue(set.m_wrapper_settings.remote_address);

    ui->lineEdit_Code1->setText(set.getCode1());
    ui->lineEdit_Code2->setText(set.getCode2());

    ui->spinBox_timeout_descriptor->setValue(set.getTimeoutDescriptor());
    ui->spinBox_period_telemetry->setValue(set.getPeriodTelemetry());

    // open dialog with settings
    if(this->exec() == QDialog::Accepted)
    {
        // ******* dialog controls -> settings
		set.m_showresponce = ui->checkBox_ShowResponce->isChecked();

		set.m_interface_type = ui->groupBoxComPort->isChecked() ? IT_COMPORT : IT_UDP;

		set.m_comport_settings.baudrate = tmp_baudrate[ui->comboBox_ComPort_BaudRate->currentIndex()];
		set.m_comport_settings.port = ui->spinBox_ComPortNumber->value();
        switch(ui->comboBox_ComPort_Parity->currentIndex())
        {
            case 0: set.m_comport_settings.parity = PAR_NONE; break;
            case 1: set.m_comport_settings.parity = PAR_EVEN; break;
            case 2: set.m_comport_settings.parity = PAR_ODD;  break;
        }

		set.m_udp_settings.ip_address = ui->lineEdit_UDP_Address->text();
		set.m_udp_settings.port = ui->spinBox_UDP_Port->value();

        set.m_wrapper_settings.enable = ui->groupBoxWrapper->isChecked();
        set.m_wrapper_settings.local_address = ui->spinBox_local_addr->value();
        set.m_wrapper_settings.remote_address = ui->spinBox_remote_addr->value();

        set.m_code1 = ui->lineEdit_Code1->text();
        set.m_code2 = ui->lineEdit_Code2->text();

        set.m_timeout_descriptor = ui->spinBox_timeout_descriptor->value();
        set.m_period_telemetry = ui->spinBox_period_telemetry->value();

        res = true; // use accept new settings
    }

    return res;
}
//---------------------------------------------------------------------------------------------------
void SettingsDlg::groupboxComPort_clicked(bool state)
{
    ui->groupBoxUDP->setChecked(!state);
}
//---------------------------------------------------------------------------------------------------
void SettingsDlg::groupboxUDP_clicked(bool state)
{
    ui->groupBoxComPort->setChecked(!state);
}


