#ifndef ITEMDIALOG_SENTRY_H
#define ITEMDIALOG_SENTRY_H

#include <QDialog>
#include <QSqlTableModel>
#include <QTableView>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlQuery>
#include <QDebug>

#include "db.h"

class ItemDialog : public QDialog {
  Q_OBJECT
public:
  explicit ItemDialog(QSqlTableModel *model, QTableView *view,int row=-1,bool copy=false,QWidget *parent=0);
private:
  QSqlTableModel *ptr_itemsModel;
  QTableView *ptr_itemsView;
  QDataWidgetMapper *mapper;
  
  QVBoxLayout *mainLayout;
  QHBoxLayout *buttonsLayout;
  QLabel *label;
  QLineEdit *lineEdit;
  QPushButton *savePB;
  QPushButton *cancelPB;
//---------main setup-----------
  void setup_Widget();
  void setup_ModelandMapper();
//---------helper functions-----	
	void func_addItem();
	void func_editItem(int);
	void func_copyItem(int);
	bool func_isItemRepeated(QString);
signals:
  void signal_ready();
private slots:
  void slot_saveItem();
  void slot_cancelItem();
};

#endif
