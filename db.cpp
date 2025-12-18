#include "db.h"

Database::Database(QObject *parent) : QObject(parent) {  }
Database::~Database() { 
	if(database_object.isOpen())
		database_object.close();
//	QSqlDatabase::removeDatabase(DB_NAME);
	if(input_widget)
		delete input_widget;
}

void Database::connect_database() {
	if(!QFile(DB_FILE_PATH).exists())
		this->restore_database();
	else  
		this->open_database();
}

bool Database::restore_database() {
	if(this->open_database()) {
		if(this->create_tables())
			return true;
	}
	qDebug()<<"Error: Database can't be restored.";
	return false;
}

bool Database::open_database() {
	database_object=QSqlDatabase::addDatabase("QMYSQL",DB_NAME);
	database_object.setDatabaseName(DB_NAME);
	database_object.setHostName(HOST);
	database_object.setUserName(USER);
//	database_object.setPassword(PW); //created user with no password, so dont need this anymore
	database_object.setPort(PORT);
	if(!database_object.open()) {
		QMessageBox::information(nullptr,"Error message",
			"Connection to database as a 'user' is NOT established!");
		return false;
	}
	if(!create_tables())
		QMessageBox::information(nullptr,"Error message",
			"Can't create tables for database!");
	return true;	
}

bool Database::create_tables() {
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
  QSqlQuery query(retrieveDB);
//---Create table income
	QString income_str={"create table if not exists income(\
												 id int primary key auto_increment,\
												 operation_number varchar(7) not null,\
												 date date not null,\
												 operation_type char(16) default 'income operation',\
												 status varchar(7) default 'SAVED',\
												 sum int not null,\
												 note varchar(100))"};
	if(!query.exec(income_str)) {
		qDebug()<<"Error when creating table: income";
		qDebug()<<query.lastError().text();
		return false;
	}
//---Create table outcome
	QString outcome_str={"create table if not exists outcome(\
											    id int primary key auto_increment,\
												  operation_number varchar(7) not null,\
												  date date not null,\
												  operation_type char(17) default 'outcome operation',\
												  status varchar(7) default 'SAVED',\
												  sum int not null,\
	 										    note varchar(100))"};
	if(!query.exec(outcome_str)) {
		qDebug()<<"Error when creating table: outcome";
		qDebug()<<query.lastError().text();
		return false;
	}
//---Create table: items
	QString items_str={"create table if not exists items(\
										    item_id smallint primary key auto_increment,\
 										    item_name varchar(50) not null unique)"};
	if(!query.exec(items_str)) {
		qDebug()<<"Error when creating table: items";
		qDebug()<<query.lastError().text();
		return false;
	}
//---Create operations 
	QString operations_str={"create table if not exists operations(\
														 operation_id int unsigned primary key auto_increment,\
														 date date not null,\
														 operation_number varchar(7) not null,\
														 operation_type char(17) not null,\
														 status varchar(7) not null,\
														 cell char(8) not null,\
														 item varchar(50) not null,\
														 quantity int not null)"};
	if(!query.exec(operations_str)) {
		qDebug()<<"Error when creating table: operations";
		qDebug()<<query.lastError().text();
		return false;
	}
//---Create filled_cells 
	QString filled_cells_str={"create table if not exists filled_cells(\
														 cell_id int primary key auto_increment,\
														 cell char(7) not null,\
														 item varchar(50) not null,\
														 quantity int not null)"};
	if(!query.exec(filled_cells_str)) {
		qDebug()<<"Error when creating table: filled_cells";
		qDebug()<<query.lastError().text();
		return false;
	}
//---Create table: cells
	QString cells_str={"create table if not exists cells(\
												cell_id int primary key auto_increment,\
												cell char(7) not null)"};
	if(!query.exec(cells_str)) {
		qDebug()<<"Error when creating table: cells";
		qDebug()<<query.lastError().text();
		return false;
	}
//---Check if cells table is empty and if its so-fill cells column
	QString check_if_cells_column_is_empty={"select count(*) from cells"};
	if(!query.exec(check_if_cells_column_is_empty)) {
		qDebug()<<"Error when checking if table is empty: cells";
		qDebug()<<query.lastError().text();
		return false;
	}
	if(query.next()) {
		if(query.value(0).toInt()==0) {
			QMessageBox::information(nullptr,"Warning message","Cells table is empty,fill it out please.");
			get_inputs_for_filling_cells();
		}
	} else
		qDebug()<<"Error: Could not retrieve count of rows in table: cells";
	return true;
}

