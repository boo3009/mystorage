#include "mainWindow.h"

void MainWindow::setup_itemsModelView() {
//------------------------------create "Model" for items table
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	itemsModel=new QSqlTableModel(this,retrieveDB);
	itemsModel->setTable("items");
  itemsModel->select();
  if(!itemsModel->select())
    qDebug()<<"Selecting not working in items";
  itemsModel->setHeaderData(0, Qt::Horizontal, "id");
  itemsModel->setHeaderData(1, Qt::Horizontal, "List of items");
  itemsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	itemsModel->setSort(0,Qt::AscendingOrder);
//------------------------------create "View" for items table
  itemsView=new QTableView();
	itemsView->setModel(itemsModel);
  itemsView->sortByColumn(0,Qt::AscendingOrder); /*finded out that view needed sort too*/
	itemsView->setSelectionMode(QAbstractItemView::SingleSelection);
  itemsView->setSelectionBehavior(QAbstractItemView::SelectRows);
  itemsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  itemsView->setColumnHidden(0,true);
  itemsView->verticalHeader()->setVisible(false);
//------------------------------some design tweaks for "itemsView" 
  itemsView->setStyleSheet(
    "QTableView {"
			"gridline-color: #ffcab0;  border: 2px solid #ffcab0;"
    	"background: #fdffcd;  selection-background-color: #f76b8a;}"
    "QTableView::item { padding: 5px;}"); 
	itemsView_header=itemsView->horizontalHeader();
  itemsView_header->setStretchLastSection(true);
  itemsView_header->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  itemsView_header->setStyleSheet(
    "QHeaderView::section { background-color: #f95959;}");
  QFont itemsView_headerFont("Colibri",10,QFont::Bold);
  itemsView_header->setFont(itemsView_headerFont);
//------------------------------setup "Widget" for "itemsView" 
  itemsModelView_widget=new QWidget();
  itemsModelView_widget_mainLayout=new QHBoxLayout(itemsModelView_widget);
  itemsModelView_widget_buttonsLayout=new QVBoxLayout();
  itemsModelView_widget_addItemPB=new QPushButton("Add item");
  itemsModelView_widget_editItemPB=new QPushButton("Edit item");
	itemsModelView_widget_copyItemPB=new QPushButton("Copy item");
  itemsModelView_widget_removeItemPB=new QPushButton("Remove item");
	
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
  
	itemsModelView_widget_mainLayout->addWidget(itemsView);
  itemsModelView_widget_buttonsLayout->addSpacing(22);
  itemsModelView_widget_buttonsLayout->addWidget(itemsModelView_widget_addItemPB);
  itemsModelView_widget_buttonsLayout->addWidget(itemsModelView_widget_editItemPB);
  itemsModelView_widget_buttonsLayout->addWidget(itemsModelView_widget_copyItemPB);
  itemsModelView_widget_buttonsLayout->addWidget(itemsModelView_widget_removeItemPB);
  itemsModelView_widget_buttonsLayout->addStretch();
  itemsModelView_widget_mainLayout->addLayout(itemsModelView_widget_buttonsLayout);
}

