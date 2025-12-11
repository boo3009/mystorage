#include "mainWindow.h"


void MainWindow::slot_updateModels() {
  itemsModel->select();
  incomeModel->select();
  outcomeModel->select();
	operationsModel->select();
	balanceModel->select();
}

void MainWindow::set_selected_button_dark(QPushButton *pb1,QPushButton *pb2, QPushButton *pb3,QPushButton *button_selected) {
	pb1->setStyleSheet("background-color: #9ECFD4; text-align:left;");
	pb2->setStyleSheet("background-color: #9ECFD4; text-align:left;");
	pb3->setStyleSheet("background-color: #9ECFD4; text-align:left;");
	button_selected->setStyleSheet("background-color: #336D82; text-align:left;");
}

//=================================ITEMS PART START=========================================

void MainWindow::slot_itemDialog_add() {
//   get current index of row if one is selected.
  QModelIndex index=itemsView->currentIndex();

//   clear the filter by passing empty string and clear filter line
	items_proxymodel->setFilterRegularExpression("");
	items_filter_lineedit->clear();
	int before=itemsModel->rowCount();

/*   create dialog and execute it   */
	ItemDialog *obj_itemDialog=new ItemDialog(itemsModel,itemsView,items_proxymodel);
  obj_itemDialog->setWindowTitle("Adding an item");
  obj_itemDialog->exec();

/*   check if row added, then select it if so. Otherwise select the old one   */
	if(itemsModel->rowCount()>before)
		index=items_proxymodel->index(items_proxymodel->rowCount()-1,1);
	itemsView->setCurrentIndex(index);

/*   connect signal and delete dialog from heap, also set pointer to it as nullptr   */
	connect(obj_itemDialog,&ItemDialog::signal_ready,this,&MainWindow::slot_updateModels);
	delete obj_itemDialog;
	obj_itemDialog=nullptr;
}

void MainWindow::slot_editItemByDoubleClick(QModelIndex index) {
  if(!index.isValid()) {
    QMessageBox::information(nullptr,"Warning message","Please, select an item before editing!");
		return;
	}
  ItemDialog *obj_itemDialog=new ItemDialog(itemsModel,itemsView,items_proxymodel,index.row());
  obj_itemDialog->setWindowTitle("Editing an item");
  obj_itemDialog->exec();
  itemsView->setCurrentIndex(items_proxymodel->index(index.row(),1)); //select edited row (after editing)

	connect(obj_itemDialog,&ItemDialog::signal_ready,this,&MainWindow::slot_updateModels);
	delete obj_itemDialog;
	obj_itemDialog=nullptr;
}

void MainWindow::slot_itemDialog_edit() {
  QModelIndex index=itemsView->currentIndex(); 
  if(!index.isValid()) {
    QMessageBox::information(nullptr,"Warning message","Please, select an item before editing!");
		return;
	}
  ItemDialog *obj_itemDialog=new ItemDialog(itemsModel,itemsView,items_proxymodel,index.row());
  obj_itemDialog->setWindowTitle("Editing an item");
  obj_itemDialog->exec();
  itemsView->setCurrentIndex(items_proxymodel->index(index.row(),1)); //select edited row (after editing)

	connect(obj_itemDialog,&ItemDialog::signal_ready,this,&MainWindow::slot_updateModels);
	delete obj_itemDialog;
	obj_itemDialog=nullptr;
}

void MainWindow::slot_itemDialog_copy() {
  QModelIndex index=itemsView->currentIndex(); 
  if(!index.isValid()) {
    QMessageBox::information(nullptr,"Warning message","Please, select an item before copying!");
		return;
	}
  ItemDialog *obj_itemDialog=new ItemDialog(itemsModel,itemsView,items_proxymodel,index.row(),true);
  obj_itemDialog->setWindowTitle("Copying an item");
  obj_itemDialog->exec();
  itemsView->setCurrentIndex(items_proxymodel->index(index.row(),1)); //select the same row, dont change the focus

	connect(obj_itemDialog,&ItemDialog::signal_ready,this,&MainWindow::slot_updateModels);
	delete obj_itemDialog;
	obj_itemDialog=nullptr;
}

