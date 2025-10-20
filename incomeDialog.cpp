#include "incomeDialog.h"

IncomeDialog::IncomeDialog(QSqlTableModel *model, QTableView *view,QSqlTableModel *itemsModel,int row,bool copy,QWidget *parent) 
							: QDialog(parent), ptr_incomesModel(model), ptr_incomesView(view), ptr_itemsModel(itemsModel) {
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
//  setFixedSize(500,500);

  mainLayout=new QVBoxLayout(this);
	item_layout=new QHBoxLayout();	
  buttonsLayout=new QHBoxLayout();
  
	date_label=new QLabel("Date");
	date=new QDateEdit();
	date->setDate(QDate::currentDate());
	date->setDisplayFormat("dd/MM/yyyy");

	item_label=new QLabel("Item");
	item=new QLineEdit();
	item_label->setBuddy(item);
	open_itemsListPB=new QPushButton("->");
  item_layout->addWidget(item);
  item_layout->addWidget(open_itemsListPB);
	
	quantity_label=new QLabel("Quantity");
	quantity=new QLineEdit();
	quantity_label->setBuddy(quantity);
	validator=new QIntValidator(0,9999,this);
	quantity->setValidator(validator);

	note_label=new QLabel("Note");
	note=new QLineEdit();
	note_label->setBuddy(note);
	
  mainLayout->addWidget(date_label);
  mainLayout->addWidget(date);
  mainLayout->addWidget(item_label);
  mainLayout->addLayout(item_layout);
  mainLayout->addWidget(quantity_label);
  mainLayout->addWidget(quantity);
  mainLayout->addWidget(note_label);
  mainLayout->addWidget(note);
	
	save_incomePB=new QPushButton("Save");
  cancel_incomePB=new QPushButton("Cancel");
  buttonsLayout->addWidget(save_incomePB);
  buttonsLayout->addWidget(cancel_incomePB);
  mainLayout->addLayout(buttonsLayout); 
  
  connect(save_incomePB,&QPushButton::clicked,this,&IncomeDialog::slot_saveIncome); 
  connect(cancel_incomePB,&QPushButton::clicked,this,&IncomeDialog::slot_cancelIncome); 
  connect(open_itemsListPB,&QPushButton::clicked,this,&IncomeDialog::slot_openItemsList);
} 

void IncomeDialog::setup_ModelandMapper() {
  mapper_income=new QDataWidgetMapper();
  mapper_income->setModel(ptr_incomesModel);
  mapper_income->addMapping(date,ptr_incomesModel->fieldIndex("date"));
  mapper_income->addMapping(item,ptr_incomesModel->fieldIndex("item"));
  mapper_income->addMapping(quantity,ptr_incomesModel->fieldIndex("quantity"));
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
	ptr_incomesModel->insertRow(ptr_incomesModel->rowCount(QModelIndex()));
// get every fields values from existing row (get indexes then store data)
	QModelIndex index_date=ptr_incomesModel->index(row,1);
	QVariant date=ptr_incomesModel->data(index_date,Qt::DisplayRole);
	QModelIndex index_quantity=ptr_incomesModel->index(row,3);
	QVariant quantity=ptr_incomesModel->data(index_quantity,Qt::DisplayRole);
	QModelIndex index_note=ptr_incomesModel->index(row,4);
	QVariant note=ptr_incomesModel->data(index_note,Qt::DisplayRole);
	QModelIndex index_item=ptr_incomesModel->index(row,5);
	QVariant item=ptr_incomesModel->data(index_item,Qt::DisplayRole);
// get inserted rows number, so we can put values in that rows columns
	int rows=ptr_incomesModel->rowCount()-1;
// set every fields values to inserted row (get indexes then set data)
	QModelIndex newindex_date=ptr_incomesModel->index(rows,1);
	ptr_incomesModel->setData(newindex_date,date,Qt::EditRole);
	QModelIndex newindex_quantity=ptr_incomesModel->index(rows,3);
	ptr_incomesModel->setData(newindex_quantity,quantity,Qt::EditRole);
	QModelIndex newindex_note=ptr_incomesModel->index(rows,4);
	ptr_incomesModel->setData(newindex_note,note,Qt::EditRole);
	QModelIndex newindex_item=ptr_incomesModel->index(rows,5);
	ptr_incomesModel->setData(newindex_item,item,Qt::EditRole);
// set inserted and filled out row hidden until saving (with save pb)
	ptr_incomesView->setRowHidden(ptr_incomesModel->rowCount()-1,true);
	mapper_income->toLast();
}

