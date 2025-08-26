#include "db.h"

Database::Database(QObject *parent) : QObject(parent) {  }
Database::~Database() {  }

void Database::connectDatabase() {
	if(!QFile(DB_FILE_PATH).exists())
		this->restoreDatabase();
	else 
		this->openDatabase();
}

bool Database::restoreDatabase() {
	if(this->openDatabase()) {
		if(!this->createTables())
			return false;
		else
			return true;
	} else {
			qDebug()<<"Database restoring is NOT succeded"<<"\t"<<DB_FILE_PATH;
			return false;
	}
	return false;
}

bool Database::openDatabase() {
	db=QSqlDatabase::addDatabase("QMYSQL",DB_NAME);
	db.setDatabaseName(DB_NAME);
	db.setHostName(HOST);
	db.setUserName(USER);
	db.setPassword(PW);
	db.setPort(PORT);
  if(!db.open()) {
		QMessageBox::information(nullptr,"Error message",
					"Connection to database is NOT established!");
		return false;
	}
	return true;	
}

void Database::closeDatabase() {
	db.close();
}

bool Database::createTables() {
	QSqlQuery query;
	QString itemsQueryString={"create table items(\
											   item_id smallint primary key autoincrement,\
	 										   item_name varchar(50) not null unique"};
	if(!query.exec(itemsQueryString)) {
		qDebug()<<"Error when creating item table";
		qDebug()<<query.lastError().text();
		return false;
	}
	return true;
}
