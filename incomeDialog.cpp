#include "incomeDialog.h"

IncomeDialog::IncomeDialog(QSqlTableModel *model,QTableView *view,QSqlTableModel *itemsModel,
  QSqlTableModel *opModel,int row,QWidget *parent) : QDialog(parent), ptr_incomesModel(model),
	ptr_incomesView(view), ptr_itemsModel(itemsModel), ptr_operationsModel(opModel) {
//----------------------------------------------------------------------------------------
  setup_Widget();
  setup_ModelandMapper();
  if(row==-1)
  	func_addIncome();
	else
		func_editIncome(row);
}

void IncomeDialog::setup_Widget() {
  setWindowTitle("Creating new income");
  setFixedSize(600,800);
	this->setWindowFlags(Qt::FramelessWindowHint);
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
	
	operations_addPB->setStyleSheet("background-color: #FFC50F");
	operations_copyPB->setStyleSheet("background-color: #FFC50F");
	operations_removePB->setStyleSheet("background-color: #FFC50F");
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
  mainLayout->addSpacing(30);
  mainLayout->addLayout(operations_buttons_layout); 
  mainLayout->addWidget(operationsView);
  mainLayout->addLayout(buttonsLayout); 
//---Setting shortcuts and making pbs autodefault false, so hitting enter not activated them
	operations_addPB->setAutoDefault(false);
	operations_copyPB->setAutoDefault(false);
	operations_removePB->setAutoDefault(false);
	save_incomePB->setAutoDefault(false);
	cancel_incomePB->setAutoDefault(false);

	shortcut_add_operation=new QShortcut(this);
	shortcut_add_operation->setKey(Qt::Key_F8);
	shortcut_add_operation->setContext(Qt::WidgetWithChildrenShortcut);
	connect(shortcut_add_operation,&QShortcut::activated,this,&IncomeDialog::slot_add_operation);

	shortcut_copy_operation=new QShortcut(this);
	shortcut_copy_operation->setKey(Qt::Key_F9);
	shortcut_copy_operation->setContext(Qt::WidgetWithChildrenShortcut);
	connect(shortcut_copy_operation,&QShortcut::activated,this,&IncomeDialog::slot_copy_operation);
	
	shortcut_remove_operation=new QShortcut(this);
	shortcut_remove_operation->setKey(Qt::Key_Delete);
	shortcut_remove_operation->setContext(Qt::WidgetWithChildrenShortcut);
	connect(shortcut_remove_operation,&QShortcut::activated,this,&IncomeDialog::slot_remove_operation);
//------------------------------------------------------------------------------------------

  connect(save_incomePB,&QPushButton::clicked,this,&IncomeDialog::slot_saveIncome); 
  connect(cancel_incomePB,&QPushButton::clicked,this,&IncomeDialog::slot_cancelIncome); 
  connect(operationsView,&QTableView::doubleClicked,this,&IncomeDialog::slot_open_itemsList);
	connect(operations_addPB,&QPushButton::clicked,this,&IncomeDialog::slot_add_operation);
	connect(operations_copyPB,&QPushButton::clicked,this,&IncomeDialog::slot_copy_operation);
	connect(operations_removePB,&QPushButton::clicked,this,&IncomeDialog::slot_remove_operation);
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
//---Insert row and set mapper to it
  int row=ptr_incomesModel->rowCount();
	ptr_incomesModel->insertRow(row);
	mapper->setCurrentModelIndex(ptr_incomesModel->index(row,1));
//---Get the string for operation_number and set it to lineedit op_number
	QString num="in-"+QString::number(row+1);
	op_number->setText(num);
//---Set filter for operations proxymodel, so it to show operations considered to new op_number
	operations_proxymodel->setFilterRegExp(op_number->text());
//---Hide the newly inserted row, to unhide it only on pressing "Save" button 
	ptr_incomesView->setRowHidden(ptr_incomesModel->rowCount()-1,true);
}

void IncomeDialog::func_editIncome(int row) {
	mapper->setCurrentModelIndex(ptr_incomesModel->index(row,1));
	operations_proxymodel->setFilterRegExp(op_number->text());
}

