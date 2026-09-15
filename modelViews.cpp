#include "mainWindow.h"
#include "translations.h"

void MainWindow::setup_itemsModelView() {
//------------------------------create "Model" for items table
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	itemsModel=new QSqlTableModel(this,retrieveDB);
	itemsModel->setTable("items");
  itemsModel->select();
  if(!itemsModel->select())
    qDebug()<<"Selecting not working in items";
  itemsModel->setHeaderData(0, Qt::Horizontal, ID_RU);
  itemsModel->setHeaderData(1, Qt::Horizontal,LIST_ITEM_RU);
  itemsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	itemsModel->setSort(0,Qt::AscendingOrder);
//------------------------------create proxy model for "itemsModel"
	items_proxymodel=new QSortFilterProxyModel(this);
	items_proxymodel->setSourceModel(itemsModel);
	items_proxymodel->setFilterKeyColumn(itemsModel->fieldIndex("item_name"));
	items_proxymodel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	items_proxymodel->setDynamicSortFilter(true);
//------------------------------create "View" for proxy model
  itemsView=new QTableView();
	itemsView->setModel(items_proxymodel);
	itemsView->setSelectionMode(QAbstractItemView::SingleSelection);
  itemsView->setSelectionBehavior(QAbstractItemView::SelectRows);
  itemsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  itemsView->setColumnHidden(0,true);
  itemsView->verticalHeader()->setVisible(false);

	items_filter_label=new QLabel(SEARCH_ITEM_RU);
	items_filter_lineedit=new QLineEdit();
	clear_filterPB=new QPushButton(CLEAR_FILTER_RU);
	items_filter_label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	items_filter_lineedit->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	clear_filterPB->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	connect(items_filter_lineedit,&QLineEdit::textChanged,this,&MainWindow::slot_set_items_filter);
	connect(clear_filterPB,&QPushButton::clicked,this,&MainWindow::slot_clear_items_filter);
//------------------------------some design tweaks for "itemsView" 
  itemsView->setStyleSheet(
    "QTableView {"
			"gridline-color: #7F8CAA;  border: 2px solid #7F8CAA;"
    	"background: #E8F9FF;  selection-background-color: #6D94C5;}"
    "QTableView::item { padding: 5px;}"); 
	itemsView_header=itemsView->horizontalHeader();
  itemsView_header->setStretchLastSection(true);
  itemsView_header->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  itemsView_header->setStyleSheet(
    "QHeaderView::section { background-color: #0F828C;}");
  QFont itemsView_headerFont("Colibri",10,QFont::Bold);
  itemsView_header->setFont(itemsView_headerFont);
	itemsView_header->setSectionResizeMode(QHeaderView::ResizeToContents);
//------------------------------setup "Widget" for "itemsView" 
  itemsModelView_widget=new QWidget();
  itemsModelView_widget_mainLayout=new QVBoxLayout(itemsModelView_widget);
	items_layout=new QHBoxLayout();
  itemsModelView_widget_buttonsLayout=new QVBoxLayout();
  itemsModelView_widget_addItemPB=new QPushButton(ADD_ITEM_RU);
  itemsModelView_widget_editItemPB=new QPushButton(EDIT_ITEM_RU);
	itemsModelView_widget_copyItemPB=new QPushButton(COPY_ITEM_RU);
  itemsModelView_widget_removeItemPB=new QPushButton(REMOVE_ITEM_RU);
	
	items_filter_layout=new QHBoxLayout();
	items_filter_layout->addWidget(items_filter_label);
	items_filter_layout->addWidget(items_filter_lineedit);
	items_filter_layout->addWidget(clear_filterPB);

	itemsModelView_widget->setFixedSize(900,800);
	int x=130;
	int y=27;
  itemsModelView_widget_addItemPB->setFixedSize(x,y);
  itemsModelView_widget_editItemPB->setFixedSize(x,y);
	itemsModelView_widget_copyItemPB->setFixedSize(x,y);
  itemsModelView_widget_removeItemPB->setFixedSize(x,y);
	itemsModelView_widget_addItemPB->setStyleSheet("text-align:left;");
	itemsModelView_widget_editItemPB->setStyleSheet("text-align:left;");
	itemsModelView_widget_copyItemPB->setStyleSheet("text-align:left;");
	itemsModelView_widget_removeItemPB->setStyleSheet("text-align:left;");
  
  itemsModelView_widget_mainLayout->addLayout(items_filter_layout);
	items_filter_layout->addStretch();
	items_layout->addWidget(itemsView);
  itemsModelView_widget_buttonsLayout->addSpacing(22);
  itemsModelView_widget_buttonsLayout->addWidget(itemsModelView_widget_addItemPB);
  itemsModelView_widget_buttonsLayout->addWidget(itemsModelView_widget_editItemPB);
  itemsModelView_widget_buttonsLayout->addWidget(itemsModelView_widget_copyItemPB);
  itemsModelView_widget_buttonsLayout->addWidget(itemsModelView_widget_removeItemPB);
  itemsModelView_widget_buttonsLayout->addStretch();
  items_layout->addLayout(itemsModelView_widget_buttonsLayout);
	itemsModelView_widget_mainLayout->addLayout(items_layout);
}