void MainWindow::slot_itemsModelView_remove() {
	QModelIndex index=itemsView->currentIndex();
  if(!index.isValid()) {
    QMessageBox::information(nullptr,"Warning message","Please, select an item before removing!");
		return;
	}

	bool bottom_row= (index.row()==items_proxymodel->rowCount()-1) ? true : false;

	QModelIndex source_index=items_proxymodel->mapToSource(index);
	itemsModel->removeRow(source_index.row());
	itemsModel->submitAll();
	itemsModel->select();

	if(!items_proxymodel->rowCount()) {
		items_proxymodel->setFilterRegExp("");	
		items_filter_lineedit->clear();	
		return;
	}

	index=items_proxymodel->index(index.row()-bottom_row,1);
	itemsView->setCurrentIndex(index);
}

void MainWindow::slot_set_items_filter() { //----experimental
	items_proxymodel->setFilterWildcard(items_filter_lineedit->text());
}

void MainWindow::slot_clear_items_filter() {
	items_proxymodel->setFilterRegularExpression("");
	items_filter_lineedit->clear();	
}

//=================================INCOMES PART START=========================================

void MainWindow::slot_incomeDialog_add() {
	int before=incomeModel->rowCount();
  QModelIndex index=incomeView->currentIndex();
	if(!index.isValid())
		index=incomeModel->index(0,incomeModel->fieldIndex("operation_number"));
	IncomeDialog *obj_incomeDialog=new IncomeDialog(incomeModel,incomeView,itemsModel,
																									operationsModel,balanceModel);
	obj_incomeDialog->exec();
	int after=incomeModel->rowCount();
  if(after!=before) {
  	QModelIndex i=incomeModel->index(incomeModel->rowCount()-1,1); //get added income's qmodelindex
		incomeView->setCurrentIndex(i); //select added row (after adding)
	} else
		incomeView->setCurrentIndex(index);
	connect(obj_incomeDialog,&IncomeDialog::signal_ready,this,&MainWindow::slot_updateModels);
	delete obj_incomeDialog;
	obj_incomeDialog=nullptr;
}

void MainWindow::slot_editIncomeByDoubleClick(QModelIndex index) {
  if(!index.isValid()) {
    QMessageBox::information(nullptr,"Warning message","Please, select an income before editing!");
		return;
	}
  IncomeDialog *obj_incomeDialog=new IncomeDialog(incomeModel,incomeView,itemsModel,
		operationsModel,balanceModel,index.row());
  obj_incomeDialog->setWindowTitle("Editing an income");
  obj_incomeDialog->exec();
  incomeView->setCurrentIndex(index); //select edited row (after editing)
	connect(obj_incomeDialog,&IncomeDialog::signal_ready,this,&MainWindow::slot_updateModels);
	delete obj_incomeDialog;
	obj_incomeDialog=nullptr;
}

void MainWindow::slot_incomeDialog_edit() {
  QModelIndex index=incomeView->currentIndex(); 
  if(!index.isValid()) {
    QMessageBox::information(nullptr,"Warning message","Please, select an income before editing!");
		return;
	}
  IncomeDialog *obj_incomeDialog=new IncomeDialog(incomeModel,incomeView,itemsModel,
		operationsModel,balanceModel,index.row());
  obj_incomeDialog->setWindowTitle("Editing an income");
  obj_incomeDialog->exec();
  incomeView->setCurrentIndex(index); //select edited row (after editing)
	connect(obj_incomeDialog,&IncomeDialog::signal_ready,this,&MainWindow::slot_updateModels);
	delete obj_incomeDialog;
	obj_incomeDialog=nullptr;
}

