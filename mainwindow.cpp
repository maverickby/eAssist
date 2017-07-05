#include <Qt>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ED/EDLogger.h"
#include "ED/EDUdpInterface.h"
#include "ED/EDComPortInterface.h"

//-----------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_interface(nullptr)
{
    ui->setupUi(this);

    m_applicationdirpath = qApp->applicationDirPath();

    ui->mainToolBar->setWindowTitle("Toolbar");

    m_logger = new DockLogger(this);

    m_interface  = new EDUdpInterface(QString("192.168.200.220"), 1011);
    m_ed         = new ED(m_interface);

    settings_dlg = new SettingsDlg(this);
    m_signals_tree = new SignalsTree(m_ed, this);

    addDockWidget(Qt::BottomDockWidgetArea, m_logger);

    addDockWidget(Qt::LeftDockWidgetArea, m_signals_tree);
    //m_signals_tree->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_signals_ctrl = new SignalsDockWidget(m_ed, this);
    addDockWidget(Qt::RightDockWidgetArea, m_signals_ctrl);
    //m_signals_ctrl->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_telemetry = new TelemetryWidget(&m_tabs, *m_ed);
    m_tabs.addTab(m_telemetry, "Telemetry");
    setCentralWidget(&m_tabs);

    CreateActions();
    CreateMenu();
    CreateToolBar();

    m_firmware = new Firmware(*m_ed, *this);

    connect( m_signals_tree, SIGNAL(SignalsSelected(QVector<EDSignal*>&)), m_signals_ctrl, SLOT(setSignals(QVector<EDSignal*>&)) );

    logger().WriteLn("Start...", Qt::darkGreen);

    // restore settings
    settings().Restore(m_applicationdirpath);
    update_on_change_settings();
}
//-----------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    settings().Save(m_applicationdirpath);

    delete ui;

    delete m_firmware;
    delete m_telemetry;
    delete m_ed;
    delete m_interface;
}
//-----------------------------------------------------------------------------------------
void MainWindow::CreateActions()
{
    // Sysem menu items
        act_CheckChannel = new QAction(tr("&Check channel"), this);
        act_CheckChannel->setStatusTip(tr("Check channel for communication"));
        connect(act_CheckChannel, SIGNAL(triggered()), this, SLOT(sysCheckChannel()));

        act_DevInfo = new QAction(tr("&Device information"), this);
        act_DevInfo->setStatusTip(tr("Device information"));
        connect(act_DevInfo, SIGNAL(triggered()), this, SLOT(sysDevInfo()));

        act_Reset = new QAction(tr("&Reset"), this);
        act_Reset->setStatusTip(tr("Device reset"));
        connect(act_Reset, SIGNAL(triggered()), this, SLOT(sysReset()));

        act_SelfTest = new QAction(tr("&Self Test"), this);
        act_SelfTest->setStatusTip(tr("Devices self test"));
        connect(act_SelfTest, SIGNAL(triggered()), this, SLOT(sysSelfTest()));

        act_SaveSettings = new QAction(tr("&Save settings"), this);
        act_SaveSettings->setStatusTip(tr("Save settings"));
        connect(act_SaveSettings, SIGNAL(triggered()), this, SLOT(sysSaveSettings()));

        act_TestChannel = new QAction(tr("&Test channel"), this);
        act_TestChannel->setStatusTip(tr("Test channel"));
        connect(act_TestChannel, SIGNAL(triggered()), this, SLOT(sysTestChannel()));

        act_ScanMode = new QAction(tr("&Scan Mode"), this);
        act_ScanMode->setStatusTip(tr("Scan Mode"));
        act_ScanMode->setCheckable(true);
        connect(act_ScanMode, SIGNAL(triggered()), this, SLOT(sysScanMode()));

        act_Scan = new QAction(tr("&Scan"), this);
        act_Scan->setStatusTip(tr("scan device capabilities"));
        act_Scan->setIcon(QIcon(":/images/start.png"));
        act_Scan->setShortcut(Qt::Key_F5);
        connect(act_Scan, SIGNAL(triggered()), this, SLOT(sysScan()));

    // Program menu items
        act_Open = new QAction(tr("&Open"), this);
        act_Open->setStatusTip(tr("Open firmware file"));
        connect(act_Open, SIGNAL(triggered()), this, SLOT(prgOpen()));

        act_Save = new QAction(tr("&Save"), this);
        act_Save->setStatusTip(tr("Save firmware file"));
        connect(act_Save, SIGNAL(triggered()), this, SLOT(prgSave()));

        act_Erase = new QAction(tr("&Erase"), this);
        act_Erase->setStatusTip(tr("Erase program memory"));
        connect(act_Erase, SIGNAL(triggered()), this, SLOT(prgErase()));

        act_Write = new QAction(tr("&Write"), this);
        act_Write->setStatusTip(tr("Write firmware file"));
        connect(act_Write, SIGNAL(triggered()), this, SLOT(prgWrite()));

        act_ReadAll = new QAction(tr("&Read All"), this);
        act_ReadAll->setStatusTip(tr("Read all program memory"));
        connect(act_ReadAll, SIGNAL(triggered()), this, SLOT(prgReadAll()));

        act_ReadDataOnly = new QAction(tr("&Read data only"), this);
        act_ReadDataOnly->setStatusTip(tr("Read only program memory occupied by firmware"));
        connect(act_ReadDataOnly, SIGNAL(triggered()), this, SLOT(prgReadDataOnly()));

        act_Compare = new QAction(tr("&Compare"), this);
        act_Compare->setStatusTip(tr("Firmware verification"));
        connect(act_Compare, SIGNAL(triggered()), this, SLOT(prgCompare()));

        act_ResetDevice = new QAction(tr("&Reset device"), this);
        act_ResetDevice->setStatusTip(tr("Reset device"));
        connect(act_ResetDevice, SIGNAL(triggered()), this, SLOT(prgResetDevice()));

        act_Auto = new QAction(tr("&Auto"), this);
        act_Auto->setStatusTip(tr("Write...read..compare"));
        connect(act_Auto, SIGNAL(triggered()), this, SLOT(prgAuto()));

    // Tools
        act_OpenQML = new QAction(tr("&Open QML form"), this);
        act_OpenQML->setStatusTip(tr("Open QML form"));
        connect(act_OpenQML, SIGNAL(triggered()), this, SLOT(toolsOpenQML()));

        act_ClearLog = new QAction(tr("&Clear log"), this);
        act_ClearLog->setStatusTip(tr("Clear log"));
        connect(act_ClearLog, SIGNAL(triggered()), this, SLOT(toolsClearLog()));

        act_ViewFirmwareDump = new QAction(tr("&View firmware/dump"), this);
        act_ViewFirmwareDump->setStatusTip(tr("Firmware and dump viewer"));
        connect(act_ViewFirmwareDump, SIGNAL(triggered()), this, SLOT(toolsViewFirmwareDump()));

        act_ViewSettings = new QAction(tr("&Settings"), this);
        act_ViewSettings->setStatusTip(tr("View settings"));
        connect(act_ViewSettings, SIGNAL(triggered()), this, SLOT(toolsViewSettings()));

        act_Code1 = new QAction(tr("Code1"), this);
        act_Code1->setStatusTip(tr("Send Code1"));
        connect(act_Code1, SIGNAL(triggered()), this, SLOT(toolCode1()));

        act_Code2 = new QAction(tr("Code2"), this);
        act_Code2->setStatusTip(tr("Send Code2"));
        connect(act_Code2, SIGNAL(triggered()), this, SLOT(toolCode2()));
}
//-----------------------------------------------------------------------------------------
void MainWindow::CreateMenu()
{
    m_System = new QMenu("System", this);
    m_System->addAction(act_Scan);
    m_System->addSeparator();
    ui->menuBar->addMenu(m_System);
    m_System->addAction(act_CheckChannel);
    m_System->addAction(act_DevInfo);
    m_System->addAction(act_Reset);
    m_System->addAction(act_SelfTest);
    m_System->addAction(act_SaveSettings);
    m_System->addAction(act_TestChannel);
    m_System->addSeparator();
    m_System->addAction(act_ScanMode);

    m_Prog = new QMenu("Program", this);
    ui->menuBar->addMenu(m_Prog);
    m_Prog->addAction(act_Auto);
    m_Prog->addSeparator();
    m_Prog->addAction(act_Open);
    m_Prog->addAction(act_Save);
    m_Prog->addSeparator();
    m_Prog->addAction(act_Erase);
    m_Prog->addAction(act_Write);
    m_Prog->addAction(act_ReadAll);
    m_Prog->addAction(act_ReadDataOnly);
    m_Prog->addAction(act_Compare);
    m_Prog->addAction(act_ResetDevice);

    m_View = new QMenu("View", this);
    ui->menuBar->addMenu(m_View);
    m_View->addAction(m_logger->toggleViewAction());
    m_View->addAction(m_signals_tree->toggleViewAction());
    m_View->addAction(m_signals_ctrl->toggleViewAction());

    m_Tools = new QMenu("Tools", this);
    ui->menuBar->addMenu(m_Tools);
    m_Tools->addAction(act_OpenQML);
    m_Tools->addAction(act_ClearLog);
    m_Tools->addAction(act_ViewFirmwareDump);
    m_Tools->addSeparator();
    m_Tools->addAction(act_ViewSettings);
    m_Tools->addSeparator();
    m_Tools->addAction(act_Code1);
    m_Tools->addAction(act_Code2);
}
//-----------------------------------------------------------------------------------------
void MainWindow::CreateToolBar()
{
    ui->mainToolBar->addWidget(new QLabel("Device: ", this));

    devicesComboBox = new QComboBox(this);
    devicesComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ui->mainToolBar->addWidget(devicesComboBox);

    ui->mainToolBar->addSeparator();
    m_progressbar = new QProgressBar(this);
    act_progress = ui->mainToolBar->addWidget(m_progressbar);
    setProgressVisible(false);
}
//-----------------------------------------------------------------------------------------
bool MainWindow::get_device_id(int &device_id)
{
    if(devicesComboBox->count() == 0)
    {
        logger().WriteLn("Imposible perfom command: no devices selected", Qt::red);
        return false;
    }
    device_id = devicesComboBox->currentIndex();
    return true;
}
//-----------------------------------------------------------------------------------------
void MainWindow::update_on_change_settings()
{
    delete m_interface;

	Settings &set = settings();
	UDP_Settings udp_set = set.getUDPSettings();
	ComPort_Settings com_set = set.getComPortSettings();
    Wrapper_Settings wrp_set = set.getWrapperSettings();

    if(set.getInterfaceType() == IT_UDP)
    {
		m_interface = new EDUdpInterface(udp_set.ip_address, udp_set.port);
        m_ed->setInterface(m_interface);
        logger().WriteLn("UDP interface selected:", Qt::blue);
		logger().WriteLn("    IP Address - " + udp_set.ip_address, Qt::blue);
		logger().WriteLn("      UDP port - " + QString::number(udp_set.port), Qt::blue);
    } else if(set.getInterfaceType() == IT_COMPORT)
    {
        m_interface = new EDComPortInterface(com_set.port, com_set.baudrate, com_set.parity);
        m_ed->setInterface(m_interface);
        logger().WriteLn("ComPort interface selected:", Qt::blue);
		logger().WriteLn("      Port - " + QString::number(com_set.port), Qt::blue);
		logger().WriteLn("  Baudrate - " + QString::number(com_set.baudrate), Qt::blue);
        logger().Write("  Parity   - ", Qt::blue);
        switch(com_set.parity)
        {
        case PAR_NONE: logger().WriteLn("none", Qt::blue); break;
        case PAR_EVEN: logger().WriteLn("even", Qt::blue); break;
        case PAR_ODD:  logger().WriteLn("odd",  Qt::blue); break;
        default:
            break;
        }

    } else
    {
        throw "Wrang interface type";
    }

    m_ed->setWrapperSettings(wrp_set.enable, wrp_set.local_address, wrp_set.remote_address);

    m_telemetry->setPeriod(set.getPeriodTelemetry());
    m_ed->setTimeoutDescriptor(set.getTimeoutDescriptor());
}
//-----------------------------------------------------------------------------------------
void MainWindow::setProgressValue(unsigned int progress)
{
    m_progressbar->setValue(progress);
}
//-----------------------------------------------------------------------------------------
void MainWindow::setProgressVisible(bool state)
{
    act_progress->setVisible(state);
}
//-----------------------------------------------------------------------------------------
void MainWindow::setProgressParam(unsigned int min_value, unsigned int max_value)
{
    m_progressbar->setMinimum(min_value);
    m_progressbar->setMaximum(max_value);
}
//-----------------------------------------------------------------------------------------
void MainWindow::sysCheckChannel() { m_ed->Sys().CheckChannel(); }
//-----------------------------------------------------------------------------------------
void MainWindow::sysDevInfo()
{
    QString tmp;
    m_ed->Sys().ReqDevInfo(tmp);
    QMessageBox::information(this, "Message", tmp, QMessageBox::Ok);
}
//-----------------------------------------------------------------------------------------
void MainWindow::sysReset() { m_ed->Sys().Reset(); }
//-----------------------------------------------------------------------------------------
void MainWindow::sysSelfTest()
{
    QString tmp;
    m_ed->Sys().SelfTest(tmp);
    QMessageBox::information(this, "Message", tmp, QMessageBox::Ok);
}
//-----------------------------------------------------------------------------------------
void MainWindow::sysSaveSettings() { m_ed->Sys().SaveSettings(); }
//-----------------------------------------------------------------------------------------
void MainWindow::sysTestChannel()
{
    const int sig_size = 1000;
    // prepare data for test
    QByteArray signature;
    signature.resize(sig_size);
    for(int i = 0; i < sig_size; i++) signature[i] = 1;//(i+1);

    m_ed->Sys().TestChannel(signature);
}
//-----------------------------------------------------------------------------------------
void MainWindow::sysScanMode()
{
    bool new_scan_mode = !m_ed->Sys().isScanMode();

    m_ed->Sys().SetScanMode(new_scan_mode);

    act_ScanMode->setChecked(m_ed->Sys().isScanMode());
}
//-----------------------------------------------------------------------------------------
void MainWindow::sysScan()
{
    devicesComboBox->clear();

    m_ed->Scan();

    // *** Adjust GUI for detected features

    // Programming
	EDPrg &prg = m_ed->Prg();

    if(prg.Available())
    {
        m_Prog->setEnabled(true);
        devicesComboBox->setEnabled(true);

        devicesComboBox->clear();
        foreach (EDDeviceDescriptor item, prg.Descriptors()) {
            devicesComboBox->addItem(item.name);
        }
    } else
    {
        m_Prog->setEnabled(false);
        devicesComboBox->setEnabled(false);
    }
}
//-----------------------------------------------------------------------------------------
void MainWindow::prgOpen() { m_firmware->Open(); }
//-----------------------------------------------------------------------------------------
void MainWindow::prgSave() { m_firmware->Save(); }
//-----------------------------------------------------------------------------------------
void MainWindow::prgErase() { int devide_id; if(!get_device_id(devide_id)) return; m_firmware->Erase(devide_id); }
//-----------------------------------------------------------------------------------------
void MainWindow::prgWrite() { int device_id; if(!get_device_id(device_id)) return; m_firmware->Write(device_id); }
//-----------------------------------------------------------------------------------------
void MainWindow::prgReadAll() { int device_id; if(!get_device_id(device_id)) return; m_firmware->ReadAll(device_id); }
//-----------------------------------------------------------------------------------------
void MainWindow::prgReadDataOnly() { int device_id; if(!get_device_id(device_id)) return; m_firmware->ReadDataOnly(device_id); }
//-----------------------------------------------------------------------------------------
void MainWindow::prgCompare() { m_firmware->Compare(); }
//-----------------------------------------------------------------------------------------
void MainWindow::prgResetDevice() { int device_id; if(!get_device_id(device_id)) return; m_firmware->ResetDevice(device_id); }
//-----------------------------------------------------------------------------------------
void MainWindow::prgAuto() { int device_id; if(!get_device_id(device_id)) return; m_firmware->Auto(device_id); }
//-----------------------------------------------------------------------------------------
void MainWindow::toolsOpenQML()
{
    QFileDialog dialog;

    QString filtr = "*.qml";

    dialog.setFileMode(QFileDialog::ExistingFile);
    QString filename = dialog.getOpenFileName(nullptr, "Open File", "", "QML Files (*.qml)", &filtr);
    if(QFile::exists(filename))
    {
        if(SFormDockWidget::isOpened(filename))
        {
            logger().WriteLn("QML file - " + filename +". " + "already opened", Qt::red);
            return;
        }
        SFormDockWidget   *sform = new SFormDockWidget(m_ed, filename, this);
        sform->setFloating(true);
        sform->setAttribute(Qt::WA_DeleteOnClose, true);
        sform->show();
    }
}
//-----------------------------------------------------------------------------------------
void MainWindow::toolsClearLog()
{
    m_logger->Clear();
}
//-----------------------------------------------------------------------------------------
void MainWindow::toolsViewFirmwareDump()
{
    m_firmware->ViewFirmwareDump();
}
//-----------------------------------------------------------------------------------------
void MainWindow::toolsViewSettings()
{
    logger().WriteLn("Open settings...", Qt::gray);
    if(settings_dlg->Execute())
        // new settings aproved
    {
        update_on_change_settings();
    }
}
//-----------------------------------------------------------------------------------------
void MainWindow::toolCode1()
{
    QByteArray code1 = settings().getCode1ByteArray();
    bool opened = m_interface->isOpen();
    if(!opened) m_interface->open(QIODevice::ReadWrite);
        m_interface->write(code1.data(), code1.size());
    if(!opened) m_interface->close();
}
//-----------------------------------------------------------------------------------------
void MainWindow::toolCode2()
{
    QByteArray code2 = settings().getCode2ByteArray();
    bool opened = m_interface->isOpen();
    if(!opened) m_interface->open(QIODevice::ReadWrite);
        m_interface->write(code2.data(), code2.size());
    if(!opened) m_interface->close();
}

