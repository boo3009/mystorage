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
#include <QShortcut>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

#include "db.h"
#include "proxy_models.h"

class IncomeDialog : public QDialog {
  Q_OBJECT
public:
  explicit IncomeDialog(QSqlTableModel *model, QTableView *view,QSqlTableModel *itemsModel,
	  QSqlTableModel *opModel,QSqlTableModel *bal_model,int row=-1,QWidget *parent=0);
private:
//-------pointers to constructors parameters
  QSqlTableModel *ptr_incomesModel;
  QTableView *ptr_incomesView;
  QSqlTableModel *ptr_itemsModel;
	QSqlTableModel *ptr_operationsModel;
	QSqlTableModel *ptr_balanceModel;
//-------pointer to mapper
  QDataWidgetMapper *mapper;
//-------selecting items part-----------		
	QWidget *items_widget;
	QSortFilterProxyModel *items_proxymodel;
  QTableView *items_view;
	QHeaderView *items_view_header;
	QVBoxLayout *items_widget_layout;
	QHBoxLayout *items_filter_layout;
	QLabel *items_filter_label;
	QLineEdit *items_filter_lineedit;
	QPushButton *items_filter_clearPB;
	QHBoxLayout *items_buttons_layout;
	QPushButton *select_itemPB;
	QPushButton *cancel_itemPB;
//-------operations view part-----------		
	QTableView *operationsView;
	QHeaderView *operationsView_header;
	Proxy_op_number *operations_proxymodel;
	QPushButton *operations_addPB;
	QPushButton *operations_copyPB;
	QPushButton *operations_removePB;
	QHBoxLayout *operations_buttons_layout;
	QShortcut *shortcut_add_operation;
	QShortcut *shortcut_copy_operation;
	QShortcut *shortcut_remove_operation;
//-------main widgets of dialog part-----------		
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
	QLabel *sum_label;
	QLineEdit *sum;
	QIntValidator *validator;
	QLabel *note_label;
	QLineEdit *note;
  QPushButton *save_incomePB;
  QPushButton *cancel_incomePB;

	bool row_added=false;
//---------main setup-----------
  void setup_Widget();
  void setup_ModelandMapper();
//---------helper functions-----	
	void func_addIncome();
	void func_editIncome(int);
  int func_check_correctness(const QSortFilterProxyModel*,int*);
	int func__insert_update();
signals:
  void signal_ready();
private slots:
  void slot_saveIncome();
  void slot_cancelIncome();
  void slot_open_itemsList(QModelIndex);
  void slot_passSelectedItem();
	void slot_add_operation();
	void slot_copy_operation();
	void slot_remove_operation();
	void slot_clear_items_filter();
	void slot_set_items_filter();
};

#endif
