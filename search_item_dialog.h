#ifndef SEARCH_ITEM_SENTRY_H
#define SEARCH_ITEM_SENTRY_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QMessageBox>
#include <QDebug>

#include "db.h"

class search_item_dialog : public QDialog {
	Q_OBJECT
public:
	explicit search_item_dialog(QSqlQueryModel *search_model,QWidget *parent=0);
private:
	QSqlQueryModel 				*ptr_search_model;
	QVBoxLayout 					*search_dialog_layout;
	QHBoxLayout 					*search_dialog_button_layout;
	QTableView 						*search_view;
	QHeaderView 					*search_view_header;
	QPushButton 					*search_PB;
	QLabel 								*date_from_L;
	QDateEdit 						*date_from_DE;
	QLabel 								*date_to_L;
	QDateEdit 						*date_to_DE;
	QLabel 								*item_L;
	QLineEdit 						*item_LE;
//--------	
	void setup_dialog(QSqlQueryModel *ptr_search_model);
private slots:
	void slot_search_filtered();
};

#endif
