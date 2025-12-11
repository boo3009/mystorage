#include "itemDialog.h"

ItemDialog::ItemDialog(QSqlTableModel *model,QTableView *view,QSortFilterProxyModel *proxy,
	int row,bool copy,QWidget *parent) : QDialog(parent), ptr_itemsModel(model), 
	ptr_itemsView(view), ptr_proxymodel(proxy)
{
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
  setFixedSize(500,100);
//	this->setWindowFlags(Qt::FramelessWindowHint);
//	this->setWindowModality(Qt::ApplicationModal);
	this->setObjectName("bordered_item_widget");
	this->setStyleSheet("QWidget#bordered_item_widget {"
											"border: 1px solid #4A70A9; background-color: #8FABD4; }");
	this->setAutoFillBackground(true);

  mainLayout=new QVBoxLayout(this);
  buttonsLayout=new QHBoxLayout();
  label=new QLabel("Item name");
  lineEdit=new QLineEdit();
	lineEdit->setStyleSheet("background-color: white;");
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
  mapper->setModel(ptr_proxymodel);
  mapper->addMapping(lineEdit,ptr_itemsModel->fieldIndex("item_name"));
  mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

void ItemDialog::func_addItem() {
	row_added=true;
	ptr_itemsModel->insertRow(ptr_itemsModel->rowCount());
	QModelIndex index=
		ptr_proxymodel->mapFromSource(ptr_itemsModel->index(ptr_itemsModel->rowCount()-1,1));
	ptr_itemsView->setRowHidden(index.row(),true);
	mapper->setCurrentModelIndex(index);
}

void ItemDialog::func_editItem(int row) {
	row_added=false;
	mapper->setCurrentModelIndex(ptr_proxymodel->index(row,1));
}

void ItemDialog::func_copyItem(int row) {
//   add row
	row_added=true;
	ptr_itemsModel->insertRow(ptr_itemsModel->rowCount());

//   convert index of the item that we want to copy from proxy space to source space. 
//   then get string by that index
	QModelIndex source_index=
		ptr_proxymodel->mapToSource(ptr_proxymodel->index(row,1));
	QString str=ptr_itemsModel->data(source_index,Qt::EditRole).toString();

//   get added rows index in source space and write string into it
	QModelIndex index=ptr_itemsModel->index(ptr_itemsModel->rowCount()-1,1);
	ptr_itemsModel->setData(index,str,Qt::EditRole);

//   convert index from source space to proxy space, unhide corresponding row and set mapper to it
	QModelIndex proxy_index=ptr_proxymodel->mapFromSource(index);
	ptr_itemsView->setRowHidden(proxy_index.row(),true);
	mapper->setCurrentModelIndex(proxy_index);
}

void ItemDialog::slot_saveItem() {
  if(lineEdit->text().isEmpty()) {
    QMessageBox::information(nullptr,"Warning message","Empty line, fill it out please!");
  	return;
	}
	if(func_isItemRepeated(lineEdit->text()) && row_added) {
    QMessageBox::information(nullptr,"Warning message","Item with that name already exists,insert the new one!");
		return;
	}
	if(row_added) {
		QModelIndex proxy_index=
			ptr_proxymodel->mapFromSource(ptr_itemsModel->index(ptr_itemsModel->rowCount()-1,1));
		ptr_itemsView->setRowHidden(proxy_index.row(),false);
	}
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
  if(row_added)
    ptr_itemsModel->removeRow(ptr_itemsModel->rowCount()-1);
  emit signal_ready();
  this->close();
}