int IncomeDialog::func_check_correctness(const QSortFilterProxyModel *proxy) {
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);
  if(proxy->rowCount()==0) {
    QMessageBox::information(nullptr,"Warning message","No operations added!");
  	return -1;
	}
	int sum=0;
	for(int row=0;row!=proxy->rowCount();++row) {
		for(int column=1;column!=proxy->columnCount();++column) {
//------------------------------------------------------------------------------------
			if((proxy->index(row,column)).data().isNull()) {
				QMessageBox::information(nullptr,"Warning message","Some fields leaved empty!");
				return -1;
			}
//------------------------------------------------------------------------------------
			if(column==6) {
				int local_quantity=proxy->index(row,column).data().toInt();
				if(local_quantity<=0) {
					QMessageBox::information(nullptr,"Warning message",QString("%1 %2").arg("Quantity is less or equal '0': ").arg(local_quantity));
					return -1;
				}
				sum=sum+local_quantity;
			}
//------------------------------------------------------------------------------------
			if(column==4) {
				QString local_cell=proxy->index(row,column).data().toString();
				QString check_existance=QString("select exists(select 1 from cells where cell like '%1')").arg(local_cell);
				if(!query.exec(check_existance)) {
					qDebug()<<"error in work of 'query.exec': check if cell from editline even exists in 'cells'.";
					qDebug()<<"error type"<<query.lastError().type();
					qDebug()<<"error text"<<query.lastError().text();
					qDebug()<<"driver error"<<query.lastError().driverText();
					qDebug()<<"database error"<<query.lastError().databaseText();
					return -1;
				}
				if(query.next()) {
					if(local_cell.isEmpty()) {
						QMessageBox::information(nullptr,"Warning message","Some fields leaved empty!");
						return -1;
					}
					if(0==query.value(0).toInt()) {
						QMessageBox::information(nullptr,"Warning message",QString("%1 %2").arg("Wrong cell number: ").arg(local_cell));
						return -1;
					}
				}
			}
//------------------------------------------------------------------------------------
 		}
	}
	return sum;
}

void IncomeDialog::func_insert_update(const QSqlTableModel *ptr_tmp_operations) {
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);

	for(int row=0;row!=ptr_tmp_operations->rowCount();++row) {
//------------------------------------------------------------------------------------
		QString check_str=QString("select * from filled_cells where cell like '%1' and item like '%2'").arg(ptr_tmp_operations->index(row,4).data().toString())
																																																	 .arg(ptr_tmp_operations->index(row,5).data().toString());
		if(!query.exec(check_str)) {
			qDebug()<<"("<<__LINE__<<") "<<"error in work of 'query.exec': local checking in 'while' loop.";
			return;
		}
		if(query.next()) {
			QString update_str=QString("update filled_cells set quantity=quantity+'%1' where cell_id like '%2'")
																 .arg(ptr_tmp_operations->index(row,6).data().toInt()).arg(query.value(0).toInt());
			if(!query.exec(update_str)) {
				qDebug()<<"("<<__LINE__<<") "<<"Error while 'query.exec' (income): update record in not empty set in 'filled_cells'.";
				return;
			}
		} else {
//------------------------------------------------------------------------------------
			query.prepare("insert into filled_cells(cell,item,quantity) values(:c,:i,:q)");
			query.bindValue(":c",ptr_tmp_operations->index(row,4).data().toString());
			query.bindValue(":i",ptr_tmp_operations->index(row,5).data().toString());
			query.bindValue(":q",ptr_tmp_operations->index(row,6).data().toInt());
			if(!query.exec()) {
				qDebug()<<"("<<__LINE__<<") "<<"error in work of 'query.exec': inserting record in not empty set in 'filled_cells'.";
				return;
			}
		}
//------------------------------------------------------------------------------------
	}
}

void IncomeDialog::slot_saveIncome() {
	QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);

	int sum=func_check_correctness(operations_proxymodel);
	if(sum==-1)
    qDebug()<<"Error: Sum is not calculated. '-1' will be returned!";
