#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QComboBox>
#include <QLabel>
#include <QFileDialog>
#include <QProgressBar>
#include <QTabWidget>
#include "DockLogger.h"
#include "SignalsTree.h"
#include "SignalsDockWidget.h"
#include "SettingsDlg.h"
#include "Settings.h"
#include "TelemetryWidget.h"
#include "Firmware.h"
#include "SForm/SFormDockWidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public EDPrgProgress
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void setProgressValue(unsigned int progress);
    virtual void setProgressVisible(bool state);
    virtual void setProgressParam(unsigned int min_value, unsigned int max_value);
    
private:
    void CreateActions();
    void CreateMenu();
    void CreateToolBar();

    bool get_device_id(int & device_id);

    void update_on_change_settings();

    Ui::MainWindow *ui;
    SettingsDlg *settings_dlg;

    QComboBox *devicesComboBox;

    QMenu *m_System;
    QAction *act_CheckChannel;
    QAction *act_DevInfo;
    QAction *act_Reset;
    QAction *act_SelfTest;
    QAction *act_SaveSettings;
    QAction *act_TestChannel;
    QAction *act_ScanMode;
    QAction *act_Scan;

    QMenu *m_Prog;
    QAction *act_Open;
    QAction *act_Save;
    QAction *act_Erase;
    QAction *act_Write;
    QAction *act_ReadAll;
    QAction *act_ReadDataOnly;
    QAction *act_Compare;
    QAction *act_ResetDevice;
    QAction *act_Auto;

    QMenu *m_View;

    QMenu *m_Tools;
    QAction *act_OpenQML;
    QAction *act_ClearLog;
    QAction *act_ViewFirmwareDump;
    QAction *act_ViewSettings;
    QAction *act_Code1;
    QAction *act_Code2;

    QAction *act_progress;

    QIODevice *m_interface;
    ED        *m_ed;

    Firmware  *m_firmware;

    QProgressBar *m_progressbar;

    DockLogger        *m_logger;
    SignalsTree       *m_signals_tree;
    SignalsDockWidget *m_signals_ctrl;

    QTabWidget      m_tabs;
    TelemetryWidget *m_telemetry;

    QString m_applicationdirpath;

private slots:
    void sysCheckChannel();
    void sysDevInfo();
    void sysReset();
    void sysSelfTest();
    void sysSaveSettings();
    void sysTestChannel();
    void sysScanMode();
    void sysScan();

    void prgOpen();
    void prgSave();
    void prgErase();
    void prgWrite();
    void prgReadAll();
    void prgReadDataOnly();
    void prgCompare();
    void prgResetDevice();
    void prgAuto();

    void toolsOpenQML();
    void toolsClearLog();
    void toolsViewFirmwareDump();
    void toolsViewSettings();
    void toolCode1();
    void toolCode2();
};

#endif // MAINWINDOW_H
