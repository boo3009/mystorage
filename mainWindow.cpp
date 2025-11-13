#include "mainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("My Storage");
  setFixedSize(1400,900);
//  this->showFullScreen();
	
	connect_Database();
  setup_ModelViews();
  setup_CoreWidgets();
  setup_SignalSlots();
	mainWidget->setLayout(mainLayout);
}

void MainWindow::connect_Database() {
	dbPointer=new Database();
	dbPointer->connectDatabase();
}

void MainWindow::setup_ModelViews() {
	setup_itemsModelView();
	setup_incomeModelView();
	setup_outcomeModelView();
	setup_balanceModelView();
	setup_operationsModelView();
}

void MainWindow::setup_CoreWidgets() {
  mainWidget=new QWidget();
  setCentralWidget(mainWidget);
//------------------------------setting "left" side of mainLayout
  mainLayout_left_incomePB=new QPushButton("List of incomes");
  mainLayout_left_outcomePB=new QPushButton("List of outcomes");
  mainLayout_left_itemsPB=new QPushButton("List of Items");
  mainLayout_left_balancePB=new QPushButton("Balance");
  mainLayout_left_quitPB=new QPushButton("Quit My Storage");
	mainLayout_left_incomePB->setStyleSheet("text-align:left;");
	mainLayout_left_outcomePB->setStyleSheet("text-align:left;");
	mainLayout_left_itemsPB->setStyleSheet("text-align:left;");
	mainLayout_left_balancePB->setStyleSheet("text-align:left;");
	mainLayout_left_quitPB->setStyleSheet("text-align:left;");
  mainLayout_left=new QVBoxLayout();
	mainLayout_left->addSpacing(50);
  mainLayout_left->addWidget(mainLayout_left_incomePB);
  mainLayout_left->addWidget(mainLayout_left_outcomePB);
  mainLayout_left->addWidget(mainLayout_left_itemsPB);
  mainLayout_left->addWidget(mainLayout_left_balancePB);
  mainLayout_left->addSpacing(50);
  mainLayout_left->addWidget(mainLayout_left_quitPB);
  mainLayout_left->addStretch();
//------------------------------setting "middle" side of mainLayout
	mainLayout_middle=new QVBoxLayout();
	mainLayout_middle->addSpacing(40);
	mainLayout_middle_stack=new QStackedWidget();
	mainLayout_middle_stack->addWidget(incomeModelView_widget);
	mainLayout_middle_stack->addWidget(outcomeModelView_widget);
	mainLayout_middle_stack->addWidget(itemsModelView_widget);
	mainLayout_middle_stack->addWidget(balanceModelView_widget);
	mainLayout_middle->addWidget(mainLayout_middle_stack);
//---------------------------------setting mainLayout of application
	mainLayout=new QHBoxLayout();
	mainLayout->addLayout(mainLayout_left);
	mainLayout->addSpacing(50);
	mainLayout->addLayout(mainLayout_middle);
	mainLayout->addStretch();

}

void MainWindow::setup_SignalSlots() {
//-------------------------mainLayouts left side PB's connects
	connect(mainLayout_left_quitPB,&QPushButton::clicked,this,&QApplication::quit);
	connect(mainLayout_left_incomePB,&QPushButton::clicked,
	/*----*/[=]() { this->mainLayout_middle_stack->setCurrentIndex(0); });
	connect(mainLayout_left_outcomePB,&QPushButton::clicked,
	/*----*/[=]() { this->mainLayout_middle_stack->setCurrentIndex(1); });
	connect(mainLayout_left_itemsPB,&QPushButton::clicked,
	/*----*/[=]() { this->mainLayout_middle_stack->setCurrentIndex(2); });
	connect(mainLayout_left_balancePB,&QPushButton::clicked,
	/*----*/[=]() { this->mainLayout_middle_stack->setCurrentIndex(3); });
//-------------------------itemsModelView connects
	connect(itemsModelView_widget_addItemPB,&QPushButton::clicked,
  /*----*/this,&MainWindow::slot_itemDialog_add);
	connect(itemsModelView_widget_editItemPB,&QPushButton::clicked,
	/*----*/this,&MainWindow::slot_itemDialog_edit);
	connect(itemsView,&QTableView::doubleClicked,this,&MainWindow::slot_editItemByDoubleClick);
	connect(itemsModelView_widget_copyItemPB,&QPushButton::clicked,
	/*----*/this,&MainWindow::slot_itemDialog_copy);
	connect(itemsModelView_widget_removeItemPB,&QPushButton::clicked,this,&MainWindow::slot_itemsModelView_remove);
//-------------------------incomesModelView connects
	connect(incomeModelView_widget_addIncomePB,&QPushButton::clicked,
  /*----*/this,&MainWindow::slot_incomeDialog_add);
	connect(incomeModelView_widget_editIncomePB,&QPushButton::clicked,
	/*----*/this,&MainWindow::slot_incomeDialog_edit);
	connect(incomeView,&QTableView::doubleClicked,this,&MainWindow::slot_editIncomeByDoubleClick);
	connect(incomeModelView_widget_removeIncomePB,&QPushButton::clicked,this,&MainWindow::slot_incomeModelView_remove);
	connect(incomeModelView_widget_cancel_removeIncomePB,&QPushButton::clicked,this,&MainWindow::slot_incomeModelView_cancel_remove);
	

//-------------------------balanceModelView connects
	connect(generate,&QPushButton::clicked,this,&MainWindow::slot_insert_update);
	connect(write_into_file,&QPushButton::clicked,this,&MainWindow::slot_write_balance_into_file);

}
