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
  mapper=new QDataWidgetMapper();
  mapper->setModel(ptr_incomesModel);
  mapper->addMapping(date_label,ptr_incomesModel->fieldIndex("date"));
  mapper->addMapping(item_label,ptr_incomesModel->fieldIndex("item"));
  mapper->addMapping(quantity_label,ptr_incomesModel->fieldIndex("quantity"));
  mapper->addMapping(note_label,ptr_incomesModel->fieldIndex("note"));
  mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

void IncomeDialog::func_addIncome() {
	ptr_incomesModel->insertRow(ptr_incomesModel->rowCount(QModelIndex()));
	ptr_incomesView->setRowHidden(ptr_incomesModel->rowCount()-1,true);
	mapper->toLast();
}

void IncomeDialog::func_editIncome(int row) {
	mapper->setCurrentModelIndex(ptr_incomesModel->index(row,1));
}

void IncomeDialog::func_copyIncome(int row) {
	ptr_incomesModel->insertRow(ptr_incomesModel->rowCount(QModelIndex()));
	QString str=ptr_incomesModel->data(ptr_incomesModel->index(row,1),Qt::EditRole).toString();
	QModelIndex index=ptr_incomesModel->index(ptr_incomesModel->rowCount()-1,1);
	ptr_incomesModel->setData(index,str,Qt::EditRole);
	ptr_incomesView->setRowHidden(ptr_incomesModel->rowCount()-1,true);
	mapper->toLast();
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
	itemsView_income_widget_layout->addWidget(itemsView_income);
	itemsView_income_widget->show();

	connect(itemsView_income,&QTableView::doubleClicked,this,&IncomeDialog::slot_passSelectedItem);
}

void IncomeDialog::slot_saveIncome() {
  if(date_label->text().isEmpty() || item_label->text().isEmpty() || quantity_label->text().isEmpty()) {
    QMessageBox::information(nullptr,"Warning message","Empty lines, fill them out please!");
  	return;
	}
  ptr_incomesView->setRowHidden(ptr_incomesModel->rowCount()-1,false);
  mapper->submit();      
  ptr_incomesModel->submitAll();
  emit signal_ready();
  this->close();
}

void IncomeDialog::slot_cancelIncome() {
  if(date_label->text().isEmpty() || item_label->text().isEmpty() || quantity_label->text().isEmpty())
    ptr_incomesModel->removeRow(ptr_incomesModel->rowCount()-1);
  emit signal_ready();
  this->close();
}

void IncomeDialog::slot_passSelectedItem(QModelIndex index) {
	QVariant data=ptr_itemsModel->data(index,Qt::DisplayRole);
	QString str=data.toString();
	item->setText(str);
}