void IncomeDialog::slot_openItemsList() {
	itemsView_income_widget=new QWidget(this,Qt::Window);
	itemsView_income_widget->setFixedSize(500,300);

	itemsView_income=new QTableView();
	itemsView_income->setModel(ptr_itemsModel);
  itemsView_income->sortByColumn(0,Qt::AscendingOrder); /*finded out that view needed sort too*/
	itemsView_income->setSelectionMode(QAbstractItemView::SingleSelection);
  itemsView_income->setSelectionBehavior(QAbstractItemView::SelectRows);
  itemsView_income->setEditTriggers(QAbstractItemView::NoEditTriggers);
  itemsView_income->setColumnHidden(0,true);
  itemsView_income->verticalHeader()->setVisible(false);
//------------------------------some design tweaks for "itemsView_income" 
  itemsView_income->setStyleSheet(
    "QTableView {"
			"gridline-color: #ffcab0;  border: 2px solid #ffcab0;"
    	"background: #fdffcd;  selection-background-color: #f76b8a;}"
    "QTableView::item { padding: 5px;}"); 
	itemsView_income_header=itemsView_income->horizontalHeader();
  itemsView_income_header->setStretchLastSection(true);
  itemsView_income_header->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  itemsView_income_header->setStyleSheet(
    "QHeaderView::section { background-color: #f95959;}");
  QFont itemsView_income_headerFont("Colibri",10,QFont::Bold);
  itemsView_income_header->setFont(itemsView_income_headerFont);

	itemsView_income_widget_layout=new QVBoxLayout(itemsView_income_widget);

	select_buttons_layout=new QHBoxLayout();
	selectPB=new QPushButton("Select");
	cancel_selectPB=new QPushButton("Cancel");
	select_buttons_layout->addWidget(selectPB);
	select_buttons_layout->addWidget(cancel_selectPB);
	
	itemsView_income_widget_layout->addWidget(itemsView_income);
	itemsView_income_widget_layout->addLayout(select_buttons_layout);
	itemsView_income_widget->show();

	connect(itemsView_income,&QTableView::doubleClicked,this,&IncomeDialog::slot_passSelectedItem);
	connect(selectPB,&QPushButton::clicked,this,&IncomeDialog::slot_passSelectedItem);
	connect(cancel_selectPB,&QPushButton::clicked,
	[=]() { itemsView_income_widget->close(); });
}

void IncomeDialog::slot_saveIncome() {
  if(date->text().isEmpty() || item->text().isEmpty() || quantity->text().isEmpty()) {
    QMessageBox::information(nullptr,"Warning message","Empty lines, fill them out please!");
  	return;
	}
  ptr_incomesView->setRowHidden(ptr_incomesModel->rowCount()-1,false);
  mapper_income->submit();      
  ptr_incomesModel->submitAll();
  emit signal_ready();
  this->close();
}

void IncomeDialog::slot_cancelIncome() {
  if(date->text().isEmpty() || item->text().isEmpty() || quantity->text().isEmpty())
    ptr_incomesModel->removeRow(ptr_incomesModel->rowCount()-1);
  emit signal_ready();
  this->close();
}

void IncomeDialog::slot_passSelectedItem() {
	QModelIndex index=itemsView_income->currentIndex();
	QVariant data=ptr_itemsModel->data(index,Qt::DisplayRole);
	QString str=data.toString();
	item->setText(str);
	itemsView_income_widget->close();
}
