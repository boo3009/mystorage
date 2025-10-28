#include "incomeDialog.h"

IncomeDialog::IncomeDialog(QSqlTableModel *model,QTableView *view,QSqlTableModel *itemsModel,
  QSqlTableModel *opModel,int row,bool copy,QWidget *parent) : 
	QDialog(parent), ptr_incomesModel(model), ptr_incomesView(view), ptr_itemsModel(itemsModel), 
	ptr_operationsModel(opModel) {
//---body of constructor
  setup_Widget();
  setup_ModelandMapper();
  if(row==-1)
  	func_addIncome();
	else if(row!=-1 && !copy)
		func_editIncome(row);
	else if(copy)
		func_copyIncome(row);
}

void IncomeDialog::setup_Widget() {
  setWindowTitle("Creating new income");
  setFixedSize(500,800);
	setWindowFlags(Qt::FramelessWindowHint);
	this->setObjectName("borders_for_income_dialog");
	this->setStyleSheet("QWidget#borders_for_income_dialog {" 
												"background-color: #ABE7B2; color: black;"
												"border: 1px solid #427A76; }");

  mainLayout=new QVBoxLayout(this);
  buttonsLayout=new QHBoxLayout();
  operations_buttons_layout=new QHBoxLayout();
  
	op_number_label=new	QLabel("Operation Number");
	op_number=new QLineEdit();
	op_number->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	op_number->setReadOnly(true);

	date_label=new QLabel("Date");
	date=new QDateEdit();
	date->setDate(QDate::currentDate());
	date->setDisplayFormat("dd/MM/yyyy");
	date->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

	note_label=new QLabel("Note");
	note=new QLineEdit();

	submitPB=new QPushButton("Submit");
	submitPB->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//---------------------operations part-------------------
	operationsView=new QTableView(this);
	operationsView->setModel(ptr_operationsModel);
  operationsView->sortByColumn(0,Qt::AscendingOrder);
	operationsView->setSelectionMode(QAbstractItemView::SingleSelection);
  operationsView->setColumnHidden(0,true);
//  operationsView->setColumnHidden(1,true);
//  /*operationsView->setColumnHidden(2,true);*/
//  operationsView->setColumnHidden(3,true);
  operationsView->verticalHeader()->setVisible(false);
	operationsView_header=operationsView->horizontalHeader();
	operationsView_header->setStretchLastSection(true);

	operations_proxymodel=new QSortFilterProxyModel(this);
	operations_proxymodel->setSourceModel(ptr_operationsModel);
	operationsView->setModel(operations_proxymodel);
	operations_proxymodel->setFilterKeyColumn(2);

	operations_addPB=new QPushButton("Add");
	operations_copyPB=new QPushButton("Copy");
	operations_removePB=new QPushButton("Remove");
	operations_buttons_layout=new QHBoxLayout();
	operations_buttons_layout->addWidget(operations_addPB);
	operations_buttons_layout->addWidget(operations_copyPB);
	operations_buttons_layout->addWidget(operations_removePB);
//---------------------saving buttons-------------------
	save_incomePB=new QPushButton("Save");
  cancel_incomePB=new QPushButton("Cancel");
  buttonsLayout->addWidget(save_incomePB);
  buttonsLayout->addWidget(cancel_incomePB);
	
  mainLayout->addWidget(op_number_label);
  mainLayout->addWidget(op_number);
  mainLayout->addWidget(date_label);
  mainLayout->addWidget(date);
  mainLayout->addWidget(note_label);
  mainLayout->addWidget(note);
	mainLayout->addWidget(submitPB);
  mainLayout->addSpacing(30);
  mainLayout->addLayout(operations_buttons_layout); 
  mainLayout->addWidget(operationsView);
  mainLayout->addLayout(buttonsLayout); 

  connect(save_incomePB,&QPushButton::clicked,this,&IncomeDialog::slot_saveIncome); 
  connect(cancel_incomePB,&QPushButton::clicked,this,&IncomeDialog::slot_cancelIncome); 
  connect(operationsView,&QTableView::doubleClicked,this,&IncomeDialog::slot_open_itemsList);
	connect(operations_addPB,&QPushButton::clicked,this,&IncomeDialog::slot_add_operation);
//	connect(operations_copyPB,&QPushButton::clicked,this,&IncomeDialog::slot_copy_operation);
//	connect(operations_removePB,&QPushButton::clicked,this,&IncomeDialog::slot_remove_operation);
} 