void MainWindow::slot_incomeModelView_remove() {
	QModelIndex index=incomeView->currentIndex();
  if(!index.isValid() || incomeModel->index(index.row(),incomeModel->fieldIndex("status")).data().toString()=="REMOVED") {
    QMessageBox::information(nullptr,"Warning message","Income not selected or has status REMOVED!");
		return;
	}
	index=incomeModel->index(index.row(),incomeModel->fieldIndex("status"));
	incomeModel->setData(index,"REMOVED",Qt::EditRole);
	incomeModel->submitAll();
	incomeModel->select();
	
	QString num=incomeModel->index(index.row(),incomeModel->fieldIndex("operation_number")).data().toString();
	QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);
	QString str=QString("select * from operations where operation_number like '%1'").arg(num);
	if(!query.exec(str)) {
		qDebug()<<__LINE__<<"error in work of 'query.exec': cant set status to REMOVED to operation.";
		qDebug()<<"error type"<<query.lastError().type();
		qDebug()<<"error text"<<query.lastError().text();
		qDebug()<<"driver error"<<query.lastError().driverText();
		qDebug()<<"database error"<<query.lastError().databaseText();
		return;
	}
	QSqlQuery upd(retrieveDB);
	while(query.next())	{
		QString upd_str=QString("update operations set status='%1' where operation_id like %2").arg("REMOVED").arg(query.value(0).toInt());
		if(!upd.exec(upd_str)) {
			qDebug()<<__LINE__<<"error in work of 'query.exec': cant set status to REMOVED to operation.";
			qDebug()<<"error type"<<query.lastError().type();
			qDebug()<<"error text"<<query.lastError().text();
			qDebug()<<"driver error"<<query.lastError().driverText();
			qDebug()<<"database error"<<query.lastError().databaseText();
		}
	}
	operationsModel->submitAll();
	operationsModel->select();
	incomeView->setCurrentIndex(index);
}

void MainWindow::slot_incomeModelView_cancel_remove() {
	QModelIndex index=incomeView->currentIndex();
  if(!index.isValid() || incomeModel->index(index.row(),incomeModel->fieldIndex("status")).data().toString()=="SAVED") {
    QMessageBox::information(nullptr,"Warning message","Income not selected or has status SAVED!");
		return;
	}
	index=incomeModel->index(index.row(),incomeModel->fieldIndex("status"));
	incomeModel->setData(index,"SAVED",Qt::EditRole);
	incomeModel->submitAll();
	incomeModel->select();
	
	QString num=incomeModel->index(index.row(),incomeModel->fieldIndex("operation_number")).data().toString();
	QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);
	QString str=QString("select * from operations where operation_number like '%1'").arg(num);
	if(!query.exec(str)) {
		qDebug()<<__LINE__<<"error in work of 'query.exec': cant set status to REMOVED to operation.";
		qDebug()<<"error type"<<query.lastError().type();
		qDebug()<<"error text"<<query.lastError().text();
		qDebug()<<"driver error"<<query.lastError().driverText();
		qDebug()<<"database error"<<query.lastError().databaseText();
		return;
	}
	QSqlQuery upd(retrieveDB);
	while(query.next())	{
		QString upd_str=QString("update operations set status='%1' where operation_id like %2").arg("SAVED").arg(query.value(0).toInt());
		if(!upd.exec(upd_str)) {
			qDebug()<<__LINE__<<"error in work of 'query.exec': cant set status to REMOVED to operation.";
			qDebug()<<"error type"<<query.lastError().type();
			qDebug()<<"error text"<<query.lastError().text();
			qDebug()<<"driver error"<<query.lastError().driverText();
			qDebug()<<"database error"<<query.lastError().databaseText();
		}
	}
	operationsModel->submitAll();
	operationsModel->select();
	incomeView->setCurrentIndex(index);
}

//=================================OUTCOMES PART START=========================================

void MainWindow::slot_outcomeDialog_add() {
	int before=outcomeModel->rowCount();
  QModelIndex index=outcomeView->currentIndex();
	if(!index.isValid())
		index=outcomeModel->index(0,outcomeModel->fieldIndex("operation_number"));
//---call a function, to recalculate operations from operations table, so we get actual balance BEFORE outcome. We dont need to do the same for incomes, since there is nothing to check.
	if(slot_generate_balance()==-1)
		return;
	OutcomeDialog *obj_outcomeDialog=new OutcomeDialog(outcomeModel,outcomeView,itemsModel,
																									operationsModel,balanceModel);
  obj_outcomeDialog->exec();
	int after=outcomeModel->rowCount();
  if(after!=before) {
  	QModelIndex i=outcomeModel->index(outcomeModel->rowCount()-1,1); //get added outcome's qmodelindex
		outcomeView->setCurrentIndex(i); //select added row (after adding)
	} else
		outcomeView->setCurrentIndex(index);
	connect(obj_outcomeDialog,&OutcomeDialog::signal_ready,this,&MainWindow::slot_updateModels);
	delete obj_outcomeDialog;
	obj_outcomeDialog=nullptr;
}

