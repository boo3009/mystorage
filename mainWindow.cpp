#include "mainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("My Storage");
  setFixedSize(1400,900);
  
	connect_Database();
  setup_ModelViews();
  setup_CoreWidgets();
  setup_SignalSlots();
//-----??????????????????????????? h_main_layout needs to be corrected 
	mainWidget->setLayout(mainLayout);
}

void MainWindow::connect_Database() {
	dbPointer=new Database();
	dbPointer->connectDatabase();
}

void MainWindow::setup_ModelViews() {
	setup_itemsModelView();
//	setup_incomeModelView();
//	setup_outcomeModelView();
//	setup_balanceModelView();
}

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
//------------------------------create "View" for items table
  itemsView=new QTableView();
  itemsView->setModel(itemsModel);
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
  QFont itemsView_headerFont("Colibri",15,QFont::Bold);
  itemsView_header->setFont(itemsView_headerFont);
//------------------------------setup "Widget" for "itemsView" 
  itemsModelView_widget=new QWidget();
  itemsModelView_widget_mainLayout=new QHBoxLayout(itemsModelView_widget);
  itemsModelView_widget_buttonsLayout=new QVBoxLayout();
  itemsModelView_widget_addItemPB=new QPushButton("Add item");
  itemsModelView_widget_editItemPB=new QPushButton("Edit item");
	itemsModelView_widget_copyItemPB=new QPushButton("Copy item");
  itemsModelView_widget_removeItemPB=new QPushButton("Remove item");
  itemsModelView_widget_mainLayout->addWidget(itemsView);
  itemsModelView_widget_buttonsLayout->addSpacing(22);
  itemsModelView_widget_buttonsLayout->addWidget(itemsModelView_widget_addItemPB);
  itemsModelView_widget_buttonsLayout->addWidget(itemsModelView_widget_editItemPB);
  itemsModelView_widget_buttonsLayout->addWidget(itemsModelView_widget_copyItemPB);
  itemsModelView_widget_buttonsLayout->addWidget(itemsModelView_widget_removeItemPB);
  itemsModelView_widget_buttonsLayout->addStretch();
  itemsModelView_widget_mainLayout->addLayout(itemsModelView_widget_buttonsLayout);
//	connect(itemsView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slotEditRecord(QModelIndex)));
}

void MainWindow::setup_CoreWidgets() {
  mainWidget=new QWidget();
  setCentralWidget(mainWidget);

  mainLayout_left=new QVBoxLayout();
  mainLayout_left_incomePB=new QPushButton("Create income");
  mainLayout_left_outcomePB=new QPushButton("Create outcome");
  mainLayout_left_itemsPB=new QPushButton("List of Items");
  mainLayout_left_balancePB=new QPushButton("Carry-over");
  mainLayout_left_quitPB=new QPushButton("Quit My Storage");
    mainLayout_left_incomePB->setStyleSheet("text-align:left;");
    mainLayout_left_outcomePB->setStyleSheet("text-align:left;");
    mainLayout_left_itemsPB->setStyleSheet("text-align:left;");
    mainLayout_left_balancePB->setStyleSheet("text-align:left;");
    mainLayout_left_quitPB->setStyleSheet("text-align:left;");
  mainLayout_left->addSpacing(50);
  mainLayout_left->addWidget(mainLayout_left_incomePB);
  mainLayout_left->addWidget(mainLayout_left_outcomePB);
  mainLayout_left->addWidget(mainLayout_left_itemsPB);
  mainLayout_left->addWidget(mainLayout_left_balancePB);
  mainLayout_left->addSpacing(50);
  mainLayout_left->addWidget(mainLayout_left_quitPB);
  mainLayout_left->addStretch();
	
	mainLayout_middle=new QVBoxLayout();
	mainLayout_middle->addSpacing(40);
	mainLayout_middle_stack=new QStackedWidget();
	mainLayout_middle_stack->addWidget(itemsModelView_widget);
	mainLayout_middle->addWidget(mainLayout_middle_stack);

	mainLayout=new QHBoxLayout();
	mainLayout->addLayout(mainLayout_left);
	mainLayout->addSpacing(50);
	mainLayout->addLayout(mainLayout_middle);
	mainLayout->addStretch();
}

void MainWindow::setup_SignalSlots() {
	connect(mainLayout_left_quitPB,&QPushButton::clicked,this,&QApplication::quit);
	connect(mainLayout_left_itemsPB,&QPushButton::clicked,this,&MainWindow::show_itemsList);
}

void MainWindow::show_itemsList() {
	this->mainLayout_middle_stack->setCurrentIndex(0);
}
