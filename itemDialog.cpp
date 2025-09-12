#include "itemDialog.h"

ItemDialog::ItemDialog(QSqlTableModel *model, QTableView *view,int row,bool copy,QWidget *parent) : 
/*-------------------*/QDialog(parent), ptr_itemsModel(model), ptr_itemsView(view)  {
  setup_Widget();
  setup_ModelandMapper();
  if(row==-1)
  	func_addItem();
	else if(row!=-1 && !copy)
		func_editItem(row);
	else if(copy)
		func_copyItem(row);
}

void ItemDialog::setup_Widget() {
  setWindowTitle("Creating new item");
  setFixedSize(500,100);

  mainLayout=new QVBoxLayout(this);
  buttonsLayout=new QHBoxLayout();
  label=new QLabel("Item name");
  lineEdit=new QLineEdit();
  savePB=new QPushButton("Save");
  cancelPB=new QPushButton("Cancel");
  
  label->setBuddy(lineEdit);
  mainLayout->addWidget(label);
  mainLayout->addWidget(lineEdit);
  buttonsLayout->addWidget(savePB);
  buttonsLayout->addWidget(cancelPB);
  mainLayout->addLayout(buttonsLayout); 
  
  connect(savePB,&QPushButton::clicked,this,&ItemDialog::slot_saveItem); 
  connect(cancelPB,&QPushButton::clicked,this,&ItemDialog::slot_cancelItem); 
} 

void ItemDialog::setup_ModelandMapper() {
  mapper=new QDataWidgetMapper();
  mapper->setModel(ptr_itemsModel);
  mapper->addMapping(lineEdit,ptr_itemsModel->fieldIndex("item_name"));
  mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

void ItemDialog::func_addItem() {
	ptr_itemsModel->insertRow(ptr_itemsModel->rowCount(QModelIndex()));
	ptr_itemsView->setRowHidden(ptr_itemsModel->rowCount()-1,true);
	mapper->toLast();
}

void ItemDialog::func_editItem(int row) {
	mapper->setCurrentModelIndex(ptr_itemsModel->index(row,1));
}

void ItemDialog::func_copyItem(int row) {
	ptr_itemsModel->insertRow(ptr_itemsModel->rowCount(QModelIndex()));
	QString str=ptr_itemsModel->data(ptr_itemsModel->index(row,1),Qt::EditRole).toString();
	QModelIndex index=ptr_itemsModel->index(ptr_itemsModel->rowCount()-1,1);
	ptr_itemsModel->setData(index,str,Qt::EditRole);
	ptr_itemsView->setRowHidden(ptr_itemsModel->rowCount()-1,true);
	mapper->toLast();
}

void ItemDialog::slot_saveItem() {
  if(lineEdit->text().isEmpty()) {
    QMessageBox::information(nullptr,"Warning message","Empty line, fill it out please!");
  	return;
	}
	if(func_isItemRepeated(lineEdit->text())) {
    QMessageBox::information(nullptr,"Warning message","Item with that name already exists,insert the new one!");
		return;
	}
  ptr_itemsView->setRowHidden(ptr_itemsModel->rowCount()-1,false);
  mapper->submit();      
  ptr_itemsModel->submitAll();
  emit signal_ready();
  this->close();
}

bool ItemDialog::func_isItemRepeated(QString name) {
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);
	QString str=QString("select item_name from items");
	query.prepare(str);
	query.exec();
	while(query.next()) {
		QString tmp=query.value(0).toString();
		if(tmp==name)
			return true;
	}
	return false;
}

void ItemDialog::slot_cancelItem() {
  if(lineEdit->text().isEmpty())
    ptr_itemsModel->removeRow(ptr_itemsModel->rowCount()-1);
  emit signal_ready();
  this->close();
}