void MainWindow::slot_editOutcomeByDoubleClick(QModelIndex index) {
  if(!index.isValid()) {
    QMessageBox::information(nullptr,"Warning message","Please, select an outcome before editing!");
		return;
	}
	if(slot_generate_balance()==-1)
		return;
  OutcomeDialog *obj_outcomeDialog=new OutcomeDialog(outcomeModel,outcomeView,itemsModel,operationsModel,balanceModel,index.row());
  obj_outcomeDialog->setWindowTitle("Editing an outcome");
  obj_outcomeDialog->exec();
  outcomeView->setCurrentIndex(index); //select edited row (after editing)
	connect(obj_outcomeDialog,&OutcomeDialog::signal_ready,this,&MainWindow::slot_updateModels);
	delete obj_outcomeDialog;
	obj_outcomeDialog=nullptr;
}

void MainWindow::slot_outcomeDialog_edit() {
  QModelIndex index=outcomeView->currentIndex(); 
  if(!index.isValid()) {
    QMessageBox::information(nullptr,"Warning message","Please, select an outcome before editing!");
		return;
	}
	if(slot_generate_balance()==-1)
		return;
  OutcomeDialog *obj_outcomeDialog=new OutcomeDialog(outcomeModel,outcomeView,itemsModel,operationsModel,balanceModel,index.row());
  obj_outcomeDialog->setWindowTitle("Editing an outcome");
  obj_outcomeDialog->exec();
  outcomeView->setCurrentIndex(index); //select edited row (after editing)
	connect(obj_outcomeDialog,&OutcomeDialog::signal_ready,this,&MainWindow::slot_updateModels);
	delete obj_outcomeDialog;
	obj_outcomeDialog=nullptr;
}

void MainWindow::slot_outcomeModelView_remove() {
	QModelIndex index=outcomeView->currentIndex();
  if(!index.isValid() || outcomeModel->index(index.row(),outcomeModel->fieldIndex("status")).data().toString()=="REMOVED") {
    QMessageBox::information(nullptr,"Warning message","Outcome not selected or has status REMOVED!");
		return;
	}
	index=outcomeModel->index(index.row(),outcomeModel->fieldIndex("status"));
	outcomeModel->setData(index,"REMOVED",Qt::EditRole);
	outcomeModel->submitAll();
	outcomeModel->select();
	
	QString num=outcomeModel->index(index.row(),outcomeModel->fieldIndex("operation_number")).data().toString();
	QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);
	QString str=QString("select * from operations where operation_number like '%1'").arg(num);
	if(!query.exec(str)) {
		qDebug()<<__LINE__<<"error in work of 'query.exec': cant set status to REMOVED to operation.";
		qDebug()<<"error type"<<query.lastError().type();
		qDebug()<<"error text"<<query.lastError().text();
		qDebug()<<"driver error"<<query.lastError().driverText();
		qDebug()<<"database error"<<query.lastError().databaseText();
		return;
	}
	QSqlQuery upd(retrieveDB);
	while(query.next())	{
		QString upd_str=QString("update operations set status='%1' where operation_id like %2").arg("REMOVED").arg(query.value(0).toInt());
		if(!upd.exec(upd_str)) {
			qDebug()<<__LINE__<<"error in work of 'query.exec': cant set status to REMOVED to operation.";
			qDebug()<<"error type"<<query.lastError().type();
			qDebug()<<"error text"<<query.lastError().text();
			qDebug()<<"driver error"<<query.lastError().driverText();
			qDebug()<<"database error"<<query.lastError().databaseText();
		}
	}
	operationsModel->submitAll();
	operationsModel->select();
	outcomeView->setCurrentIndex(index);
}

