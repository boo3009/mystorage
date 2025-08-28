#include "itemDialog.h"

ItemDialog::ItemDialog(QSqlTableModel *model, QTableView *view,int row,bool needCopy,
											 QWidget *parent) : QDialog(parent), ptr_itemsModel(model), 
																					ptr_itemsView(view)  {
  setup_Widget();
  setup_ModelandMapper();
  if(row==-1 && !needCopy) {
    ptr_itemsModel->insertRow(ptr_itemsModel->rowCount(QModelIndex()));
    ptr_itemsView->setRowHidden(ptr_itemsModel->rowCount()-1,true);
    mapper->toLast();
  } else if(row!=-1 && !needCopy)
      mapper->setCurrentModelIndex(ptr_itemsModel->index(row,1));
    else if(row!=-1 && needCopy) {
      ptr_itemsModel->insertRow(ptr_itemsModel->rowCount(QModelIndex()));
      /*-----i need to call some copyitem function here instead of this lines!-------*/
      QVariant var=ptr_itemsModel->data(ptr_itemsModel->index(row,1),Qt::EditRole);
      QString str=var.toString();
      QModelIndex i=ptr_itemsModel->index(ptr_itemsModel->rowCount()-1,1);
      ptr_itemsModel->setData(i,str,Qt::EditRole);
      ptr_itemsView->setRowHidden(ptr_itemsModel->rowCount()-1,true);
      mapper->toLast();
    }
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
//  connect(ptr_itemsModel,&QSqlTableModel::copySignal,this,&ItemDialog::copyItem); 
} 

void ItemDialog::setup_ModelandMapper() {
  mapper=new QDataWidgetMapper();
  mapper->setModel(ptr_itemsModel);
  mapper->addMapping(lineEdit,ptr_itemsModel->fieldIndex("item_name"));
  mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

void ItemDialog::slot_saveItem() {
  if(lineEdit->text().isEmpty()) {
    ptr_itemsModel->removeRow(ptr_itemsModel->rowCount()-1);
    this->close();
  }
  ptr_itemsView->setRowHidden(ptr_itemsModel->rowCount()-1,false);
  mapper->submit();      
  ptr_itemsModel->submitAll();
  emit signalReady();
  this->close();
}

void ItemDialog::slot_cancelItem() {
  if(lineEdit->text().isEmpty())
    ptr_itemsModel->removeRow(ptr_itemsModel->rowCount()-1);
  emit signalReady();
  this->close();
}

//void ItemDialog::copyItem() {
//  ptr_itemsModel->insertRow(ptr_itemsModel(rowCount(QModelIndex()));    
//
//}
