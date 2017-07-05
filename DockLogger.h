#ifndef DOCKLOGGER_H
#define DOCKLOGGER_H

#include <QDockWidget>
#include <QTextBrowser>
#include <QMessageBox>
#include "ED/EDILogger.h"

#define MAX_NUM_RECORDS 1000

class DockLogger : public QDockWidget, public EDILogger
{
    Q_OBJECT
public:
    explicit DockLogger(QWidget *parent = 0);
    ~DockLogger();

    virtual void Write                  (const QString &value, Qt::GlobalColor color);
    virtual void WriteLn                (const QString &value, Qt::GlobalColor color);
    virtual void WriteLnMessage         (const QString &value, Qt::GlobalColor color);
    virtual void WriteLnMessageCritical (const QString &value, Qt::GlobalColor color);
    virtual void WriteCommandResult     (EDCommand::EDCommandResult result);
    virtual void setPauseLogging        (bool state);
    virtual bool getPauseLogging        ();

    virtual void WritePercent           (int value, Qt::GlobalColor color);
    virtual void UpdatePercent          (int value, Qt::GlobalColor color);

    virtual void WriteSignalInfo        (const EDSignal &value);

    void Clear();
    
private:
   QTextBrowser *text_browser;

   bool m_pause;

   int m_num_last_chars;
};

#endif // DOCKLOGGER_H
