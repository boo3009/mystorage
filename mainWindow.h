#ifndef MAINWINDOW_SENTRY_H
#define MAINWINDOW_SENTRY_H

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include <QtSql>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QDataWidgetMapper>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>
#include <QSizePolicy>
#include <QDateTime>
#include <QRegExp>

#include "db.h"
#include "itemDialog.h"
#include "incomeDialog.h"
#include "outcomeDialog.h"


class MainWindow:public QMainWindow {
	Q_OBJECT
public:
	MainWindow(QWidget *parent=nullptr);
private:
//--------------------------main parts of interface
	QWidget 							*mainWidget;
	QHBoxLayout 					*mainLayout;
	QVBoxLayout 					*mainLayout_left;
	QVBoxLayout 					*mainLayout_middle;
//--------------------------left part
	QPushButton 					*mainLayout_left_incomePB;
	QPushButton 					*mainLayout_left_outcomePB;
	QPushButton 					*mainLayout_left_itemsPB;
	QPushButton 					*mainLayout_left_balancePB;
	QPushButton 					*mainLayout_left_quitPB;
//--------------------------middle part: stack of widgets
	QStackedWidget 				*mainLayout_middle_stack;
//--------------------------middle part: List of items
	QSqlTableModel 				*itemsModel;
	QTableView 		 				*itemsView;
	QSortFilterProxyModel *items_proxymodel; //----experimental
	QLabel 								*items_filter_label;
	QLineEdit 						*items_filter_lineedit;
	QPushButton 					*clear_filterPB;
	QHBoxLayout 					*items_filter_layout;
	QHeaderView 					*itemsView_header;
	QWidget 							*itemsModelView_widget;
	QVBoxLayout 					*itemsModelView_widget_mainLayout;
	QHBoxLayout 					*items_layout;
	QVBoxLayout 					*itemsModelView_widget_buttonsLayout;
	QPushButton 					*itemsModelView_widget_addItemPB;
	QPushButton 					*itemsModelView_widget_editItemPB;
	QPushButton 					*itemsModelView_widget_copyItemPB;
	QPushButton 					*itemsModelView_widget_removeItemPB;
//--------------------------middle part: Income
	QSqlTableModel 				*incomeModel;
	QTableView 						*incomeView;
	QHeaderView 					*incomeView_header;
	QWidget 							*incomeModelView_widget;
	QHBoxLayout 					*incomeModelView_widget_mainLayout;
	QVBoxLayout 					*incomeModelView_widget_buttonsLayout;
	QPushButton 					*incomeModelView_widget_addIncomePB;
	QPushButton 					*incomeModelView_widget_editIncomePB;
	QPushButton 					*incomeModelView_widget_removeIncomePB;
	QPushButton 					*incomeModelView_widget_cancel_removeIncomePB;
//--------------------------hidden part: Operations
	QSqlTableModel 				*operationsModel;
//--------------------------middle part: Outcome
	QSqlTableModel 				*outcomeModel;
	QTableView 						*outcomeView;
	QHeaderView 					*outcomeView_header;
	QWidget 							*outcomeModelView_widget;
	QHBoxLayout 					*outcomeModelView_widget_mainLayout;
	QVBoxLayout 					*outcomeModelView_widget_buttonsLayout;
	QPushButton 					*outcomeModelView_widget_addOutcomePB;
	QPushButton 					*outcomeModelView_widget_editOutcomePB;
	QPushButton 					*outcomeModelView_widget_removeOutcomePB;
	QPushButton 					*outcomeModelView_widget_cancel_removeOutcomePB;
//--------------------------middle part: Balance
	QSqlTableModel 				*balanceModel;
	QTableView	 					*balanceView;
	QHeaderView 					*balanceView_header;
	QWidget 							*balanceModelView_widget;
	QDateEdit 						*date_filter;
	QPushButton 					*generate;
	QPushButton 					*write_into_file;
	QLabel 								*pieces_label;
	QLineEdit 						*pieces_lineedit;
	QLabel 								*non_empty_cells_label;
	QLineEdit 						*non_empty_cells_lineedit;
	QVBoxLayout 					*balanceModelView_widget_mainLayout;
	QHBoxLayout 					*balanceModelView_widget_buttons_layout;
//--------------------------pointer to "Database" object	
	Database 							*dbPointer;
//--------------------------member functions Level 0
	void connect_Database();
	void setup_ModelViews();
	void setup_CoreWidgets();
	void setup_SignalSlots();
//--------------------------member functions Level 1
	void setup_itemsModelView();	
	void setup_incomeModelView();	
	void setup_outcomeModelView();	
	void setup_balanceModelView();	
	void setup_operationsModelView();	
signals:
/*----------------------------------------*/
private slots:
	void slot_updateModels();
	void set_selected_button_dark(QPushButton*,QPushButton*,
																QPushButton*,QPushButton*);
//--------------------------slots for items
	void slot_itemDialog_add();
	void slot_itemDialog_edit();
	void slot_editItemByDoubleClick(QModelIndex);
	void slot_itemDialog_copy();
	void slot_itemsModelView_remove();
	void slot_set_items_filter(); //----experimental
	void slot_clear_items_filter(); //----experimental
//--------------------------slots for income
	void slot_incomeDialog_add();
	void slot_incomeDialog_edit();
	void slot_editIncomeByDoubleClick(QModelIndex);
	void slot_incomeModelView_remove();
	void slot_incomeModelView_cancel_remove();
//--------------------------slots for outcome
	void slot_outcomeDialog_add();
	void slot_outcomeDialog_edit();
	void slot_editOutcomeByDoubleClick(QModelIndex);
	void slot_outcomeModelView_remove();
	void slot_outcomeModelView_cancel_remove();
//--------------------------slots for balance
	int slot_generate_balance();
	void slot_call_generate_balance();
	void slot_write_balance_into_file();
};

#endif