void Database::get_inputs_for_filling_cells() {
	input_widget=new QWidget();	
	input_widget->setFixedSize(250,115);

	input_widget_layout=new QGridLayout(input_widget);

	input_widget_sections_lable=new QLabel("Sections");
	input_widget_sections_lineedit=new QLineEdit();
	input_widget_levels_lable=new QLabel("Levels");
	input_widget_levels_lineedit=new QLineEdit();
	input_widget_cells_lable=new QLabel("Cells");
	input_widget_cells_lineedit=new QLineEdit();

	input_widget_layout->addWidget(input_widget_sections_lable,0,0);
	input_widget_layout->addWidget(input_widget_sections_lineedit,1,0);
	input_widget_layout->addWidget(input_widget_levels_lable,0,1);
	input_widget_layout->addWidget(input_widget_levels_lineedit,1,1);
	input_widget_layout->addWidget(input_widget_cells_lable,0,2);
	input_widget_layout->addWidget(input_widget_cells_lineedit,1,2);

	input_widget_generate_button=new QPushButton("Generate");
	input_widget_cancel_button=new QPushButton("Cancel");

	input_widget_layout->addWidget(input_widget_generate_button,2,0);
	input_widget_layout->addWidget(input_widget_cancel_button,2,1);
	input_widget->show();

	connect(input_widget_generate_button,&QPushButton::clicked,this,&Database::filling_cells);
	connect(input_widget_cancel_button,&QPushButton::clicked,[=](){ input_widget->close(); });
}

void Database::filling_cells() {
	uint8_t sections=input_widget_sections_lineedit->text().toInt();
	uint8_t height=input_widget_levels_lineedit->text().toInt();
	uint8_t cells=input_widget_cells_lineedit->text().toInt();
//-----------------------------------------------------------------------
  QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);
	QString check_if_cells_column_is_empty={"select count(*) from cells"};
	if(!query.exec(check_if_cells_column_is_empty)) {
		qDebug()<<"Error when checking if table is empty: cells";
		qDebug()<<query.lastError().text();
		return;
	}
	if(query.next()) {
		int rows=query.value(0).toInt();
		if(rows!=0)
			return;
	}	
//-----------------------------------------------------------------------
	const uint8_t length=8;
	char letter='A';
	const char dash='-';
	QString cell_string;
//-----------------------------------------------------------------------
	char *str=(char*)malloc(length*sizeof(char));
	if(!str) {
		perror("Heap allocation error. Exiting");
		exit(1);
	}
//   Starting transaction for inserting cells.
	if(!database_object.transaction()) {
		qDebug()<<"Failed to start transaction!";
		qDebug()<<query.lastError().text();
		return;
	}
//-----------------------------------------------------------------------
	for(uint8_t s=0;s!=sections;++s,++letter) {
    char floor_cell_right='0';
		char floor_cell_left='0';
//-----------------------------------------------------------------------
		for(uint8_t h=0;h!=height;++h)	{
			char cell_right='1';
			char cell_left='0';
			if(floor_cell_right=='9') {
				floor_cell_right='0';
				floor_cell_left++;
			} else {
				floor_cell_right++;
			}
//-----------------------------------------------------------------------
			for(uint8_t c=0;c!=cells;++c) {
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
//-----------------------------------------------------------------------
				cell_string=str;
				query.prepare("insert into cells(cell) values(:strvalue)");
				query.bindValue(":strvalue",cell_string);
				if(!query.exec()) {
					qDebug()<<"Error while filling cells column with strings: cells";
					qDebug()<<query.lastError().text();
					return;
				}
//-----------------------------------------------------------------------
			}
		}
	}
//   Commiting transaction for inserted cells.
	if(!database_object.commit()) {
		qDebug()<<"Failed to commit transaction!";
		qDebug()<<query.lastError().text();
		return;
	}
//-----------------------------------------------------------------------
	if(str)
		free(str);
//-----------------------------------------------------------------------
	input_widget->close();
	QMessageBox::information(nullptr,"Informational message",
				"Cells table was filled with data, fine :)");
}
