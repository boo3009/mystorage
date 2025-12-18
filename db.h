#ifndef DB_SENTRY_H
#define DB_SENTRY_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#define DB_NAME "mystorage"
#define DB_FILE_PATH "/var/lib/mysql/mystorage"
#define HOST "localhost"
#define USER "william"
#define PORT 3306


class Database : public QObject
{
	Q_OBJECT
public:
	explicit Database(QObject *parent=0);
	~Database();
	void connect_database();
private:
	QSqlDatabase	database_object;
//----------input part for filling cells
	QWidget 		 *input_widget;
	QGridLayout  *input_widget_layout;
	QLabel 			 *input_widget_sections_lable;
	QLineEdit 	 *input_widget_sections_lineedit;
	QLabel 			 *input_widget_levels_lable;
	QLineEdit 	 *input_widget_levels_lineedit;
	QLabel 			 *input_widget_cells_lable;
	QLineEdit 	 *input_widget_cells_lineedit;
	QPushButton  *input_widget_generate_button;
	QPushButton	 *input_widget_cancel_button;
//----------main part functions	
	bool open_database();
	bool restore_database();
	bool create_tables();
//----------getting inputs and filling cells
	void get_inputs_for_filling_cells();
	void filling_cells();
};

#endif
