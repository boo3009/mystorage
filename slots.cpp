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
		index=incomeModel->index(incomeModel->rowCount(),1);
	int before=incomeModel->rowCount();
	IncomeDialog *obj_incomeDialog=new IncomeDialog(incomeModel,incomeView,itemsModel,
																									operationsModel);
  obj_incomeDialog->exec();
  int after=incomeModel->rowCount();
	if(after!=before) {
  	QModelIndex i=incomeModel->index(incomeModel->rowCount()-1,0); //get added income's qmodelindex
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
  if(!index.isValid()) {
    QMessageBox::information(nullptr,"Warning message","Please, select an income before removing!");
		return;
	}
	QModelIndex newindex;
	if(index.row()==incomeModel->rowCount()-1)
		newindex=incomeModel->index(index.row()-1,1);
	else
		newindex=index;
	incomeModel->removeRow(index.row());
	incomeModel->submitAll();
	incomeModel->select();
	incomeView->setCurrentIndex(newindex);
}

//=================================INCOMES PART END=========================================
