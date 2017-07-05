#ifndef SFORMDOCKWIDGET_H
#define SFORMDOCKWIDGET_H

#include <QDockWidget>
#include <QList>
#include <QString>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include "../ED/ed.h"
#include "SFormWidget.h"

class SFormDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit SFormDockWidget(ED *ed, QString filename, QWidget *parent = nullptr);
    virtual ~SFormDockWidget();

    static bool isOpened(const QString &filename);
signals:

public slots:

private slots:
    void active();
    void read();
    void write();

private:
    ED *m_ed;
    SFormWidget m_sform;
    QString m_filename;

    static QList<QString> m_opened_files;

    QMainWindow     *m_window;
    QToolBar        *m_toolbar;
    QAction         *act_active;
    QAction         *act_read;
    QAction         *act_write;
};

#endif // SFORMDOCKWIDGET_H