void MainWindow::setup_incomeModelView() {
//------------------------------create "Model" for income table
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	incomeModel=new QSqlTableModel(this,retrieveDB);
	incomeModel->setTable("income");
  incomeModel->select();
  if(!incomeModel->select())
    qDebug()<<"Selecting not working in income";
  incomeModel->setHeaderData(0, Qt::Horizontal, "id");
  incomeModel->setHeaderData(1, Qt::Horizontal, "Date");
  incomeModel->setHeaderData(2, Qt::Horizontal, "Operation");
  incomeModel->setHeaderData(3, Qt::Horizontal, "Quantity");
  incomeModel->setHeaderData(4, Qt::Horizontal, "Note");
  incomeModel->setHeaderData(5, Qt::Horizontal, "Item");
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
//------------------------------some design tweaks for "incomeView" 
  incomeView->setStyleSheet(
    "QTableView {"
			"gridline-color: #ffcab0;  border: 2px solid #ffcab0;"
    	"background: #fdffcd;  selection-background-color: #f76b8a;}"
    "QTableView::item { padding: 5px;}"); 
	incomeView_header=incomeView->horizontalHeader();
  incomeView_header->setStretchLastSection(true);
  incomeView_header->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  incomeView_header->setStyleSheet(
    "QHeaderView::section { background-color: #f95959;}");
  QFont incomeView_headerFont("Colibri",10,QFont::Bold);
  incomeView_header->setFont(incomeView_headerFont);
//------------------------------setup "Widget" for "incomeView" 
  incomeModelView_widget=new QWidget();
  incomeModelView_widget_mainLayout=new QHBoxLayout(incomeModelView_widget);
  incomeModelView_widget_buttonsLayout=new QVBoxLayout();
  incomeModelView_widget_addIncomePB=new QPushButton("Add income");
  incomeModelView_widget_editIncomePB=new QPushButton("Edit income");
	incomeModelView_widget_copyIncomePB=new QPushButton("Copy income");
  incomeModelView_widget_removeIncomePB=new QPushButton("Remove income");

	incomeModelView_widget->setFixedSize(900,800);
	int x=130;
	int y=27;
  incomeModelView_widget_addIncomePB->setFixedSize(x,y);
  incomeModelView_widget_editIncomePB->setFixedSize(x,y);
	incomeModelView_widget_copyIncomePB->setFixedSize(x,y);
  incomeModelView_widget_removeIncomePB->setFixedSize(x,y);
	incomeModelView_widget_addIncomePB->setStyleSheet("text-align:left;");
	incomeModelView_widget_editIncomePB->setStyleSheet("text-align:left;");
	incomeModelView_widget_copyIncomePB->setStyleSheet("text-align:left;");
	incomeModelView_widget_removeIncomePB->setStyleSheet("text-align:left;");
  
	incomeModelView_widget_mainLayout->addWidget(incomeView);
  incomeModelView_widget_buttonsLayout->addSpacing(22);
  incomeModelView_widget_buttonsLayout->addWidget(incomeModelView_widget_addIncomePB);
  incomeModelView_widget_buttonsLayout->addWidget(incomeModelView_widget_editIncomePB);
  incomeModelView_widget_buttonsLayout->addWidget(incomeModelView_widget_copyIncomePB);
  incomeModelView_widget_buttonsLayout->addWidget(incomeModelView_widget_removeIncomePB);
  incomeModelView_widget_buttonsLayout->addStretch();
  incomeModelView_widget_mainLayout->addLayout(incomeModelView_widget_buttonsLayout);
}

