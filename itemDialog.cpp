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
	qDebug()<<"the row count is:"<<ptr_itemsModel->rowCount(QModelIndex());
	ptr_itemsView->setRowHidden(ptr_itemsModel->rowCount()-1,true);
	mapper->toLast();
}

void ItemDialog::func_editItem(int row) {
	mapper->setCurrentModelIndex(ptr_itemsModel->index(row,1));
}

void ItemDialog::func_copyItem(int row) {
	ptr_itemsModel->insertRow(ptr_itemsModel->rowCount(QModelIndex()));
	QVariant var=ptr_itemsModel->data(ptr_itemsModel->index(row,1),Qt::EditRole);
	QString str=var.toString();
	QModelIndex i=ptr_itemsModel->index(ptr_itemsModel->rowCount()-1,1);
	ptr_itemsModel->setData(i,str,Qt::EditRole);
	ptr_itemsView->setRowHidden(ptr_itemsModel->rowCount()-1,true);
	mapper->toLast();
}

void ItemDialog::func_removeItem(int row) {
//--------need to be filled---------	
}

void ItemDialog::slot_saveItem() {
  if(lineEdit->text().isEmpty()) {
    ptr_itemsModel->removeRow(ptr_itemsModel->rowCount()-1);
    this->close();
  }
  ptr_itemsView->setRowHidden(ptr_itemsModel->rowCount()-1,false);
  mapper->submit();      
  ptr_itemsModel->submitAll();
  emit signal_ready();
  this->close();
}

void ItemDialog::slot_cancelItem() {
  if(lineEdit->text().isEmpty())
    ptr_itemsModel->removeRow(ptr_itemsModel->rowCount()-1);
  emit signal_ready();
  this->close();
}