//---------------------FINALIZING CODE FOR MAIN INCOME VIEW---------------------------
	QModelIndex sum_index;
	if(!ptr_incomesView->currentIndex().isValid())
		sum_index=ptr_incomesModel->index(ptr_incomesModel->rowCount()-1,4);
	else 
		sum_index=ptr_incomesModel->index(ptr_incomesView->currentIndex().row(),4);
	ptr_incomesModel->setData(sum_index,sum,Qt::EditRole);
  ptr_incomesView->setRowHidden(ptr_incomesModel->rowCount()-1,false);
  mapper->submit();      
  ptr_incomesModel->submitAll();
  ptr_operationsModel->submitAll();
  emit signal_ready();
  this->close();
//------------------------------------------------------------------------------------
}

void IncomeDialog::slot_update_filled_cells() {
	func_insert_update(ptr_operationsModel);
//	print somehow this result set
//	truncate this table, cause its temporary!!!
//	so we need to generate balance by clicking on some Generate button, print out result and after closing truncate filled_cells table!
}

void IncomeDialog::slot_cancelIncome() {
  if(operations_proxymodel->rowCount()==0)
		//-----------------------NEED TO WRITE CODE FOR DATA RETRIEVING, FEX AFTER DELETION,SO DATA WRITE BACK AS IT WAS

  emit signal_ready();
  this->close();
}

void IncomeDialog::slot_open_itemsList(QModelIndex index) {
	if(!index.isValid() || index.column()!=5)
		return;
	items_widget=new QWidget(this,Qt::Window);
	items_widget->setFixedSize(500,300);
//	items_widget->setWindowFlags(Qt::FramelessWindowHint);
	
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

	int col=4;
	if(operationsView->currentIndex().isValid())
		col=operationsView->currentIndex().column();
	operationsView->setCurrentIndex(operations_proxymodel->index(operations_proxymodel->rowCount()-1,col));
}

void IncomeDialog::slot_copy_operation() {
	if(operations_proxymodel->rowCount()==0) {
    QMessageBox::information(nullptr,"Warning message","Empty table, nothing to copy!");
		return;
	}
	if(!operationsView->currentIndex().isValid()) {
    QMessageBox::information(nullptr,"Warning message","Select row before copying!");
		return;
	}
	QModelIndex source_index=operationsView->currentIndex();
	slot_add_operation();
	QModelIndex dest_index;
	for(int col=4;col!=operations_proxymodel->columnCount();++col) {
		QModelIndex tmp_index=operations_proxymodel->index(source_index.row(),col);
		dest_index=operations_proxymodel->index(operations_proxymodel->rowCount()-1,col);
		QVariant var=operations_proxymodel->data(tmp_index,Qt::EditRole);
		if(col!=operations_proxymodel->columnCount()-1)
			operations_proxymodel->setData(dest_index,var.toString(),Qt::EditRole);
		else
			operations_proxymodel->setData(dest_index,var.toInt(),Qt::EditRole);
	}
	operationsView->setCurrentIndex(operations_proxymodel->index(dest_index.row(),source_index.column()));
}

void IncomeDialog::slot_remove_operation() {
	QModelIndex op_index=operations_proxymodel->index(operationsView->currentIndex().row(),operationsView->currentIndex().column());
	if(!op_index.isValid()) {
    QMessageBox::information(nullptr,"Warning message","Select row before deletion!");
		return;
	}
	int last_row_before_remove=operations_proxymodel->rowCount()-1;

	QModelIndex correspond_index=operations_proxymodel->mapToSource(op_index);
	ptr_operationsModel->removeRow(correspond_index.row());
	ptr_operationsModel->submitAll();
	ptr_incomesModel->select();

	if(op_index.row()<last_row_before_remove)
		operationsView->setCurrentIndex(operations_proxymodel->index(op_index.row(),op_index.column()));
	else
		operationsView->setCurrentIndex(operations_proxymodel->index(operations_proxymodel->rowCount()-1,op_index.column()));

	emit signal_ready();
}
