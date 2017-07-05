#ifndef FIRMWAREDUMPVIEWER_H
#define FIRMWAREDUMPVIEWER_H

#include <QDialog>
#include <QByteArray>
#include <QMessageBox>
#include <QHBoxLayout>
#include "hexedit/qhexedit.h"

namespace Ui {
class FirmwareDumpViewer;
}

class FirmwareDumpViewer : public QDialog
{
    Q_OBJECT
    
public:
    explicit FirmwareDumpViewer(QWidget *parent = 0);
    ~FirmwareDumpViewer();

    void setFirmware(const QByteArray & value);
    void setDump(const QByteArray & value);

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_compare_clicked();

private:
    Ui::FirmwareDumpViewer *ui;

    QByteArray m_firmware;
    QByteArray m_dump;

    QHexEdit *hexedit_left;
    QHexEdit *hexedit_right;

    QScrollBar *m_vscrollbar_left;
    QScrollBar *m_vscrollbar_right;

    QScrollBar *m_hscrollbar_left;
    QScrollBar *m_hscrollbar_right;

public Q_SLOTS:
    void show();

};

#endif // FIRMWAREDUMPVIEWER_H