void IncomeDialog::setup_ModelandMapper() {
  mapper=new QDataWidgetMapper();
  mapper->setModel(ptr_incomesModel);
  mapper->addMapping(op_number,ptr_incomesModel->fieldIndex("operation_number"));
  mapper->addMapping(date,ptr_incomesModel->fieldIndex("date"));
  mapper->addMapping(note,ptr_incomesModel->fieldIndex("note"));
  mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

void IncomeDialog::func_addIncome() {
  int row=ptr_incomesModel->rowCount();
	ptr_incomesModel->insertRow(row);
	mapper->setCurrentModelIndex(ptr_incomesModel->index(row,1));
	QString num="in-"+QString::number(row+1);
	op_number->setText(num);
	operations_proxymodel->setFilterRegExp(op_number->text());
	ptr_incomesView->setRowHidden(ptr_incomesModel->rowCount()-1,true);
}

void IncomeDialog::func_editIncome(int row) {
	mapper->setCurrentModelIndex(ptr_incomesModel->index(row,1));
	operations_proxymodel->setFilterRegExp(op_number->text());
}

void IncomeDialog::func_copyIncome(int row) {
// insert row at the end of the list
// get every fields values from existing row (get indexes then store data)
	ptr_incomesModel->insertRow(ptr_incomesModel->rowCount(QModelIndex()));
	QModelIndex index_date=ptr_incomesModel->index(row,1);
	QVariant date=ptr_incomesModel->data(index_date,Qt::DisplayRole);
	QModelIndex index_note=ptr_incomesModel->index(row,4);
	QVariant note=ptr_incomesModel->data(index_note,Qt::DisplayRole);
// get inserted rows number, so we can put values in that rows columns
// set every fields values to inserted row (get indexes then set data)
	int rows=ptr_incomesModel->rowCount()-1;
	QModelIndex newindex_date=ptr_incomesModel->index(rows,1);
	ptr_incomesModel->setData(newindex_date,date,Qt::EditRole);
	QModelIndex newindex_note=ptr_incomesModel->index(rows,4);
	ptr_incomesModel->setData(newindex_note,note,Qt::EditRole);
// set inserted and filled out row hidden until saving (with save pb)
	ptr_incomesView->setRowHidden(ptr_incomesModel->rowCount()-1,true);
	mapper->toLast();
}

void IncomeDialog::slot_saveIncome() {
  if(operations_proxymodel->rowCount()==0) {
    QMessageBox::information(nullptr,"Warning message","Empty lines, fill them out please!");
  	return;
	}
  ptr_incomesView->setRowHidden(ptr_incomesModel->rowCount()-1,false);
  mapper->submit();      
  ptr_incomesModel->submitAll();
  ptr_operationsModel->submitAll();
  emit signal_ready();
  this->close();
}

void IncomeDialog::slot_cancelIncome() {
  if(operations_proxymodel->rowCount()==0)
    ptr_incomesModel->removeRow(ptr_incomesModel->rowCount()-1);
  emit signal_ready();
  this->close();
}

void IncomeDialog::slot_open_itemsList(QModelIndex index) {
	if(!index.isValid() || index.column()!=5)
		return;
	items_widget=new QWidget(this,Qt::Window);
	items_widget->setFixedSize(500,300);
	
	items_widget->setObjectName("borders_for_items_income");
	items_widget->setStyleSheet("QWidget#borders_for_items_income {" 
												"background-color: #ABE7B2; color: black;"
												"border: 1px solid #427A76; }");

	items_view=new QTableView();
	items_view->setModel(ptr_itemsModel);
  items_view->sortByColumn(0,Qt::AscendingOrder); /*finded out that view needed sort too*/
	items_view->setSelectionMode(QAbstractItemView::SingleSelection);
  items_view->setSelectionBehavior(QAbstractItemView::SelectRows);
  items_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
  items_view->setColumnHidden(0,true);
  items_view->verticalHeader()->setVisible(false);
//------------------------------some design tweaks for "items_view" 
  items_view->setStyleSheet(
    "QTableView {"
			"gridline-color: #ffcab0;  border: 2px solid #ffcab0;"
    	"background: #fdffcd;  selection-background-color: #f76b8a;}"
    "QTableView::item { padding: 5px;}"); 
	items_view_header=items_view->horizontalHeader();
  items_view_header->setStretchLastSection(true);
  items_view_header->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  items_view_header->setStyleSheet(
    "QHeaderView::section { background-color: #f95959;}");
  QFont items_view_headerFont("Colibri",10,QFont::Bold);
  items_view_header->setFont(items_view_headerFont);

	items_widget_layout=new QVBoxLayout(items_widget);

	items_buttons_layout=new QHBoxLayout();
	select_itemPB=new QPushButton("Select");
	cancel_itemPB=new QPushButton("Cancel");
	items_buttons_layout->addWidget(select_itemPB);
	items_buttons_layout->addWidget(cancel_itemPB);
	
	items_widget_layout->addWidget(items_view);
	items_widget_layout->addLayout(items_buttons_layout);
	items_widget->show();

	connect(items_view,&QTableView::doubleClicked,this,&IncomeDialog::slot_passSelectedItem);
	connect(select_itemPB,&QPushButton::clicked,this,&IncomeDialog::slot_passSelectedItem);
	connect(cancel_itemPB,&QPushButton::clicked,[=]() { items_widget->close(); });
}

void IncomeDialog::slot_passSelectedItem() {
//---Get the index of selected item from items_view
	QModelIndex item_index=ptr_itemsModel->index(items_view->currentIndex().row(),1);
	if(!item_index.isValid()) {
    QMessageBox::information(nullptr,"Warning message","Nothing selected, pick an item please!");
  	return;
	}
//---Get the text from selected item
	QVariant data=ptr_itemsModel->data(item_index,Qt::DisplayRole);
	QString str=data.toString();
//---Get the index of row in operations view of our widget
	QModelIndex op_index=operations_proxymodel->index(operationsView->currentIndex().row(),5);
//---Get the correspond index in main operations model
	QModelIndex correspond_index=operations_proxymodel->mapToSource(op_index);
	if(!op_index.isValid()) {
    QMessageBox::information(nullptr,"Warning message","Nothing selected, select a row please!");
  	return;
	}
//---Set the selected items text by index in operations model
	ptr_operationsModel->setData(correspond_index,str,Qt::EditRole);
	items_widget->close();
}

void IncomeDialog::slot_add_operation() {
	int row=ptr_operationsModel->rowCount();
	ptr_operationsModel->insertRow(row);	
//	operationsView->setRowHidden(row,true);
	QDate date_qdate=date->date();
	ptr_operationsModel->setData(ptr_operationsModel->index(row,1),date_qdate,Qt::EditRole);
	ptr_operationsModel->setData(ptr_operationsModel->index(row,2),op_number->text(),Qt::EditRole);
	ptr_operationsModel->setData(ptr_operationsModel->index(row,3),"income operation",Qt::EditRole);
}
