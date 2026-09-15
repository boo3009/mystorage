#ifndef SEARCH_SENTRY_H
#define SEARCH_SENTRY_H

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
#include <QSpacerItem>
#include <QMessageBox>
#include <QDebug>
#include <QColor>

#include "db.h"
#include "delegates_proxies.h"
#include "translations.h"

class search_dialog : public QDialog {
	Q_OBJECT
public:
	explicit search_dialog(QSqlQueryModel *search_model,int mode=0,QWidget *parent=0);
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
	QLabel 								*cell_or_item_L;
	QLineEdit 						*cell_or_item_LE;
	QHBoxLayout 					*balance_button_layout;
	QLabel 								*inc_balance_L;
	QLineEdit 						*inc_balance_LE;
	QLabel 								*out_balance_L;
	QLineEdit 						*out_balance_LE;
	QLabel 								*filtered_balance_L;
	QLineEdit 						*filtered_balance_LE;
	QLabel 								*current_balance_L;
	QLineEdit 						*current_balance_LE;
	int 									cell_or_item_mode=0;
//-------------------------------------------------------
	void setup_dialog(QSqlQueryModel *ptr_search_model);
private slots:
	void slot_search_filtered();
	void get_search_balance();
};

class query_model_class : public QSqlQueryModel {
	Q_OBJECT
public:
	using QSqlQueryModel::QSqlQueryModel;  //inherit constructors
	QVariant data(const QModelIndex &index,int role) const override {
		QColor income_color("#F5F5DC");
		QColor outcome_color("#FF6347");
		if(role==Qt::BackgroundRole) {
			if(this->index(index.row(),3).data().toString() == "outcome operation")
				return outcome_color;
			else
				return income_color;
		}
		return QSqlQueryModel::data(index,role);
	}
};

#endif