void MainWindow::setup_incomeModelView() {
//------------------------------create "Model" for income table
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	incomeModel=new QSqlTableModel(this,retrieveDB);
	incomeModel->setTable("income");
  incomeModel->select();
  if(!incomeModel->select())
    qDebug()<<"Selecting not working in income";
  incomeModel->setHeaderData(0, Qt::Horizontal, ID_RU);
  incomeModel->setHeaderData(1, Qt::Horizontal, NUM_RU);
  incomeModel->setHeaderData(2, Qt::Horizontal, DATE_RU);
  incomeModel->setHeaderData(3, Qt::Horizontal, OP_TYPE_RU);
  incomeModel->setHeaderData(4, Qt::Horizontal, STATUS_RU);
  incomeModel->setHeaderData(5, Qt::Horizontal, SUM_RU);
  incomeModel->setHeaderData(6, Qt::Horizontal, NOTE_RU);
  incomeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	incomeModel->setSort(0,Qt::AscendingOrder);
//------------------------------create "View" for items table
  incomeView=new QTableView();
	incomeView->setModel(incomeModel);
  incomeView->sortByColumn(0,Qt::AscendingOrder); /*finded out that view needed sort too*/
	incomeView->setSelectionMode(QAbstractItemView::SingleSelection);
  incomeView->setSelectionBehavior(QAbstractItemView::SelectRows);
  incomeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  incomeView->setColumnHidden(0,true);
  incomeView->verticalHeader()->setVisible(false);
	incomeView->setColumnWidth(1,70);
	incomeView->setColumnWidth(2,100);
	incomeView->setColumnWidth(3,140);
	incomeView->setColumnWidth(4,90);
	incomeView->setColumnWidth(5,60);
	incomeView->setItemDelegateForColumn(2,new date_delegate(incomeView));
//------------------------------some design tweaks for "incomeView" 
  incomeView->setStyleSheet(
    "QTableView {"
			"gridline-color: #A1BC98;  border: 2px solid #A1BC98;"
    	"background: #CBF3BB;  selection-background-color: #41A67E;}"
    "QTableView::item { padding: 5px;}"); 
	incomeView_header=incomeView->horizontalHeader();
  incomeView_header->setStretchLastSection(true);
  incomeView_header->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  incomeView_header->setStyleSheet(
    "QHeaderView::section { background-color: #5D866C;}");
  QFont incomeView_headerFont("Colibri",10,QFont::Bold);
  incomeView_header->setFont(incomeView_headerFont);
//------------------------------setup "Widget" for "incomeView" 
  incomeModelView_widget=new QWidget();
  incomeModelView_widget_mainLayout=new QHBoxLayout(incomeModelView_widget);
  incomeModelView_widget_buttonsLayout=new QVBoxLayout();
  incomeModelView_widget_addIncomePB=new QPushButton(ADD_INCOME_RU);
  incomeModelView_widget_editIncomePB=new QPushButton(EDIT_INCOME_RU);
  incomeModelView_widget_removeIncomePB=new QPushButton(REMOVE_INCOME_RU);
  incomeModelView_widget_cancel_removeIncomePB=new QPushButton(CANCEL_REMOVING_RU);

	incomeModelView_widget->setFixedSize(900,800);
	int x=130;
	int y=27;
  incomeModelView_widget_addIncomePB->setFixedSize(x,y);
  incomeModelView_widget_editIncomePB->setFixedSize(x,y);
  incomeModelView_widget_removeIncomePB->setFixedSize(x,y);
  incomeModelView_widget_cancel_removeIncomePB->setFixedSize(x,y);
	incomeModelView_widget_addIncomePB->setStyleSheet("text-align:left;");
	incomeModelView_widget_editIncomePB->setStyleSheet("text-align:left;");
	incomeModelView_widget_removeIncomePB->setStyleSheet("text-align:left;");
	incomeModelView_widget_cancel_removeIncomePB->setStyleSheet("text-align:left;");
  
	incomeModelView_widget_mainLayout->addWidget(incomeView);
  incomeModelView_widget_buttonsLayout->addSpacing(22);
  incomeModelView_widget_buttonsLayout->addWidget(incomeModelView_widget_addIncomePB);
  incomeModelView_widget_buttonsLayout->addWidget(incomeModelView_widget_editIncomePB);
  incomeModelView_widget_buttonsLayout->addWidget(incomeModelView_widget_removeIncomePB);
  incomeModelView_widget_buttonsLayout->addWidget(incomeModelView_widget_cancel_removeIncomePB);
  incomeModelView_widget_buttonsLayout->addStretch();
  incomeModelView_widget_mainLayout->addLayout(incomeModelView_widget_buttonsLayout);
}

