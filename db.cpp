#include "db.h"

Database::Database(QObject *parent) : QObject(parent) {  }
Database::~Database() { 
	if(db.isOpen())
		db.close();
	QSqlDatabase::removeDatabase(DB_NAME);
}

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
//---Create table: items
	QString itemsQueryString={"create table if not exists items(\
											   item_id smallint primary key autoincrement,\
	 										   item_name varchar(50) not null unique)"};
	if(!query.exec(itemsQueryString)) {
		qDebug()<<"Error when creating table: items";
		qDebug()<<query.lastError().text();
		return false;
	}
//---Create table: balance
	QString balanceQueryString={"create table if not exists balance(\
															 cell_id int primary key auto_increment,\
															 cell char(8) not null,\
															 item varchar(50) null,\
															 quantity int null)"};
	if(!query.exec(balanceQueryString)) {
		qDebug()<<"Error when creating table: balance";
		qDebug()<<query.lastError().text();
		return false;
	}
////---Check if balance table is empty and if its so-fill cells column
//	QString check_if_cells_column_is_empty={"select count(*) from balance"};
//	if(!query.exec(check_if_cells_column_is_empty)) {
//		qDebug()<<"Error when checking if table is empty: balance";
//		qDebug()<<query.lastError().text();
//		return false;
//	}
//	if(query.next()) {
//		int rows=query.value(0).toInt();
//		if(rows==0)
//			fill_cells_column(2,7,18);
//	} else
//		qDebug()<<"Error: Could not retrieve count of rows in table: balance";


	return true;
}

void Database::fill_cells_column(uint8_t sections,uint8_t height,uint8_t cells) {
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);
	QString check_if_cells_column_is_empty={"select count(*) from balance"};
	if(!query.exec(check_if_cells_column_is_empty)) {
		qDebug()<<"Error when checking if table is empty: balance";
		qDebug()<<query.lastError().text();
		return;
	}
	if(query.next()) {
		int rows=query.value(0).toInt();
		if(rows!=0)
			return;
	}	
	uint8_t length=8;
	char letter='A';
	char dash='-';
	char floor_cell_right='0';
	char floor_cell_left='0';
	char cell_right='1';
	char cell_left='0';
	QString cell_string;
	for(uint8_t s=0;s!=sections;++s,++letter) {
		floor_cell_right='0';
		floor_cell_left='0';
		for(uint8_t h=0;h!=height;++h)	{
			cell_right='1';
			cell_left='0';
			if(floor_cell_right=='9') {
				floor_cell_right='0';
				floor_cell_left++;
			} else {
				floor_cell_right++;
			}
			for(uint8_t c=0;c!=cells;++c) {
				char *str=(char*)malloc(length*sizeof(char));
				if(!str) {
					perror("Heap allocation error. Exiting");
				exit(1);
				}
				str[length-1]='\0';
				str[length-2]=cell_right; 
				if(cell_right=='9') {
					cell_right='0';
					str[length-3]=cell_left;
					cell_left++;
				} else {
					cell_right++;
					str[length-3]=cell_left;
				}
				str[length-4]=dash;
				str[length-5]=floor_cell_right;
				str[length-6]=floor_cell_left;
				str[length-7]=dash;
				str[length-8]=letter;
//				printf("%s\n",str);
				cell_string=str;
				query.prepare("insert into balance(cell,item,quantity) values(:strvalue,null,null)");
				query.bindValue(":strvalue",cell_string);
				if(!query.exec()) {
					qDebug()<<"Error while filling cells column with strings: balance";
					qDebug()<<query.lastError().text();
					return;
				}
				if(str)
					free(str);
			}
		}
	}
}
