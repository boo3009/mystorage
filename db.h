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

#define DB_NAME "mystorage"
#define DB_FILE_PATH "/var/lib/mysql/mystorage"
#define HOST "localhost"
#define USER "william"
#define PW "98077"
#define PORT 3306


class Database : public QObject
{
	Q_OBJECT
public:
	explicit Database(QObject *parent=0);
	~Database();
	void connectDatabase();
	void cells_filling(uint8_t,uint8_t,uint8_t);
private:
	QSqlDatabase db;
	
	bool openDatabase();
	bool restoreDatabase();
	void closeDatabase();
	bool createTables();
};

#endif