void MainWindow::slot_outcomeModelView_cancel_remove() {
	QModelIndex index=outcomeView->currentIndex();
  if(!index.isValid() || outcomeModel->index(index.row(),outcomeModel->fieldIndex("status")).data().toString()=="SAVED") {
    QMessageBox::information(nullptr,"Warning message","Outcome not selected or has status SAVED!");
		return;
	}
	index=outcomeModel->index(index.row(),outcomeModel->fieldIndex("status"));
	outcomeModel->setData(index,"SAVED",Qt::EditRole);
	outcomeModel->submitAll();
	outcomeModel->select();
	
	QString num=outcomeModel->index(index.row(),outcomeModel->fieldIndex("operation_number")).data().toString();
	QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);
	QString str=QString("select * from operations where operation_number like '%1'").arg(num);
	if(!query.exec(str)) {
		qDebug()<<__LINE__<<"error in work of 'query.exec': cant set status to REMOVED to operation.";
		qDebug()<<"error type"<<query.lastError().type();
		qDebug()<<"error text"<<query.lastError().text();
		qDebug()<<"driver error"<<query.lastError().driverText();
		qDebug()<<"database error"<<query.lastError().databaseText();
		return;
	}
	QSqlQuery upd(retrieveDB);
	while(query.next())	{
		QString upd_str=QString("update operations set status='%1' where operation_id like %2").arg("SAVED").arg(query.value(0).toInt());
		if(!upd.exec(upd_str)) {
			qDebug()<<__LINE__<<"error in work of 'query.exec': cant set status to REMOVED to operation.";
			qDebug()<<"error type"<<query.lastError().type();
			qDebug()<<"error text"<<query.lastError().text();
			qDebug()<<"driver error"<<query.lastError().driverText();
			qDebug()<<"database error"<<query.lastError().databaseText();
		}
	}
	operationsModel->submitAll();
	operationsModel->select();
	outcomeView->setCurrentIndex(index);
}

//=================================BALANCE PART START=========================================

void MainWindow::slot_call_generate_balance() {
	if(slot_generate_balance()==-1)
		qDebug()<<__LINE__<<"Error: Cant generate balance,soryan :)";
	balanceView->setVisible(true);
}

int MainWindow::slot_generate_balance() {
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);
	if(!query.exec("truncate table filled_cells")) {
			qDebug()<<"("<<__LINE__<<") "<<"error in work of 'query.exec': Cant truncate filled_cells.";
			return -1;
	}
//   operations columns
	int cell_column=operationsModel->fieldIndex("cell");
	int item_column=operationsModel->fieldIndex("item");
	int quantity_column=operationsModel->fieldIndex("quantity");
	int operation_type_column=operationsModel->fieldIndex("operation_type");
//   filled_cells columns
	int balance_cell_id_column=balanceModel->fieldIndex("cell_id");
//------------------------------------------------------------------------------------
	if(!query.exec("select * from operations where status like 'SAVED'")) {
		qDebug()<<"("<<__LINE__<<") "<<"error in work of 'query.exec': selecting from operations.";
		return -1;
	}
	while(query.next()) {
		QSqlQuery subquery(retrieveDB);
		QString check_str=QString("select * from filled_cells where cell like '%1' and item like '%2'")
		  .arg(query.value(cell_column).toString())
			.arg(query.value(item_column).toString());
		if(!subquery.exec(check_str)) {
			qDebug()<<"("<<__LINE__<<") "<<"error in work of 'subquery.exec': local checking in 'while' loop.";
			return -1;
		}
//---result set is NOT empty, so we can change quantity in income or outcome.
		if(subquery.next()) {
		QString update_str;
		if(query.value(operation_type_column).toString()=="income operation")
			update_str=QString("update filled_cells set quantity=quantity+'%1' where cell_id like '%2'")
				.arg(query.value(quantity_column).toInt())
				.arg(subquery.value(balance_cell_id_column).toInt());
		else
			update_str=QString("update filled_cells set quantity=quantity-'%1' where cell_id like '%2'")
				.arg(query.value(quantity_column).toInt())
				.arg(subquery.value(balance_cell_id_column).toInt());
		if(!subquery.exec(update_str)) {
			qDebug()<<"("<<__LINE__<<") "<<"Error while 'subquery.exec' (income): update record in not empty set in 'filled_cells'.";
			return -1;
		}
		continue;
		}
//---result set is empty, so we have nothing in that cell, lets ADD an income or minus outcome.
		subquery.prepare("insert into filled_cells(cell,item,quantity) values(:c,:i,:q)");
		subquery.bindValue(":c",query.value(cell_column).toString());
		subquery.bindValue(":i",query.value(item_column).toString());
		if(query.value(operation_type_column).toString()=="income operation")
			subquery.bindValue(":q",query.value(quantity_column).toInt());
		else
			subquery.bindValue(":q",-query.value(quantity_column).toInt());
		if(!subquery.exec()) {
			qDebug()<<"("<<__LINE__<<") "<<"error in work of 'subquery.exec': inserting record in not empty set in 'filled_cells'.";
			return -1;
		}
	}
