#ifndef INCOMEDIALOG_SENTRY_H
#define INCOMEDIALOG_SENTRY_H

#include <QDialog>
#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QIntValidator>
#include <QPushButton>
#include <QSqlQuery>
#include <QDebug>

#include "db.h"

class IncomeDialog : public QDialog {
  Q_OBJECT
public:
  explicit IncomeDialog(QSqlTableModel *model, QTableView *view,QSqlTableModel *itemsModel,int row=-1,bool copy=false,QWidget *parent=0);
private:
  QSqlTableModel *ptr_incomesModel;
  QTableView *ptr_incomesView;
  QDataWidgetMapper *mapper_income;

  QSqlTableModel *ptr_itemsModel;
  QTableView *itemsView_income;
	QWidget *itemsView_income_widget;
	QVBoxLayout *itemsView_income_widget_layout;
  QHeaderView *itemsView_income_header;
	QHBoxLayout *select_buttons_layout;
	QPushButton *selectPB;
	QPushButton *cancel_selectPB;

  QVBoxLayout *mainLayout;
  QHBoxLayout *buttonsLayout;
	QLabel *date_label;
  QDateEdit *date;
	QHBoxLayout *labels_layout;
	QHBoxLayout *item_layout;
	QLabel *cell_label;
	QLineEdit *cell;
	QLabel *item_label;
	QLineEdit *item;
	QPushButton *open_itemsListPB;
	QLabel *quantity_label;
	QLineEdit *quantity;
	QIntValidator *validator;
	QLabel *note_label;
	QLineEdit *note;
  QPushButton *save_incomePB;
  QPushButton *cancel_incomePB;
//---------main setup-----------
  void setup_Widget();
  void setup_ModelandMapper();
//---------helper functions-----	
	void func_addIncome();
	void func_editIncome(int);
	void func_copyIncome(int);
signals:
  void signal_ready();
private slots:
  void slot_saveIncome();
  void slot_cancelIncome();
  void slot_openItemsList();
  void slot_passSelectedItem();
};

#endif
