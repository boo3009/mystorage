#include "incomeDialog.h"

IncomeDialog::IncomeDialog(QSqlTableModel *model,QTableView *view,QSqlTableModel *itemsModel,
  QSqlTableModel *opModel,int row,bool copy,QWidget *parent) : QDialog(parent), ptr_incomesModel(model), 
	ptr_incomesView(view), ptr_itemsModel_income(itemsModel), ptr_operationsModel(opModel) {
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

  mainLayout=new QVBoxLayout(this);
  buttonsLayout=new QHBoxLayout();
  
	op_number_label=new	QLabel("Operation Number");
	op_number=new QLineEdit();
	op_number->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	op_number->setReadOnly(true);

	date_label=new QLabel("Date");
	date=new QDateEdit();
	date->setDate(QDate::currentDate());
	date->setDisplayFormat("dd/MM/yyyy");
	date->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

	operationsView_income=new QTableView(this);
	operationsView_income->setModel(ptr_operationsModel);
  operationsView_income->sortByColumn(0,Qt::AscendingOrder);
	operationsView_income->setSelectionMode(QAbstractItemView::SingleSelection);
//  operationsView_income->setSelectionBehavior(QAbstractItemView::SelectRows);
//  operationsView_income->setEditTriggers(QAbstractItemView::NoEditTriggers);
  operationsView_income->setColumnHidden(0,true);
  operationsView_income->setColumnHidden(1,true);
//  operationsView_income->setColumnHidden(2,true);
  operationsView_income->setColumnHidden(3,true);
  operationsView_income->verticalHeader()->setVisible(false);
	operationsViewHeader_income=operationsView_income->horizontalHeader();
	operationsViewHeader_income->setStretchLastSection(true);

	operationsProxyModel_income=new QSortFilterProxyModel(this);
	operationsProxyModel_income->setSourceModel(ptr_operationsModel);
	operationsView_income->setModel(operationsProxyModel_income);
	operationsProxyModel_income->setFilterKeyColumn(2);
	operationsProxyModel_income->setFilterRegExp(op_number->text());

	note_label=new QLabel("Note");
	note=new QLineEdit();
	
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
  mainLayout->addWidget(operationsView_income);
  mainLayout->addLayout(buttonsLayout); 

	this->setStyleSheet("background-color: #fdffcd; color: black;");
  
  connect(save_incomePB,&QPushButton::clicked,this,&IncomeDialog::slot_saveIncome); 
  connect(cancel_incomePB,&QPushButton::clicked,this,&IncomeDialog::slot_cancelIncome); 
  connect(operationsView_income,&QTableView::doubleClicked,this,&IncomeDialog::slot_open_itemsList);
} 

void IncomeDialog::setup_ModelandMapper() {
  mapper_income=new QDataWidgetMapper();
  mapper_income->setModel(ptr_incomesModel);
  mapper_income->addMapping(op_number,ptr_incomesModel->fieldIndex("operation_number"));
  mapper_income->addMapping(date,ptr_incomesModel->fieldIndex("date"));
  mapper_income->addMapping(note,ptr_incomesModel->fieldIndex("note"));
  mapper_income->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

void IncomeDialog::func_addIncome() {
	ptr_incomesModel->insertRow(ptr_incomesModel->rowCount(QModelIndex()));
	ptr_incomesView->setRowHidden(ptr_incomesModel->rowCount()-1,true);
	mapper_income->toLast();
}

void IncomeDialog::func_editIncome(int row) {
	mapper_income->setCurrentModelIndex(ptr_incomesModel->index(row,1));
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
	mapper_income->toLast();
}

void IncomeDialog::slot_saveIncome() {
  if(operationsProxyModel_income->rowCount()==0) {
    QMessageBox::information(nullptr,"Warning message","Empty lines, fill them out please!");
  	return;
	}
  ptr_incomesView->setRowHidden(ptr_incomesModel->rowCount()-1,false);
  mapper_income->submit();      
  ptr_incomesModel->submitAll();
  ptr_operationsModel->submitAll();
  emit signal_ready();
  this->close();
}

void IncomeDialog::slot_cancelIncome() {
  if(operationsProxyModel_income->rowCount()==0)
    ptr_incomesModel->removeRow(ptr_incomesModel->rowCount()-1);
  emit signal_ready();
  this->close();
}

void IncomeDialog::slot_open_itemsList(QModelIndex index) {
	if(!index.isValid() || index.column()!=5)
		return;
	items_income_widget=new QWidget(this,Qt::Window);
	items_income_widget->setFixedSize(500,300);

	items_income_view=new QTableView();
	items_income_view->setModel(ptr_itemsModel_income);
  items_income_view->sortByColumn(0,Qt::AscendingOrder); /*finded out that view needed sort too*/
	items_income_view->setSelectionMode(QAbstractItemView::SingleSelection);
  items_income_view->setSelectionBehavior(QAbstractItemView::SelectRows);
  items_income_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
  items_income_view->setColumnHidden(0,true);
  items_income_view->verticalHeader()->setVisible(false);
//------------------------------some design tweaks for "items_income_view" 
  items_income_view->setStyleSheet(
    "QTableView {"
			"gridline-color: #ffcab0;  border: 2px solid #ffcab0;"
    	"background: #fdffcd;  selection-background-color: #f76b8a;}"
    "QTableView::item { padding: 5px;}"); 
	items_income_view_header=items_income_view->horizontalHeader();
  items_income_view_header->setStretchLastSection(true);
  items_income_view_header->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  items_income_view_header->setStyleSheet(
    "QHeaderView::section { background-color: #f95959;}");
  QFont items_income_view_headerFont("Colibri",10,QFont::Bold);
  items_income_view_header->setFont(items_income_view_headerFont);

	items_income_widget_layout=new QVBoxLayout(items_income_widget);

	items_select_buttons_layout=new QHBoxLayout();
	select_itemPB=new QPushButton("Select");
	cancel_itemPB=new QPushButton("Cancel");
	items_select_buttons_layout->addWidget(select_itemPB);
	items_select_buttons_layout->addWidget(cancel_itemPB);
	
	items_income_widget_layout->addWidget(items_income_view);
	items_income_widget_layout->addLayout(items_select_buttons_layout);
	items_income_widget->show();

	connect(items_income_view,&QTableView::doubleClicked,this,&IncomeDialog::slot_passSelectedItem);
	connect(select_itemPB,&QPushButton::clicked,this,&IncomeDialog::slot_passSelectedItem);
	connect(cancel_itemPB,&QPushButton::clicked,[=]() { items_income_widget->close(); });
}

void IncomeDialog::slot_passSelectedItem() {
	QModelIndex cur_index=items_income_view->currentIndex();
	if(!cur_index.isValid()) {
    QMessageBox::information(nullptr,"Warning message","Nothing selected, pick an item please!");
  	return;
	}
	QVariant data=ptr_itemsModel_income->data(cur_index,Qt::DisplayRole);
	QString str=data.toString();
	QModelIndex op_index=ptr_operationsModel->index(operationsView_income->currentIndex().row(),5);
	ptr_operationsModel->setData(op_index,str,Qt::EditRole);
	items_income_widget->close();
}
