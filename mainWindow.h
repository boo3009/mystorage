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
#include <QTableView>
#include <QDataWidgetMapper>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>

#include "db.h"
#include "itemDialog.h"


class MainWindow:public QMainWindow {
	Q_OBJECT
public:
	MainWindow(QWidget *parent=nullptr);
private:
//--------------------------main parts of interface
	QWidget *mainWidget;
	QHBoxLayout *mainLayout;
	QVBoxLayout *mainLayout_left;
	QVBoxLayout *mainLayout_middle;
//--------------------------left part
	QPushButton *mainLayout_left_incomePB;
	QPushButton *mainLayout_left_outcomePB;
	QPushButton *mainLayout_left_itemsPB;
	QPushButton *mainLayout_left_balancePB;
	QPushButton *mainLayout_left_quitPB;
//--------------------------middle part: stack of widgets
	QStackedWidget *mainLayout_middle_stack;
//--------------------------middle part: List of items
	QSqlTableModel *itemsModel;
	QTableView *itemsView;
	QHeaderView *itemsView_header;
	QWidget *itemsModelView_widget;
	QHBoxLayout *itemsModelView_widget_mainLayout;
	QVBoxLayout *itemsModelView_widget_buttonsLayout;
	QPushButton *itemsModelView_widget_addItemPB;
	QPushButton *itemsModelView_widget_editItemPB;
	QPushButton *itemsModelView_widget_copyItemPB;
	QPushButton *itemsModelView_widget_removeItemPB;
//--------------------------pointer to "Database" object	
	Database *dbPointer;
//--------------------------member functions
	void connect_Database();
	void setup_ModelViews();
	void setup_CoreWidgets();
	void setup_SignalSlots();
	
	void setup_itemsModelView();	
signals:
	void signal_clickedEdit();
	void signal_addItem();
private slots:
	void slot_UpdateModels();
//	void slot_editItem();
	void slot_itemDialog_add();
};

#endif
