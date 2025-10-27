#ifndef INCOMEDIALOG_SENTRY_H
#define INCOMEDIALOG_SENTRY_H

#include <QDialog>
#include <QSqlTableModel>
#include <QTableView>
#include <QSortFilterProxyModel>
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
  explicit IncomeDialog(QSqlTableModel *model, QTableView *view,QSqlTableModel *itemsModel,
 					 	 QSqlTableModel *opModel,int row=-1,bool copy=false,QWidget *parent=0);
private:
  QSqlTableModel *ptr_incomesModel;
  QTableView *ptr_incomesView;
  QDataWidgetMapper *mapper_income;

  QSqlTableModel *ptr_itemsModel_income;
	QWidget *items_income_widget;
  QTableView *items_income_view;
	QHeaderView *items_income_view_header;
	QVBoxLayout *items_income_widget_layout;
	QHBoxLayout *items_select_buttons_layout;
	QPushButton *select_itemPB;
	QPushButton *cancel_itemPB;

	QSqlTableModel *ptr_operationsModel;
	QTableView *operationsView_income;
	QHeaderView *operationsViewHeader_income;
	QSortFilterProxyModel *operationsProxyModel_income;

  QVBoxLayout *mainLayout;
  QHBoxLayout *buttonsLayout;
	QLabel *op_number_label;
	QLineEdit *op_number;
	QLabel *date_label;
  QDateEdit *date;
	QHBoxLayout *labels_layout;
	QHBoxLayout *item_layout;
	QLabel *cell_label;
	QLineEdit *cell;
	QLabel *item_label;
	QLineEdit *item;
	QPushButton *open_itemsListPB;
	QLabel *sum_label;
	QLineEdit *sum;
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
  void slot_open_itemsList(QModelIndex);
  void slot_passSelectedItem();
};

#endif
