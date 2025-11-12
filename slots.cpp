#include "mainWindow.h"

void MainWindow::slot_updateModels() {
  itemsModel->select();
  incomeModel->select();
	operationsModel->select();
}

//=================================ITEMS PART START=========================================

void MainWindow::slot_itemDialog_add() {
  QModelIndex index=itemsView->currentIndex();
	int before=itemsModel->rowCount();
	ItemDialog *obj_itemDialog=new ItemDialog(itemsModel,itemsView);
  obj_itemDialog->exec();
  int after=itemsModel->rowCount();
	if(after!=before) {
  	QModelIndex i=itemsModel->index(itemsModel->rowCount()-1,0); //get added item's qmodelindex
		itemsView->setCurrentIndex(i); //select added row (after adding)
	} else
			itemsView->setCurrentIndex(index);
	connect(obj_itemDialog,&ItemDialog::signal_ready,this,&MainWindow::slot_updateModels);
	delete obj_itemDialog;
	obj_itemDialog=nullptr;
}

void MainWindow::slot_editItemByDoubleClick(QModelIndex index) {
  if(!index.isValid()) {
    QMessageBox::information(nullptr,"Warning message","Please, select an item before editing!");
		return;
	}
  ItemDialog *obj_itemDialog=new ItemDialog(itemsModel,itemsView,index.row());
  obj_itemDialog->setWindowTitle("Editing an item");
  obj_itemDialog->exec();
  itemsView->setCurrentIndex(index); //select edited row (after editing)
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
  ItemDialog *obj_itemDialog=new ItemDialog(itemsModel,itemsView,index.row());
  obj_itemDialog->setWindowTitle("Editing an item");
  obj_itemDialog->exec();
  itemsView->setCurrentIndex(index); //select edited row (after editing)
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
  ItemDialog *obj_itemDialog=new ItemDialog(itemsModel,itemsView,index.row(),true);
  obj_itemDialog->setWindowTitle("Copying an item");
  obj_itemDialog->exec();
  itemsView->setCurrentIndex(index); //select the same row, dont change the focus
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
	QModelIndex newindex;
	if(index.row()==itemsModel->rowCount()-1)
		newindex=itemsModel->index(index.row()-1,1);
	else
		newindex=index;
	itemsModel->removeRow(index.row());
	itemsModel->submitAll();
	itemsModel->select();
	itemsView->setCurrentIndex(newindex);
}

//=================================ITEMS PART END=========================================

//=================================INCOMES PART START=========================================

void MainWindow::slot_incomeDialog_add() {
  QModelIndex index=incomeView->currentIndex();
	if(!index.isValid())
		index=incomeModel->index(incomeModel->rowCount()-1,1);
	IncomeDialog *obj_incomeDialog=new IncomeDialog(incomeModel,incomeView,itemsModel,
																									operationsModel);
  if(obj_incomeDialog->exec()==QDialog::Accepted) {
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
  IncomeDialog *obj_incomeDialog=new IncomeDialog(incomeModel,incomeView,itemsModel,operationsModel,index.row());
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
  IncomeDialog *obj_incomeDialog=new IncomeDialog(incomeModel,incomeView,itemsModel,operationsModel,index.row());
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

//=================================INCOMES PART END=========================================

//----...




//=================================BALANCE PART START=========================================
void MainWindow::slot_insert_update() {
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);
	if(!query.exec("truncate table filled_cells")) {
			qDebug()<<"("<<__LINE__<<") "<<"error in work of 'query.exec': Cant truncate filled_cells.";
			return;
	}
//   operations columns
	int cell_column=operationsModel->fieldIndex("cell");
	int item_column=operationsModel->fieldIndex("item");
	int quantity_column=operationsModel->fieldIndex("quantity");
	int operation_type_column=operationsModel->fieldIndex("operation_type");
	int status_column=operationsModel->fieldIndex("status");
//   filled_cells columns
	int balance_cell_id_column=balanceModel->fieldIndex("cell_id");
	int balance_quantity_column=balanceModel->fieldIndex("quantity");
	for(int row=0;row!=operationsModel->rowCount();++row) {
		if(operationsModel->index(row,status_column).data().toString()=="REMOVED")
			continue;
//------------------------------------------------------------------------------------
		QString check_str=QString("select * from filled_cells where cell like '%1' and item like '%2'")
					 .arg(operationsModel->index(row,cell_column).data().toString()).arg(operationsModel->index(row,item_column).data().toString());
		if(!query.exec(check_str)) {
			qDebug()<<"("<<__LINE__<<") "<<"error in work of 'query.exec': local checking in 'while' loop.";
			return;
		}
		if(query.next()) {
			QString update_str;
			if(operationsModel->index(row,operation_type_column).data().toString()=="income operation")
				update_str=QString("update filled_cells set quantity=quantity+'%1' where cell_id like '%2'")
																	 .arg(operationsModel->index(row,quantity_column).data().toInt()).arg(query.value(balance_cell_id_column).toInt());
			else {
				if(query.value(balance_quantity_column).toInt()<operationsModel->index(row,quantity_column).data().toInt()) {
					QMessageBox::information(nullptr,"Warning message",QString("%1 %2%, %3 %4 %5").arg("Balance of the cell: ")
								.arg(operationsModel->index(row,cell_column).data().toString()).arg(operationsModel->index(row,item_column).data().toString())
								.arg(query.value(balance_quantity_column).toInt()).arg("correct quantity please."));
					return;
				}
				update_str=QString("update filled_cells set quantity=quantity-'%1' where cell_id like '%2'")
																	 .arg(operationsModel->index(row,quantity_column).data().toInt()).arg(query.value(balance_cell_id_column).toInt());
			}
			if(!query.exec(update_str)) {
				qDebug()<<"("<<__LINE__<<") "<<"Error while 'query.exec' (income): update record in not empty set in 'filled_cells'.";
				return;
			}
		} else {
//------------------------------------------------------------------------------------
			query.prepare("insert into filled_cells(cell,item,quantity) values(:c,:i,:q)");
			query.bindValue(":c",operationsModel->index(row,cell_column).data().toString());
			query.bindValue(":i",operationsModel->index(row,item_column).data().toString());
			query.bindValue(":q",operationsModel->index(row,quantity_column).data().toInt());
			if(!query.exec()) {
				qDebug()<<"("<<__LINE__<<") "<<"error in work of 'query.exec': inserting record in not empty set in 'filled_cells'.";
				return;
			}
		}
//------------------------------------------------------------------------------------
	}
	balanceModel->select();
	balanceView->setVisible(true);
}