void MainWindow::setup_outcomeModelView() {
//------------------------------create "Model" for outcome table
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	outcomeModel=new QSqlTableModel(this,retrieveDB);
	outcomeModel->setTable("outcome");
  outcomeModel->select();
  if(!outcomeModel->select())
    qDebug()<<"Selecting not working in outcome";
  outcomeModel->setHeaderData(0, Qt::Horizontal, ID_RU);
  outcomeModel->setHeaderData(1, Qt::Horizontal, NUM_RU);
  outcomeModel->setHeaderData(2, Qt::Horizontal, DATE_RU);
  outcomeModel->setHeaderData(3, Qt::Horizontal, OP_TYPE_RU);
  outcomeModel->setHeaderData(4, Qt::Horizontal, STATUS_RU);
  outcomeModel->setHeaderData(5, Qt::Horizontal, SUM_RU);
  outcomeModel->setHeaderData(6, Qt::Horizontal, NOTE_RU);
  outcomeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	outcomeModel->setSort(0,Qt::AscendingOrder);
//------------------------------create "View" for items table
  outcomeView=new QTableView();
	outcomeView->setModel(outcomeModel);
  outcomeView->sortByColumn(0,Qt::AscendingOrder); /*finded out that view needed sort too*/
	outcomeView->setSelectionMode(QAbstractItemView::SingleSelection);
  outcomeView->setSelectionBehavior(QAbstractItemView::SelectRows);
  outcomeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  outcomeView->setColumnHidden(0,true);
  outcomeView->verticalHeader()->setVisible(false);
	outcomeView->setColumnWidth(1,70);
	outcomeView->setColumnWidth(2,100);
	outcomeView->setColumnWidth(3,140);
	outcomeView->setColumnWidth(4,90);
	outcomeView->setColumnWidth(5,60);
	outcomeView->setItemDelegateForColumn(2,new date_delegate(outcomeView));
//------------------------------some design tweaks for "outcomeView" 
  outcomeView->setStyleSheet(
    "QTableView {"
			"gridline-color: #F5BABB;  border: 2px solid #F5BABB;"
    	"background: #FFEAEA;  selection-background-color: #FD7979;}"
    "QTableView::item { padding: 5px;}"); 
	outcomeView_header=outcomeView->horizontalHeader();
  outcomeView_header->setStretchLastSection(true);
  outcomeView_header->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  outcomeView_header->setStyleSheet(
    "QHeaderView::section { background-color: #D34E4E;}");
  QFont outcomeView_headerFont("Colibri",10,QFont::Bold);
  outcomeView_header->setFont(outcomeView_headerFont);
//------------------------------setup "Widget" for "outcomeView" 
  outcomeModelView_widget=new QWidget();
  outcomeModelView_widget_mainLayout=new QHBoxLayout(outcomeModelView_widget);
  outcomeModelView_widget_buttonsLayout=new QVBoxLayout();
  outcomeModelView_widget_addOutcomePB=new QPushButton(ADD_OUTCOME_RU);
  outcomeModelView_widget_editOutcomePB=new QPushButton(EDIT_OUTCOME_RU);
  outcomeModelView_widget_removeOutcomePB=new QPushButton(REMOVE_OUTCOME_RU);
  outcomeModelView_widget_cancel_removeOutcomePB=new QPushButton(CANCEL_REMOVING_RU);

	outcomeModelView_widget->setFixedSize(900,800);
	int x=130;
	int y=27;
  outcomeModelView_widget_addOutcomePB->setFixedSize(x,y);
  outcomeModelView_widget_editOutcomePB->setFixedSize(x,y);
  outcomeModelView_widget_removeOutcomePB->setFixedSize(x,y);
  outcomeModelView_widget_cancel_removeOutcomePB->setFixedSize(x,y);
	outcomeModelView_widget_addOutcomePB->setStyleSheet("text-align:left;");
	outcomeModelView_widget_editOutcomePB->setStyleSheet("text-align:left;");
	outcomeModelView_widget_removeOutcomePB->setStyleSheet("text-align:left;");
	outcomeModelView_widget_cancel_removeOutcomePB->setStyleSheet("text-align:left;");
  
	outcomeModelView_widget_mainLayout->addWidget(outcomeView);
  outcomeModelView_widget_buttonsLayout->addSpacing(22);
  outcomeModelView_widget_buttonsLayout->addWidget(outcomeModelView_widget_addOutcomePB);
  outcomeModelView_widget_buttonsLayout->addWidget(outcomeModelView_widget_editOutcomePB);
  outcomeModelView_widget_buttonsLayout->addWidget(outcomeModelView_widget_removeOutcomePB);
  outcomeModelView_widget_buttonsLayout->addWidget(outcomeModelView_widget_cancel_removeOutcomePB);
  outcomeModelView_widget_buttonsLayout->addStretch();
  outcomeModelView_widget_mainLayout->addLayout(outcomeModelView_widget_buttonsLayout);
}

