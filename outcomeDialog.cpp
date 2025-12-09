#include "outcomeDialog.h"

OutcomeDialog::OutcomeDialog(QSqlTableModel *model,QTableView *view,QSqlTableModel *itemsModel,
  QSqlTableModel *opModel,QSqlTableModel *bal_model,int row,QWidget *parent) : QDialog(parent), ptr_outcomesModel(model),
	ptr_outcomesView(view), ptr_itemsModel(itemsModel), ptr_operationsModel(opModel), ptr_balanceModel(bal_model)
{
//----------------------------------------------------------------------------------------
  setup_Widget();
  setup_ModelandMapper();
  if(row==-1)
  	func_addOutcome();
	else
		func_editOutcome(row);
}

void OutcomeDialog::setup_Widget() {
  setWindowTitle("Creating new outcome");
  setFixedSize(600,800);
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setObjectName("borders_for_outcome_dialog");
	this->setStyleSheet("QWidget#borders_for_outcome_dialog {" 
												"background-color: #FFB6C1; color: black;"
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
	date->setDisplayFormat("dd.MM.yyyy");
	date->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

	note_label=new QLabel("Note");
	note=new QLineEdit();

//---------------------operations part-------------------
	operationsView=new QTableView(this);
	operationsView->setModel(ptr_operationsModel);
  operationsView->sortByColumn(0,Qt::AscendingOrder);
	operationsView->setSelectionMode(QAbstractItemView::SingleSelection);
  operationsView->setColumnHidden(0,true);
  operationsView->setColumnHidden(1,true);
  operationsView->setColumnHidden(2,true);
  operationsView->setColumnHidden(3,true);
  operationsView->setColumnHidden(4,true);
  operationsView->verticalHeader()->setVisible(false);
	operationsView_header=operationsView->horizontalHeader();
	operationsView_header->setStretchLastSection(true);
//	operationsView_header->setSectionResizeMode(QHeaderView::ResizeToContents);
	operationsView->setColumnWidth(5,80);
	operationsView->setColumnWidth(6,400);

	operations_proxymodel=new Proxy_op_number(this);
	operations_proxymodel->setSourceModel(ptr_operationsModel);
	operations_proxymodel->setFilterKeyColumn(ptr_operationsModel->fieldIndex("operation_number"));
	operationsView->setModel(operations_proxymodel);

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
	save_outcomePB=new QPushButton("Save");
  cancel_outcomePB=new QPushButton("Cancel");
  buttonsLayout->addWidget(save_outcomePB);
  buttonsLayout->addWidget(cancel_outcomePB);
	
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
	save_outcomePB->setAutoDefault(false);
	cancel_outcomePB->setAutoDefault(false);

	shortcut_add_operation=new QShortcut(this);
	shortcut_add_operation->setKey(Qt::Key_F8);
	shortcut_add_operation->setContext(Qt::WidgetWithChildrenShortcut);
	connect(shortcut_add_operation,&QShortcut::activated,this,&OutcomeDialog::slot_add_operation);

	shortcut_copy_operation=new QShortcut(this);
	shortcut_copy_operation->setKey(Qt::Key_F9);
	shortcut_copy_operation->setContext(Qt::WidgetWithChildrenShortcut);
	connect(shortcut_copy_operation,&QShortcut::activated,this,&OutcomeDialog::slot_copy_operation);
	
	shortcut_remove_operation=new QShortcut(this);
	shortcut_remove_operation->setKey(Qt::Key_Delete);
	shortcut_remove_operation->setContext(Qt::WidgetWithChildrenShortcut);
	connect(shortcut_remove_operation,&QShortcut::activated,this,&OutcomeDialog::slot_remove_operation);
//------------------------------------------------------------------------------------------

  connect(save_outcomePB,&QPushButton::clicked,this,&OutcomeDialog::slot_saveOutcome); 
  connect(cancel_outcomePB,&QPushButton::clicked,this,&OutcomeDialog::slot_cancelOutcome); 
  connect(operationsView,&QTableView::doubleClicked,this,&OutcomeDialog::slot_open_itemsList);
	connect(operations_addPB,&QPushButton::clicked,this,&OutcomeDialog::slot_add_operation);
	connect(operations_copyPB,&QPushButton::clicked,this,&OutcomeDialog::slot_copy_operation);
	connect(operations_removePB,&QPushButton::clicked,this,&OutcomeDialog::slot_remove_operation);
} 

void OutcomeDialog::setup_ModelandMapper() {
  mapper=new QDataWidgetMapper();
  mapper->setModel(ptr_outcomesModel);
  mapper->addMapping(op_number,ptr_outcomesModel->fieldIndex("operation_number"));
  mapper->addMapping(date,ptr_outcomesModel->fieldIndex("date"));
  mapper->addMapping(note,ptr_outcomesModel->fieldIndex("note"));
  mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

void OutcomeDialog::func_addOutcome() {
//---Insert row and set mapper to it
  int row=ptr_outcomesModel->rowCount();
	ptr_outcomesModel->insertRow(row);
	row_added=true;
	mapper->setCurrentModelIndex(ptr_outcomesModel->index(row,1));
//---Get the string for operation_number and set it to lineedit op_number
	QString num="out-"+QString::number(row+1);
	op_number->setText(num);
//---Set filter for operations proxymodel to show operations considered to new op_number
	operations_proxymodel->setFilterPattern(op_number->text());
//---Hide the newly inserted row, to unhide it only on pressing "Save" button 
	ptr_outcomesView->setRowHidden(ptr_outcomesModel->rowCount()-1,true);
}

void OutcomeDialog::func_editOutcome(int row) {
	row_added=false;
	mapper->setCurrentModelIndex(ptr_outcomesModel->index(row,1));
	operations_proxymodel->setFilterPattern(op_number->text());
	if(func_revert_insert_update()==-1) {
		qDebug()<<"Something went wrong while reverting inserts and updates of current outcome. Returning.";
		return;
	}
}

int OutcomeDialog::func_check_correctness(const QSortFilterProxyModel *proxy,int *sum) {
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);
  if(proxy->rowCount()==0) {
    QMessageBox::information(nullptr,"Warning message","No operations added!");
  	return -1;
	}
	for(int row=0;row!=proxy->rowCount();++row) {
		for(int column=1;column!=proxy->columnCount();++column) {
//------------------------------------------------------------------------------------
			if((proxy->index(row,column)).data().isNull()) {
				QMessageBox::information(nullptr,"Warning message","Some fields leaved empty!");
				return -1;
			}
//------------------------------------------------------------------------------------
			if(column==ptr_operationsModel->fieldIndex("quantity") && !operationsView->isRowHidden(row)) {
				int local_quantity=proxy->index(row,column).data().toInt();
				if(local_quantity<=0) {
					QMessageBox::information(nullptr,"Warning message",QString("%1 %2").arg("Quantity is less or equal '0': ").arg(local_quantity));
					return -1;
				}
				*sum=*sum+local_quantity;
			}
//------------------------------------------------------------------------------------
			if(column==ptr_operationsModel->fieldIndex("cell")) {
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
	return 0;
}

int OutcomeDialog::func_insert_update() {
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);
	if(!query.exec("drop temporary table if exists tmp_filled_cells")) {
		qDebug()<<"("<<__LINE__<<") "<<"error in work of 'query.exec': Cant drop temporary table for filled_cells.";
		qDebug()<<"error type"<<query.lastError().type();
		qDebug()<<"error text"<<query.lastError().text();
		qDebug()<<"driver error"<<query.lastError().driverText();
		qDebug()<<"database error"<<query.lastError().databaseText();
		return -1;
	}
	if(!query.exec("create temporary table tmp_filled_cells as select * from filled_cells")) {
		qDebug()<<"("<<__LINE__<<") "<<"error in work of 'query.exec': Cant create temporary table for filled_cells.";
		qDebug()<<"error type"<<query.lastError().type();
		qDebug()<<"error text"<<query.lastError().text();
		qDebug()<<"driver error"<<query.lastError().driverText();
		qDebug()<<"database error"<<query.lastError().databaseText();
		return -1;
	}
//   operations columns
	int cell_column=ptr_operationsModel->fieldIndex("cell");
	int item_column=ptr_operationsModel->fieldIndex("item");
	int quantity_column=ptr_operationsModel->fieldIndex("quantity");
	int status_column=ptr_operationsModel->fieldIndex("status");
//   filled_cells columns
	int balance_quantity_column=ptr_balanceModel->fieldIndex("quantity");
	int balance_cell_id_column=ptr_balanceModel->fieldIndex("cell_id");
//------------------------------------------------------------------------------------
	for(int row=0;row!=operations_proxymodel->rowCount();++row) {
		if(operations_proxymodel->index(row,status_column).data().toString()=="REMOVED")
			continue;
		QString check_str=QString("select * from tmp_filled_cells where cell like '%1' and item like '%2'")
		  .arg(operations_proxymodel->index(row,cell_column).data().toString())
			.arg(operations_proxymodel->index(row,item_column).data().toString());
		if(!query.exec(check_str)) {
			qDebug()<<"("<<__LINE__<<") "<<"error in work of 'query.exec': local checking in 'while' loop.";
			return -1;
		}
		if(query.next()) {
			if(query.value(balance_quantity_column).toInt()<operations_proxymodel->index(row,quantity_column).data().toInt()) {
				QMessageBox::information(nullptr,"Warning message",QString("%1 '%2' %3 '%4' %5 '%6', %7")
					.arg("In cell")
					.arg(operations_proxymodel->index(row,cell_column).data().toString())
					.arg("balance of item")
					.arg(operations_proxymodel->index(row,item_column).data().toString())
					.arg("is")
					.arg(query.value(balance_quantity_column).toInt())
					.arg("so correct the quantity please."));
				return -1;
			}
			QString update_str=QString("update tmp_filled_cells set quantity=quantity-'%1' where cell_id like '%2'")
				.arg(operations_proxymodel->index(row,quantity_column).data().toInt())
				.arg(query.value(balance_cell_id_column).toInt());
			if(!query.exec(update_str)) {
				qDebug()<<"("<<__LINE__<<") "<<"Error while 'query.exec' (income): update record in not empty set in 'filled_cells'.";
				return -1;
			}
			continue;
		} 
		QMessageBox::information(nullptr,"Warning message",QString("%1 '%2' %3 '%4' %5")
			.arg("Cell with number")
			.arg(operations_proxymodel->index(row,cell_column).data().toString())
			.arg("dont contain the item")
			.arg(operations_proxymodel->index(row,item_column).data().toString())
			.arg("so you cant write off."));
		return -1;
	}
//------------------------------------------------------------------------------------
	return 0;
}

int OutcomeDialog::func_revert_insert_update() {
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);
//   operations columns
	int cell_column=ptr_operationsModel->fieldIndex("cell");
	int item_column=ptr_operationsModel->fieldIndex("item");
	int quantity_column=ptr_operationsModel->fieldIndex("quantity");
	int status_column=ptr_operationsModel->fieldIndex("status");
//   balance columns
	int balance_cell_id_column=ptr_balanceModel->fieldIndex("cell_id");
//------------------------------------------------------------------------------------
	for(int row=0;row!=operations_proxymodel->rowCount();++row) {
		if(operations_proxymodel->index(row,status_column).data().toString()=="REMOVED")
			continue;
		QString check_str=QString("select * from filled_cells where cell like '%1' and item like '%2'")
		  .arg(operations_proxymodel->index(row,cell_column).data().toString())
			.arg(operations_proxymodel->index(row,item_column).data().toString());
		if(!query.exec(check_str)) {
			qDebug()<<"("<<__LINE__<<") "<<"error in work of 'query.exec': local checking in 'while' loop.";
			return -1;
		}
		if(query.next()) {
			QString update_str=QString("update filled_cells set quantity=quantity+'%1' where cell_id like '%2'")
				.arg(operations_proxymodel->index(row,quantity_column).data().toInt())
				.arg(query.value(balance_cell_id_column).toInt());
			if(!query.exec(update_str)) {
				qDebug()<<"("<<__LINE__<<") "<<"Error while 'query.exec' (income): update record in not empty set in 'filled_cells'.";
				return -1;
			}
			continue;
		}
		query.prepare("insert into filled_cells(cell,item,quantity) values(:c,:i,:q)");
		query.bindValue(":c",operations_proxymodel->index(row,cell_column).data().toString());
		query.bindValue(":i",operations_proxymodel->index(row,item_column).data().toString());
		query.bindValue(":q",-operations_proxymodel->index(row,quantity_column).data().toInt());
		if(!query.exec()) {
			qDebug()<<"("<<__LINE__<<") "<<"error in work of 'query.exec': inserting record in not empty set in 'filled_cells'.";
			return -1;
		}
	}
//------------------------------------------------------------------------------------
	return 0;
}

void OutcomeDialog::slot_saveOutcome() {
	int sum=0;
	if(func_check_correctness(operations_proxymodel,&sum)==0 && func_insert_update()==0) {
		QModelIndex index=ptr_outcomesModel->index(ptr_outcomesView->currentIndex().row(),ptr_outcomesModel->fieldIndex("sum"));
		if(row_added)
			index=ptr_outcomesModel->index(ptr_outcomesModel->rowCount()-1,ptr_outcomesModel->fieldIndex("sum"));
		if(!index.isValid()) {
			qDebug()<<"Invalid index of row...Returning.";
			return;
		}
		ptr_outcomesModel->setData(index,sum,Qt::EditRole);

		mapper->submit();      
		ptr_operationsModel->submitAll();
		ptr_outcomesModel->submitAll();
		ptr_outcomesView->setRowHidden(ptr_outcomesModel->rowCount()-1,false);
		emit signal_ready();
		this->close();
	}
}

void OutcomeDialog::slot_cancelOutcome() {
  if(row_added)
		ptr_outcomesModel->removeRow(ptr_outcomesModel->rowCount()-1);
  emit signal_ready();
	this->close();
}

void OutcomeDialog::slot_open_itemsList(QModelIndex index) {
	if(!index.isValid() || index.column()!=ptr_operationsModel->fieldIndex("item"))
		return;
	items_widget=new QWidget(this,Qt::Window);
	items_widget->setFixedSize(500,400);
//	items_widget->setWindowFlags(Qt::FramelessWindowHint);
	
	items_widget->setObjectName("borders_for_items_outcome");
	items_widget->setStyleSheet("QWidget#borders_for_items_outcome {" 
												"background-color: #ABE7B2; color: black;"
												"border: 1px solid #427A76; }");

	items_proxymodel=new QSortFilterProxyModel(this); //----experimental
	items_proxymodel->setSourceModel(ptr_itemsModel); //----experimental
	items_proxymodel->setFilterKeyColumn(ptr_itemsModel->fieldIndex("item_name")); //----experimental
	items_proxymodel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	items_proxymodel->setDynamicSortFilter(true); //----experimental

	items_view=new QTableView();
	items_view->setModel(items_proxymodel);
//  items_view->sortByColumn(0,Qt::AscendingOrder); /*finded out that view needed sort too*/
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
//-----------------------------------------------------------------
	items_widget_layout=new QVBoxLayout(items_widget);

	items_filter_layout=new QHBoxLayout();
	items_filter_label=new QLabel("Search item");
	items_filter_lineedit=new QLineEdit();
	items_filter_clearPB=new QPushButton("Clear filter");
	items_filter_label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	items_filter_lineedit->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	items_filter_clearPB->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	items_filter_layout->addWidget(items_filter_label);
	items_filter_layout->addWidget(items_filter_lineedit);
	items_filter_layout->addWidget(items_filter_clearPB);

	items_buttons_layout=new QHBoxLayout();
	select_itemPB=new QPushButton("Select");
	cancel_itemPB=new QPushButton("Cancel");
	items_buttons_layout->addWidget(select_itemPB);
	items_buttons_layout->addWidget(cancel_itemPB);
	
	items_widget_layout->addLayout(items_filter_layout);
	items_filter_layout->addStretch();
	items_widget_layout->addWidget(items_view);
	items_widget_layout->addLayout(items_buttons_layout);
	items_widget->show();

	connect(items_filter_clearPB,&QPushButton::clicked,this,&OutcomeDialog::slot_clear_items_filter);
	connect(items_filter_lineedit,&QLineEdit::textChanged,this,&OutcomeDialog::slot_set_items_filter);
	connect(items_view,&QTableView::doubleClicked,this,&OutcomeDialog::slot_passSelectedItem);
	connect(select_itemPB,&QPushButton::clicked,this,&OutcomeDialog::slot_passSelectedItem);
	connect(cancel_itemPB,&QPushButton::clicked,[=]() { items_widget->close(); });
}

void OutcomeDialog::slot_clear_items_filter() {
	items_proxymodel->setFilterRegularExpression("");
	items_filter_lineedit->clear();	
}

void OutcomeDialog::slot_set_items_filter() {
	items_proxymodel->setFilterWildcard(items_filter_lineedit->text());
}

void OutcomeDialog::slot_passSelectedItem() {
//---Get the index of selected item from items_view
//	QModelIndex item_index=ptr_itemsModel->index(items_view->currentIndex().row(),ptr_itemsModel->fieldIndex("item_name"));
	QModelIndex proxy_index=items_proxymodel->index(items_view->currentIndex().row(),ptr_itemsModel->fieldIndex("item_name"));
	if(!proxy_index.isValid()) {
    QMessageBox::information(nullptr,"Warning message","Nothing selected, pick an item please!");
  	return;
	}
//---Get the text from selected item
	QModelIndex source_index=items_proxymodel->mapToSource(proxy_index);
	QVariant data=ptr_itemsModel->data(source_index,Qt::DisplayRole);
	QString str=data.toString();
//---Get the index of row in operations view of our widget
	QModelIndex op_index=operations_proxymodel->index(operationsView->currentIndex().row(),ptr_operationsModel->fieldIndex("item"));
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

void OutcomeDialog::slot_add_operation() {
	int row=ptr_operationsModel->rowCount();
	ptr_operationsModel->insertRow(row);

	QDate date_qdate=date->date();
	QModelIndex tmp_date_index=ptr_operationsModel->index(row,ptr_operationsModel->fieldIndex("date"));
	QModelIndex tmp_operation_number_index=ptr_operationsModel->index(row,ptr_operationsModel->fieldIndex("operation_number"));
	QModelIndex tmp_operation_type_index=ptr_operationsModel->index(row,ptr_operationsModel->fieldIndex("operation_type"));
	QModelIndex tmp_status_index=ptr_operationsModel->index(row,ptr_operationsModel->fieldIndex("status"));
	ptr_operationsModel->setData(tmp_date_index,date_qdate,Qt::EditRole);
	ptr_operationsModel->setData(tmp_operation_number_index,op_number->text(),Qt::EditRole);
	ptr_operationsModel->setData(tmp_operation_type_index,"outcome operation",Qt::EditRole);
	ptr_operationsModel->setData(tmp_status_index,"SAVED",Qt::EditRole);

//   select the same column before adding an operation row. if no column selected then select 'cell' column
	int col=ptr_operationsModel->fieldIndex("cell");
	if(operationsView->currentIndex().isValid())
		col=operationsView->currentIndex().column();
	operationsView->setCurrentIndex(operations_proxymodel->index(operations_proxymodel->rowCount()-1,col));
}

void OutcomeDialog::slot_copy_operation() {
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
	int col=ptr_operationsModel->fieldIndex("cell");
	for( ;col!=operations_proxymodel->columnCount();++col) {
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

void OutcomeDialog::slot_remove_operation() {
//   get the index of removing row. check validness
	QModelIndex op_index=operations_proxymodel->index(operationsView->currentIndex().row(),operationsView->currentIndex().column());
	if(!op_index.isValid()) {
    QMessageBox::information(nullptr,"Warning message","Select row before deletion!");
		return;
	}
//   set that row hidden 
	operationsView->setRowHidden(op_index.row(),true); //   experimental
//   get the index of row that will be selected after deletion. if row is the last select previous, otherwise next
	QModelIndex new_index=operations_proxymodel->index(op_index.row()+1,op_index.column());
	if(op_index.row()==operations_proxymodel->rowCount()-1) //   experimental 
		new_index=operations_proxymodel->index(op_index.row()-1,op_index.column());
	operationsView->setCurrentIndex(new_index);

//   convert proxy index of removing row to source and remove row by that index
	QModelIndex correspond_index=operations_proxymodel->mapToSource(op_index);
	ptr_operationsModel->removeRow(correspond_index.row());

	emit signal_ready();
}
