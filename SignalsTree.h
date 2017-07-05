#ifndef SIGNALSTREE_H
#define SIGNALSTREE_H

#include <QDockWidget>
#include <QTreeWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QSplitter>
#include <QKeyEvent>
#include <QCheckBox>
#include <QToolBar>
#include <QAction>
#include "ED/ED.h"

class SignalsTree : public QDockWidget
{
    Q_OBJECT
public:
    explicit SignalsTree(ED *ed, QWidget *parent = nullptr);
    ~SignalsTree();

private:
    void create_toolbar();
    const QTreeWidgetItem *get_signals_widget(int signal_id);
    const QTreeWidgetItem *get_parent_widget (int signal_id);

    void table_setup();
    void table_show_signal(EDSignal &value);
    void table_clear_signal();
    void table_clear();

    // application data
    ED *m_ed;
    EDSignal *m_sig_selected;

    // controls
    QWidget     *m_widget;
    QSplitter   *m_splitter;
    QVBoxLayout *m_vlayout;
    QTreeWidget *m_tree;
    QToolBar    *m_toolbar;

    QTableWidgetItem *m_t_value;
    QTableWidgetItem *m_t_period;
    QTableWidgetItem *m_t_physics_type;

    QTableWidgetItem *m_t_en_read;
    QTableWidgetItem *m_t_en_settings;
    QTableWidgetItem *m_t_en_ctrl;
    QTableWidgetItem *m_t_en_telemetry;
    QTableWidgetItem *m_t_en_signature;
    QTableWidgetItem *m_t_type_class;
    QTableWidgetItem *m_t_type;
    QTableWidgetItem *m_t_coef;

    QCheckBox *m_checkbox_fix;
    QCheckBox *m_checkbox_intelemetry;

    QTableWidget *m_table;

    // actions
    QAction *act_save;
    QAction *act_read;

signals:
    void SignalsSelected(QVector<EDSignal*> &signals_list);

private slots:
    void NewSignals();
    void processSignalValueChanged(EDSignal *signal);
    void update_tree();

    void itemPressed(QTreeWidgetItem *item, int column);
    void itemChanged(QTableWidgetItem *item);
    void itemDoubleClicked(QTableWidgetItem *item);
    void TreeItemDoubleClicked(QTreeWidgetItem *item, int column);

    void clicked_fix(bool checked);
    void clicked_intelemetry(bool checked);

    void save();
    void read();
};

#endif // SIGNALSTREE_H