void MainWindow::setup_balanceModelView() {
//------------------------------create "Model" for balance table
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	balanceModel=new QSqlTableModel(this,retrieveDB);
	balanceModel->setTable("filled_cells");
  balanceModel->select();
  if(!balanceModel->select())
    qDebug()<<"Selecting not working in filled_cells";
  balanceModel->setHeaderData(0, Qt::Horizontal, ID_RU);
  balanceModel->setHeaderData(1, Qt::Horizontal, CELL_RU);
  balanceModel->setHeaderData(2, Qt::Horizontal, ITEM_RU);
  balanceModel->setHeaderData(3, Qt::Horizontal, QUANTITY_RU);
  balanceModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	balanceModel->setSort(1,Qt::AscendingOrder);
//------------------------------create "Model" for search	
//	search_model=new QSqlQueryModel(this);
  search_model=new query_model_class(this);
	search_model->setHeaderData(0, Qt::Horizontal, ID_RU);
  search_model->setHeaderData(1, Qt::Horizontal, DATE_RU);
  search_model->setHeaderData(2, Qt::Horizontal, NUM_RU);
  search_model->setHeaderData(3, Qt::Horizontal, OP_TYPE_RU);
  search_model->setHeaderData(4, Qt::Horizontal, STATUS_RU);
  search_model->setHeaderData(5, Qt::Horizontal, CELL_RU);
  search_model->setHeaderData(6, Qt::Horizontal, ITEM_RU);
  search_model->setHeaderData(7, Qt::Horizontal, QUANTITY_RU);
//------------------------------create "View" for balance table
  balanceView=new QTableView();
	balanceView->setModel(balanceModel);
  balanceView->sortByColumn(1,Qt::AscendingOrder); /*finded out that view needed sort too*/
	balanceView->setSelectionMode(QAbstractItemView::SingleSelection);
  balanceView->setSelectionBehavior(QAbstractItemView::SelectRows);
  balanceView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	balanceView->resizeColumnsToContents();
  balanceView->setColumnHidden(0,true);
  balanceView->verticalHeader()->setVisible(false);
  
	balanceView->setStyleSheet(
    "QTableView {"
			"gridline-color: #7F8CAA;  border: 2px solid #7F8CAA;"
    	"background: #E8F9FF;  selection-background-color: #6D94C5;}"
    "QTableView::item { padding: 5px;}"); 
	balanceView_header=balanceView->horizontalHeader();
  balanceView_header->setStretchLastSection(true);
  balanceView_header->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  balanceView_header->setStyleSheet(
    "QHeaderView::section { background-color: #0F828C;}");
  QFont balanceView_headerFont("Colibri",10,QFont::Bold);
  balanceView_header->setFont(balanceView_headerFont);
	balanceView_header->setSectionResizeMode(QHeaderView::ResizeToContents);
//------------------------------setup "Widget" for "balanceView" 
  balanceModelView_widget=new QWidget();
	balanceModelView_widget->setFixedSize(1000,800);
  balanceModelView_widget_mainLayout=new QVBoxLayout(balanceModelView_widget);

	balanceModelView_widget_buttons_layout=new QHBoxLayout();
	generate=new QPushButton(GENERATE_RU);
	generate->setStyleSheet("background-color: #8BAE66");
	generate->setFixedWidth(120);
	generate->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	date_filter=new QDateEdit();
	date_filter->setDate(QDate::currentDate());
	date_filter->setDisplayFormat("dd.MM.yyyy");
	date_filter->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	write_into_file=new QPushButton(WRITE_RU);
	write_into_file->setStyleSheet("background-color: #ED985F");
	write_into_file->setFixedWidth(130);
	write_into_file->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	pieces_label=new QLabel(PIECES_RU);
	pieces_lineedit=new QLineEdit();
	pieces_lineedit->setReadOnly(true);
	pieces_lineedit->setFixedWidth(70);
	pieces_label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	pieces_lineedit->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	non_empty_cells_label=new QLabel(NON_EMPTY_RU);
	non_empty_cells_lineedit=new QLineEdit();
	non_empty_cells_lineedit->setReadOnly(true);
	non_empty_cells_lineedit->setFixedWidth(70);
	non_empty_cells_label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	non_empty_cells_lineedit->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	search_by_cell_PB=new QPushButton(SEARCH_BY_CELL_RU);
	search_by_cell_PB->setStyleSheet("background-color: #2F4F4F");
	search_by_cell_PB->setFixedWidth(120);
	search_by_cell_PB->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	search_by_item_PB=new QPushButton(SEARCH_BY_ITEM_RU);
	search_by_item_PB->setStyleSheet("background-color: #2F4F4F");
	search_by_item_PB->setFixedWidth(120);
	search_by_item_PB->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

	balanceModelView_widget_buttons_layout->addWidget(date_filter);
	balanceModelView_widget_buttons_layout->addWidget(generate);
	balanceModelView_widget_buttons_layout->addWidget(write_into_file);
	balanceModelView_widget_buttons_layout->addStretch(2);
	balanceModelView_widget_buttons_layout->addWidget(pieces_label);
	balanceModelView_widget_buttons_layout->addWidget(pieces_lineedit);
	balanceModelView_widget_buttons_layout->addWidget(non_empty_cells_label);
	balanceModelView_widget_buttons_layout->addWidget(non_empty_cells_lineedit);
	balanceModelView_widget_buttons_layout->addStretch(2);
	balanceModelView_widget_buttons_layout->addWidget(search_by_cell_PB);
	balanceModelView_widget_buttons_layout->addWidget(search_by_item_PB);

	balanceModelView_widget_mainLayout->addLayout(balanceModelView_widget_buttons_layout);
	balanceModelView_widget_mainLayout->addWidget(balanceView);

	QSizePolicy sp(QSizePolicy::Expanding,QSizePolicy::Expanding);
	sp.setRetainSizeWhenHidden(true);
	balanceView->setSizePolicy(sp);
	balanceView->setVisible(false);
}

void MainWindow::setup_operationsModelView() {
//------------------------------create "Model" for qtableview in operations widget
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	operationsModel=new QSqlTableModel(this,retrieveDB);
	operationsModel->setTable("operations");
  operationsModel->select();
  if(!operationsModel->select())
    qDebug()<<"Selecting not working in operations";
  operationsModel->setHeaderData(0, Qt::Horizontal, ID_RU);
  operationsModel->setHeaderData(1, Qt::Horizontal, DATE_RU);
  operationsModel->setHeaderData(2, Qt::Horizontal, NUM_RU);
  operationsModel->setHeaderData(3, Qt::Horizontal, OP_TYPE_RU);
  operationsModel->setHeaderData(4, Qt::Horizontal, STATUS_RU);
  operationsModel->setHeaderData(5, Qt::Horizontal, CELL_RU);
  operationsModel->setHeaderData(6, Qt::Horizontal, ITEM_RU);
  operationsModel->setHeaderData(7, Qt::Horizontal, QUANTITY_RU);
  operationsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	operationsModel->setSort(0,Qt::AscendingOrder);
}
