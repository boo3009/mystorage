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
	void connectDatabase();
private:
	QSqlDatabase db;
	QWidget *input_widget;
	QGridLayout *input_widget_layout;
	QLabel *input_widget_sections_lable;
	QLineEdit *input_widget_sections_lineedit;
	QLabel *input_widget_levels_lable;
	QLineEdit *input_widget_levels_lineedit;
	QLabel *input_widget_cells_lable;
	QLineEdit *input_widget_cells_lineedit;
	QPushButton *input_widget_generatePG;
	QPushButton	*input_widget_cancelPG;
	void input_for_filling_cells();
	void cells_filling();
		
	bool openDatabase();
	bool restoreDatabase();
	bool createTables();
};

#endif
