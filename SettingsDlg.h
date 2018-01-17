#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <QDialog>
#include "Settings.h"
#include "mainwindow.h"

class MainWindow;

namespace Ui {
class SettingsDlg;
}

class SettingsDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDlg(QWidget *parent = 0);
    ~SettingsDlg();

    bool Execute();

private:
    Ui::SettingsDlg *ui;
	MainWindow* mainWIndow;

private slots:
    void groupboxComPort_clicked(bool state);
    void groupboxUDP_clicked(bool state);
    void on_buttonBox_clicked(QAbstractButton *button);
};

#endif // SETTINGSDLG_H
