#include <cstdint>
#include <QFileDialog>
#include <QMessageBox>
#include <QMap>
#include "SignalsTree.h"
#include "ED/EDLogger.h"

static const int TABLE_MIN_SIZE = 105;
static const int TABLE_MAX_SIZE = 235;

static const int NUM_COLS = 2;
static const int NUM_ROWS = 13;

//--------------------------------------------------------------------------------------------
SignalsTree::SignalsTree(ED *ed, QWidget *parent):
    QDockWidget("Signals tree", parent),
    m_sig_selected(nullptr)
{
    m_checkbox_fix = new QCheckBox();
    m_checkbox_intelemetry = new QCheckBox();

    m_ed = ed;

    m_widget = new QWidget(this);

    m_vlayout = new QVBoxLayout(this);

    m_splitter = new QSplitter(this);
    m_splitter->setOrientation(Qt::Vertical);

    create_toolbar();

    m_tree = new QTreeWidget(this);
    m_tree->setColumnCount(1);
    m_tree->setHeaderHidden(true);

    m_table = new QTableWidget(this);
    table_setup();

    m_splitter ->addWidget(m_tree);
    m_splitter ->addWidget(m_table);
    m_vlayout  ->addWidget(m_toolbar);
    m_vlayout  ->addWidget(m_splitter);
    m_widget   ->setLayout(m_vlayout);
    this       ->setWidget(m_widget);

    // set default sizes
    QList<int> sizes;
    sizes << 50000 << TABLE_MIN_SIZE;
    m_splitter->setSizes(sizes);

    connect(m_tree, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this, SLOT(itemPressed(QTreeWidgetItem*,int)));

    connect(m_checkbox_fix, SIGNAL(clicked(bool)), this, SLOT(clicked_fix(bool)));
    connect(m_checkbox_intelemetry, SIGNAL(clicked(bool)), this, SLOT(clicked_intelemetry(bool)));
    connect(m_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(TreeItemDoubleClicked(QTreeWidgetItem*,int)));

    // connecting to ED callbacks
    connect(&m_ed->Signals(), SIGNAL(SignalValueChanged(EDSignal*)), this, SLOT(processSignalValueChanged(EDSignal*)));
    connect(&m_ed->Signals(), SIGNAL(NewSignalsList()), this, SLOT(NewSignals()));
    connect(&m_ed->Signals(), SIGNAL(SignalFixModeChanged(EDSignal*)), this, SLOT(update_tree()));
    connect(&m_ed->Signals(), SIGNAL(SignalTelModeChanged(EDSignal*)), this, SLOT(update_tree()));
}
//--------------------------------------------------------------------------------------------
SignalsTree::~SignalsTree()
{
    table_clear();
    delete m_checkbox_fix;
    delete m_checkbox_intelemetry;
}
//--------------------------------------------------------------------------------------------
void SignalsTree::NewSignals()
{
    m_tree->clear();

    disconnect(m_table, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(itemDoubleClicked(QTableWidgetItem*)));

    m_sig_selected = nullptr;

    bool second_pass_needed = false;
    // first pass
    int size = m_ed->Signals().count();

    for(int i = 0; i < size; i++)
    {
        int parent_id = m_ed->Signals().at(i).getDescriptor().parent_id;
        QTreeWidgetItem *parent_item = const_cast<QTreeWidgetItem *>(get_parent_widget(i));
        QTreeWidgetItem *new_item;

        if(parent_id == -1) // top level
        {
            new_item = new QTreeWidgetItem(m_tree);
            m_ed->Signals()[i].setUser_data(new_item);
            new_item->setData(0, Qt::UserRole, i);
            new_item->setText(0, m_ed->Signals()[i].getDescriptor().name);
        }
        else
        if(parent_item != nullptr) // parent is here
        {
            new_item = new QTreeWidgetItem(parent_item);
            m_ed->Signals()[i].setUser_data(new_item);
            new_item->setData(0, Qt::UserRole, i);
            new_item->setText(0, m_ed->Signals()[i].getDescriptor().name);
        }
        else // parent not yet born
        {
            second_pass_needed = true;
        }
    }

    // second pass
    if(second_pass_needed)
    {
        for(int i = 0; i < size; i++)
        {
            if(m_ed->Signals()[i].getUser_data() == nullptr)
            {
                QTreeWidgetItem *parent_item = const_cast<QTreeWidgetItem*>(get_parent_widget(i));
                QTreeWidgetItem *new_item;

                new_item = new QTreeWidgetItem(parent_item);
                m_ed->Signals()[i].setUser_data(new_item);
                new_item->setData(0, Qt::UserRole, i);
                new_item->setText(0, m_ed->Signals()[i].getDescriptor().name);
            }
        }
    }

    // prepare table
    table_clear_signal();

    update_tree();
}
//--------------------------------------------------------------------------------------------
void SignalsTree::create_toolbar()
{
    m_toolbar = new QToolBar(this);

    act_save = new QAction(tr("Save"), this);
    connect(act_save, SIGNAL(triggered()), this, SLOT(save()));

    act_read = new QAction(tr("Read"), this);
    connect(act_read, SIGNAL(triggered()), this, SLOT(read()));

    m_toolbar->addAction(act_save);
    m_toolbar->addAction(act_read);
}
//--------------------------------------------------------------------------------------------
const QTreeWidgetItem *SignalsTree::get_signals_widget(int signal_id)
{
	if (m_ed->Signals().count() < (static_cast<int>(signal_id) + 1)) return nullptr;

    return reinterpret_cast<const QTreeWidgetItem *>(m_ed->Signals().at(signal_id).getUser_data());
}
//--------------------------------------------------------------------------------------------
const QTreeWidgetItem *SignalsTree::get_parent_widget(int signal_id)
{
    int parent_id = m_ed->Signals().at(signal_id).getDescriptor().parent_id;

    if(parent_id == -1) return nullptr;

    return get_signals_widget(parent_id);
}
//--------------------------------------------------------------------------------------------
void SignalsTree::table_setup()
{
    QColor col1_1(195, 254, 181);
    QColor col1_2(186, 244, 173);
    QColor col2_1(252, 253, 156);
    QColor col2_2(235, 235, 144);
    QColor col;
    QTableWidgetItem *tmp;

    // global setups
    m_table->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_table->setMinimumHeight(TABLE_MIN_SIZE);
    m_table->setMaximumHeight(TABLE_MAX_SIZE);
    m_table->setColumnCount(NUM_COLS);
    m_table->setRowCount(NUM_ROWS);
    m_table->setCornerButtonEnabled(false);
    m_table->setShowGrid(true);
    for(int i = 0; i < m_table->rowCount(); i++) m_table->setRowHeight(i, 16);
    m_table->verticalHeader()->hide();
    m_table->setHorizontalHeaderLabels(QStringList() << "Property" << "Value");
    m_table->horizontalHeader()->setSectionsClickable(false);

    // create items
    for(int c = 0; c < NUM_COLS; c++)
        for(int r = 0; r < NUM_ROWS; r++)
        {
            tmp = new QTableWidgetItem();
            if(r < 5)
            {
                if( (r % 2) != 0) col = col1_1;
                else col = col1_2;
            }else
            {
                if( (r % 2) != 0) col = col2_1;
                else col = col2_2;
            }
            tmp->setBackgroundColor(col);
            m_table->setItem(r, c, tmp);
        }

    // values names
    m_table->item(0, 0) ->setText("Value");
    m_table->item(1, 0) ->setText("Fixed");
    m_table->item(2, 0) ->setText("In telemetry");
    m_table->item(3, 0) ->setText("Period");
    m_table->item(4, 0) ->setText("Physical");

    m_table->item(5, 0) ->setText("read");
    m_table->item(6, 0) ->setText("setting");
    m_table->item(7, 0) ->setText("control");
    m_table->item(8, 0) ->setText("telemetry");
    m_table->item(9, 0) ->setText("signature");
    m_table->item(10, 0)->setText("class");
    m_table->item(11, 0)->setText("type");
    m_table->item(12, 0)->setText("coefficient");

    // assign pointers to values items
    m_t_value           = m_table->item(0, 1);
    m_t_period          = m_table->item(3, 1);
    m_t_physics_type    = m_table->item(4, 1);
    m_t_en_read         = m_table->item(5, 1);
    m_t_en_settings     = m_table->item(6, 1);
    m_t_en_ctrl         = m_table->item(7, 1);
    m_t_en_telemetry    = m_table->item(8, 1);
    m_t_en_signature    = m_table->item(9, 1);
    m_t_type_class      = m_table->item(10, 1);
    m_t_type            = m_table->item(11, 1);
    m_t_coef            = m_table->item(12, 1);

    // make editable items with white background
    m_t_value->setBackgroundColor(Qt::white);
    m_table->setCellWidget(1, 1, m_checkbox_fix);
    m_table->setCellWidget(2, 1, m_checkbox_intelemetry);

    // set flags for values items
    for(int i = 3; i < NUM_ROWS; i++)
        m_table->item(i, 1)->setFlags(Qt::ItemIsEnabled);
    for(int i = 0; i < NUM_ROWS; i++)  // disable editing names of rows
        m_table->item(i, 0)->setFlags(Qt::ItemIsEnabled);

    m_table->horizontalHeader()->setStretchLastSection(true); // to look good

    // event handlers assignment

}
//--------------------------------------------------------------------------------------------
void SignalsTree::table_show_signal(EDSignal &value)
{
    QString tmp_type;
    QString tmp_class;
    QString tmp_value;

    switch(value.getDescriptor().type_class)
    {
    case SCL_STD:       tmp_class = "STD";      break;
    case SCL_ENUM:      tmp_class = "ENUM";     break;
    case SCL_ARRAY1:    tmp_class = "ARRAY1";   break;
    case SCL_ARRAY2:    tmp_class = "ARRAY2";   break;
    }

    switch(value.getDescriptor().type)
    {
    case STYPE_STRING:  tmp_type = "STRING";    break;
    case STYPE_BOOL:    tmp_type = "BOOL";      break;
    case STYPE_BYTE:    tmp_type = "BYTE";      break;
    case STYPE_UINT:    tmp_type = "UINT";      break;
    case STYPE_INT:     tmp_type = "INT";       break;
    case STYPE_LONG:    tmp_type = "LONG";      break;
    case STYPE_ULONG:   tmp_type = "ULONG";     break;
    case STYPE_FLOAT:   tmp_type = "FLOAT";     break;
    }

    if(value.getDescriptor().type_class == SCL_STD)
    {
         bool res = m_ed->Signals().getSignalValueStr(value, tmp_value, false);
         if(!res) tmp_value = "error";
    }else
    {
        tmp_value = "error";
    }

    // value field
    m_t_value->setText(tmp_value);
    if(value.getDescriptor().type == STYPE_STRING) m_t_value->setFlags(Qt::ItemIsEnabled);
    else m_t_value->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);

    // control mode checkbox
    if(value.getCtrl_mode() == CM_FIXED) m_checkbox_fix->setChecked(true);
    else m_checkbox_fix->setChecked(false);
    m_checkbox_fix->setEnabled(!value.isParent());

    // telemetry control checkbox
    if(value.getIntelemetry()) m_checkbox_intelemetry->setChecked(true);
    else m_checkbox_intelemetry->setChecked(false);
    m_checkbox_intelemetry->setEnabled(!value.isParent());

    m_t_period->setText(QString::number(value.getDescriptor().period, 10));

    QStringList list_measure_type;
    list_measure_type << "NONE" << "VOLTAGE" << "AMPERE" << "ANG_DEGREE" <<
            "RADIAN" << "SMT_METER" << "VOLT_ANG_DEGREE" << "METER_SECOND" << "CELSIUS" << "ANG_DEGREE_SECOND";
    m_t_physics_type->setText(list_measure_type.at(static_cast<int>(value.getDescriptor().measure_type)));

    if(value.getDescriptor().en_read) m_t_en_read->setText("true");
    else m_t_en_read->setText("false");

    if(value.getDescriptor().en_setting) m_t_en_settings->setText("true");
    else m_t_en_settings->setText("false");

    if(value.getDescriptor().en_ctrl) m_t_en_ctrl->setText("true");
    else m_t_en_ctrl->setText("false");

    if(value.getDescriptor().en_telemetry) m_t_en_telemetry->setText("true");
    else m_t_en_telemetry->setText("false");

    if(value.getDescriptor().en_signature) m_t_en_signature->setText("true");
    else m_t_en_signature->setText("false");

    m_t_type_class      ->setText(tmp_class);
    m_t_type            ->setText(tmp_type);

    m_t_coef->setText(QString::number(value.getDescriptor().coef, 'g', 10));

    connect(m_table, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(itemDoubleClicked(QTableWidgetItem*)));
}
//--------------------------------------------------------------------------------------------
void SignalsTree::table_clear_signal()
{
    m_t_value               ->setText("");
    m_checkbox_fix          ->setChecked(false);
    m_checkbox_intelemetry  ->setChecked(false);
    m_t_period              ->setText("");
    m_t_physics_type        ->setText("");
    m_t_en_read             ->setText("");
    m_t_en_settings         ->setText("");
    m_t_en_ctrl             ->setText("");
    m_t_en_telemetry        ->setText("");
    m_t_en_signature        ->setText("");
    m_t_type_class          ->setText("");
    m_t_type                ->setText("");
    m_t_coef                ->setText("");
}
//--------------------------------------------------------------------------------------------
void SignalsTree::table_clear()
{
    for(int c = 0; c < 2; c++)
        for(int r = 0; r < 13; r++)
        {
            delete m_table->item(r, c);
        }
}
//--------------------------------------------------------------------------------------------
void SignalsTree::update_tree()
{
	EDSignalsList &sig_list = m_ed->Signals();

	int size = sig_list.count(); 
    QTreeWidgetItem *pitem;
    QColor color;

    for(int i = 0; i < size; i++)
    {
		const EDSignal &sig = sig_list.at(i);
        pitem = const_cast<QTreeWidgetItem*>(static_cast<const QTreeWidgetItem*>(sig.getUser_data()));
        bool tel = sig.getIntelemetry();
        bool fix = (sig.getCtrl_mode() == CM_FIXED) || (sig.getCnt_fixed_childs() > 0);
        if(fix && tel) color = Qt::darkMagenta;
        else if( fix ) color = Qt::blue;
        else  if(tel) color = Qt::darkGreen;
        else color = Qt::black;
        pitem->setTextColor(0, color);
    }

	this->setWindowTitle("Signals(" + QString::number(sig_list.count(), 10) + ":" +
		QString::number(sig_list.getNumFixedSignals()) + ")");
}
//--------------------------------------------------------------------------------------------
void SignalsTree::itemPressed(QTreeWidgetItem *item, int column) //-V2009
{
    static EDSignal *sig_old = nullptr;

    if( (item == nullptr) || (column != 0) ) return;

    int signal_id = item->data(0, Qt::UserRole).toInt();// get signal_id from general purepose field data

    m_sig_selected = &m_ed->Signals()[signal_id];
    if(sig_old != m_sig_selected)
    {
        sig_old = m_sig_selected;

        table_show_signal(*m_sig_selected);

        // emit signal with selected signals
        QVector<EDSignal*> signals_list;
        m_ed->Signals().getSignalChilds(*m_sig_selected, signals_list);

        emit SignalsSelected(signals_list);
    }
}
//--------------------------------------------------------------------------------------------
void SignalsTree::itemChanged(QTableWidgetItem *item) //-V2009
{
    if(item == m_t_value)
    {
        disconnect(m_table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(itemChanged(QTableWidgetItem*)));

        if(m_sig_selected == nullptr) return;

        QString tmp_string = m_t_value->text();
        bool ok = m_ed->Signals().setSignalValueStr(*m_sig_selected, &tmp_string, 10);
        if(!ok) logger().WriteLn("Error while convert-save signal value", Qt::red);

        // read
        if(ok) // when convert-save was successfull
        {
            QString tmp_value;
            ok &= m_ed->Signals().getSignalValueStr(*m_sig_selected, tmp_value, false);
            m_t_value->setText(tmp_value);
            m_t_value->setBackgroundColor(Qt::white);
        } else // highlight when error
        {
           m_t_value->setBackgroundColor(Qt::red);
        }
    }
}
//--------------------------------------------------------------------------------------------
void SignalsTree::itemDoubleClicked(QTableWidgetItem *item) //-V2009
{
    if(m_t_value == item) connect(m_table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(itemChanged(QTableWidgetItem*)));
}
//--------------------------------------------------------------------------------------------
void SignalsTree::processSignalValueChanged(EDSignal *signal) //-V2009
{
    if(signal == m_sig_selected)
    {
        m_t_value->setText(EDSignal::SignalValueToString(*signal, false));
    }
}
//--------------------------------------------------------------------------------------------
void SignalsTree::TreeItemDoubleClicked(QTreeWidgetItem *item, int column) //-V2009
{
    (void)column;

    // abort adding to telemetry if item is node
    int signal_id = item->data(0, Qt::UserRole).toInt();
    if(m_ed->Signals().at(signal_id).isParent()) return;

    m_checkbox_intelemetry->clicked(!m_checkbox_intelemetry->isChecked());
    m_checkbox_intelemetry->setChecked(!m_checkbox_intelemetry->isChecked());
}
//--------------------------------------------------------------------------------------------
void SignalsTree::clicked_fix(bool checked)
{
    if(m_sig_selected != nullptr) m_ed->Signals().FixSignalValue(*m_sig_selected, checked);
}
//--------------------------------------------------------------------------------------------
void SignalsTree::clicked_intelemetry(bool checked)
{
	if (m_sig_selected == nullptr)
		return;
    QString tmp = (checked)?" adding to telemetry":" removing from telemetry";
    logger().WriteLn("Signal " + m_sig_selected->getDescriptor().name + tmp, Qt::gray);

    m_ed->Signals().setSignalTelMode(*m_sig_selected, checked);
}
//--------------------------------------------------------------------------------------------
void SignalsTree::save()
{
    QFileDialog dialog;
    QMap<QString, EDSignal> map;

    if(!m_sig_selected->isParent())
    {
        QMessageBox::critical(this, "Save", "Node to save not selected");
        return;
    }

    QString filtr = "*.sig";
    dialog.setFileMode(QFileDialog::ExistingFile);
    QString filename = dialog.getSaveFileName(this, "Save File", "d:/", "Signals data files (*.sig)", &filtr);
    if(filename.isEmpty()) return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);

    m_ed->Signals().FillMapWithChilds(map, *m_sig_selected);

    out << map;

    file.close();

    // show saved information
    logger().WriteLn("Saving...", Qt::blue);
    int size = map.size();
    for(int i = 0; i < size; i++)
    {
        logger().WriteLn("    " + map.keys().at(i) + ": " +
                         EDSignal::SignalValueToString(map.values().at(i), false), Qt::gray);
    }

    logger().WriteLn("Signals data saved to file: " + filename, Qt::green);
}
//--------------------------------------------------------------------------------------------
void SignalsTree::read()
{
    QFileDialog dialog;
    QMap<QString, EDSignal> map;

    QString filtr = "*.sig";

    dialog.setFileMode(QFileDialog::ExistingFile);
    QString filename = dialog.getOpenFileName(this, "Open File", "d:/", "Signals data files (*.sig)", &filtr);
    if(filename.isEmpty()) return;

    if(QFile::exists(filename))
    {
        QFile file(filename);
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);

        in >> map;

        file.close();

        // verify signals from map
        QList<QString> tmp_str = map.keys();
        if( !m_ed->Signals().VerifySignalNames(tmp_str) )
        {
            QMessageBox::critical(this, "Read", "Found errors while parsing data in file");
            return;
        }

        // send signals values to device
        QList<EDSignal> tmp_sig = map.values();
        m_ed->Signals().setSignalValueList(tmp_sig);

        // show updated signals
        int size = map.size();
        logger().WriteLn("Reading...", Qt::blue);
        for(int i = 0; i < size; i++)
        {
            logger().WriteLn("    " + map.keys().at(i) + ": " + EDSignal::SignalValueToString(map.values().at(i), false), Qt::gray);
        }
    } else
    {
        logger().WriteLn("File not found", Qt::red);
    }
}