void MainWindow::setup_outcomeModelView() {
//------------------------------create "Model" for income table
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	outcomeModel=new QSqlTableModel(this,retrieveDB);
	outcomeModel->setTable("outcome");
  outcomeModel->select();
  if(!outcomeModel->select())
    qDebug()<<"Selecting not working in outcome";
  outcomeModel->setHeaderData(0, Qt::Horizontal, "id");
  outcomeModel->setHeaderData(1, Qt::Horizontal, "Date");
  outcomeModel->setHeaderData(2, Qt::Horizontal, "Operation");
  outcomeModel->setHeaderData(3, Qt::Horizontal, "Quantity");
  outcomeModel->setHeaderData(4, Qt::Horizontal, "Comment");
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
//------------------------------some design tweaks for "outcomeView" 
  outcomeView->setStyleSheet(
    "QTableView {"
			"gridline-color: #ffcab0;  border: 2px solid #ffcab0;"
    	"background: #fdffcd;  selection-background-color: #f76b8a;}"
    "QTableView::item { padding: 5px;}"); 
	outcomeView_header=outcomeView->horizontalHeader();
  outcomeView_header->setStretchLastSection(true);
  outcomeView_header->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  outcomeView_header->setStyleSheet(
    "QHeaderView::section { background-color: #f95959;}");
  QFont outcomeView_headerFont("Colibri",10,QFont::Bold);
  outcomeView_header->setFont(outcomeView_headerFont);
//------------------------------setup "Widget" for "outcomeView" 
  outcomeModelView_widget=new QWidget();
  outcomeModelView_widget_mainLayout=new QHBoxLayout(outcomeModelView_widget);
  outcomeModelView_widget_buttonsLayout=new QVBoxLayout();
  outcomeModelView_widget_addOutcomePB=new QPushButton("Add outcome");
  outcomeModelView_widget_editOutcomePB=new QPushButton("Edit outcome");
	outcomeModelView_widget_copyOutcomePB=new QPushButton("Copy outcome");
  outcomeModelView_widget_removeOutcomePB=new QPushButton("Remove outcome");

	outcomeModelView_widget->setFixedSize(900,800);
	int x=130;
	int y=27;
  outcomeModelView_widget_addOutcomePB->setFixedSize(x,y);
  outcomeModelView_widget_editOutcomePB->setFixedSize(x,y);
	outcomeModelView_widget_copyOutcomePB->setFixedSize(x,y);
  outcomeModelView_widget_removeOutcomePB->setFixedSize(x,y);
	outcomeModelView_widget_addOutcomePB->setStyleSheet("text-align:left;");
	outcomeModelView_widget_editOutcomePB->setStyleSheet("text-align:left;");
	outcomeModelView_widget_copyOutcomePB->setStyleSheet("text-align:left;");
	outcomeModelView_widget_removeOutcomePB->setStyleSheet("text-align:left;");
  
	outcomeModelView_widget_mainLayout->addWidget(outcomeView);
  outcomeModelView_widget_buttonsLayout->addSpacing(22);
  outcomeModelView_widget_buttonsLayout->addWidget(outcomeModelView_widget_addOutcomePB);
  outcomeModelView_widget_buttonsLayout->addWidget(outcomeModelView_widget_editOutcomePB);
  outcomeModelView_widget_buttonsLayout->addWidget(outcomeModelView_widget_copyOutcomePB);
  outcomeModelView_widget_buttonsLayout->addWidget(outcomeModelView_widget_removeOutcomePB);
  outcomeModelView_widget_buttonsLayout->addStretch();
  outcomeModelView_widget_mainLayout->addLayout(outcomeModelView_widget_buttonsLayout);
}

void MainWindow::setup_balanceModelView() {
//------------------------------create "Model" for balance table
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	balanceModel=new QSqlTableModel(this,retrieveDB);
	balanceModel->setTable("balance");
  balanceModel->select();
  if(!balanceModel->select())
    qDebug()<<"Selecting not working in balance";
  balanceModel->setHeaderData(0, Qt::Horizontal, "id");
  balanceModel->setHeaderData(1, Qt::Horizontal, "Cell");
  balanceModel->setHeaderData(2, Qt::Horizontal, "Item");
  balanceModel->setHeaderData(3, Qt::Horizontal, "Quantity");
  balanceModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	balanceModel->setSort(0,Qt::AscendingOrder);
//------------------------------create "View" for balance table
  balanceView=new QTableView();
	balanceView->setModel(balanceModel);
  balanceView->sortByColumn(0,Qt::AscendingOrder); /*finded out that view needed sort too*/
	balanceView->setSelectionMode(QAbstractItemView::SingleSelection);
  balanceView->setSelectionBehavior(QAbstractItemView::SelectRows);
  balanceView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  balanceView->setColumnHidden(0,true);
  balanceView->verticalHeader()->setVisible(false);
//------------------------------some design tweaks for "balanceView" 
  balanceView->setStyleSheet(
    "QTableView {"
			"gridline-color: #ffcab0;  border: 2px solid #ffcab0;"
    	"background: #fdffcd;  selection-background-color: #f76b8a;}"
    "QTableView::item { padding: 5px;}"); 
	balanceView_header=balanceView->horizontalHeader();
  balanceView_header->setStretchLastSection(true);
  balanceView_header->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  balanceView_header->setStyleSheet(
    "QHeaderView::section { background-color: #f95959;}");
  QFont balanceView_headerFont("Colibri",10,QFont::Bold);
  balanceView_header->setFont(balanceView_headerFont);
//------------------------------setup "Widget" for "balanceView" 
  balanceModelView_widget=new QWidget();
	balanceModelView_widget->setFixedSize(900,800);
  balanceModelView_widget_mainLayout=new QHBoxLayout(balanceModelView_widget);
	balanceModelView_widget_mainLayout->addWidget(balanceView);
}