//------------------------------------------------------------------------------------
	if(!query.exec("select count(distinct cell) from filled_cells"))
		qDebug()<<"("<<__LINE__<<") "<<"error in work of 'query.exec': Cant count distinct cells from filled_cells.";
	if(query.next())
		non_empty_cells_lineedit->setText(QString::number(query.value(0).toInt()));

	if(!query.exec("select sum(quantity) from filled_cells"))
		qDebug()<<"("<<__LINE__<<") "<<"error in work of 'query.exec': Cant count sum of quantities from filled_cells.";
	if(query.next())
		pieces_lineedit->setText(QString::number(query.value(0).toInt()));
//------------------------------------------------------------------------------------
	balanceModel->select();
	return 0;
}

void MainWindow::slot_write_balance_into_file() {
	if(balanceModel->rowCount()==0 || !balanceView->isVisible()) {
    QMessageBox::information(nullptr,"Informational message","Balance table is empty, nothing to print out.");
		return;
	}
//   some enums
	enum { QUANTITY_MAX_LENGTH=6, LINES_OFSET=9 };
//   lets get width of column in pixels and convert it to length of chars that column can contain
	QFontMetrics fm(balanceView->font());
	int char_width=fm.averageCharWidth();
	int item_width_pixels=balanceView->columnWidth(balanceModel->fieldIndex("item"));
	if(!char_width)
		return;
	int item_width=item_width_pixels/char_width+2;
	int cell_width=balanceView->columnWidth(balanceModel->fieldIndex("cell"))/char_width;
//   now we can have '+------------+' line
	int total_row_width=cell_width+item_width+QUANTITY_MAX_LENGTH+LINES_OFSET;
	QString lines='+'+QString('-').repeated(total_row_width-2)+'+';
//   some short names for fields indexes as an integers
	int cell_column=balanceModel->fieldIndex("cell");
	int quantity_column=balanceModel->fieldIndex("quantity");
//   open file and filestream
	QString filename="print.txt";
	QFile file(filename);
	if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug()<<"Cant open or create file: 'print.txt'";
		return;
	}
	QTextStream file_stream(&file);
//   at this point lets define logic of writing information into file
	QDateTime cur_date_time=QDateTime::currentDateTime();
	QString cur_date_time_formated=cur_date_time.toString("dd.MM.yyyy, hh:mm:ss");
	file_stream<<"| "<<cur_date_time_formated <<" |"<<Qt::endl; 
	file_stream<< lines <<Qt::endl;
	for(int row=0;row!=balanceModel->rowCount();++row) {
//---------------------------------------------------------------------------------------------------------------------------------------
		for(int col=cell_column; col!=balanceModel->columnCount();++col) {
			file_stream<<"| ";
//   for quantity column
			if(col==quantity_column) {
				int num=balanceModel->index(row,col).data().toInt();
				QString num_str=QString::number(num);
				int num_len=num_str.length();
				QString num_sp=QString(' ').repeated(QUANTITY_MAX_LENGTH-num_len);
				file_stream<< num << num_sp <<" ";
				continue;
			}
//   for cell column
			QString str=balanceModel->index(row,col).data().toString();
			if(col==cell_column) {
				file_stream<< str <<" ";
				continue;
			}
//   for item column
			int len=str.length();
			if(len>item_width) {
				QString left_part=str.left(item_width);
				file_stream<< left_part <<" ";
				continue;
			}
			file_stream<< str;
			str=QString(' ').repeated(item_width-len);
			file_stream<< str <<" ";
		}
//---------------------------------------------------------------------------------------------------------------------------------------	
		file_stream<<"|"<<Qt::endl;
		file_stream<< lines <<Qt::endl;
	}
//---------------------------------------------------------------------------------------------------------------------------------------
	file_stream<<""<<Qt::endl;
	file_stream<<"Pieces:  "<<pieces_lineedit->text()<<Qt::endl;
	file_stream<<"Non-empty cells:  "<<non_empty_cells_lineedit->text()<<Qt::endl;
//---------------------------------------------------------------------------------------------------------------------------------------
	QMessageBox::information(nullptr,"Informational message",QString("%1 %2").arg("Done buddy. File created: ").arg(filename));
	file.close();
}
