#include "search_item_dialog.h"

search_item_dialog::search_item_dialog(QSqlQueryModel *search_model,QWidget *parent) : QDialog(parent),
											 ptr_search_model(search_model) { 
	setup_dialog(ptr_search_model); 
}

void search_item_dialog::setup_dialog(QSqlQueryModel *ptr_search_model) {
  setWindowTitle("Search");
  setFixedSize(700,700);
	this->setObjectName("borders_for_search_item_dialog");
	this->setStyleSheet("QWidget#borders_for_search_item_dialog {" 
												"background-color: #ABE7B2; color: black;"
												"border: 1px solid #427A76; }");
	search_view=new QTableView(this);
	search_view->setModel(ptr_search_model);
	search_view->setSelectionMode(QAbstractItemView::SingleSelection);
  search_view->setSelectionBehavior(QAbstractItemView::SelectRows);
  search_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
  search_view->setStyleSheet(
    "QTableView {"
			"gridline-color: #7F8CAA;  border: 2px solid #7F8CAA;"
    	"background: #E8F9FF;  selection-background-color: #6D94C5;}"
    "QTableView::item { padding: 5px;}"); 
	search_view_header=search_view->horizontalHeader();
  search_view_header->setStretchLastSection(true);
  search_view_header->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  search_view_header->setStyleSheet(
    "QHeaderView::section { background-color: #0F828C;}");
  QFont search_view_headerFont("Colibri",10,QFont::Bold);
  search_view_header->setFont(search_view_headerFont);
//	search_view_header->setSectionResizeMode(QHeaderView::ResizeToContents);
	search_view->horizontalHeader()->setVisible(false);	
  search_view->verticalHeader()->setVisible(false);
	search_view->setVisible(false);

  search_dialog_layout=new QVBoxLayout(this);
  search_dialog_button_layout=new QHBoxLayout();
	search_PB=new QPushButton("Search");
	search_PB->setStyleSheet("background-color: #8A2BE2");
	search_PB->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	date_from_DE=new QDateEdit();
	date_from_DE->setDate(QDate::currentDate());
	date_from_DE->setDisplayFormat("dd.MM.yyyy");
	date_from_DE->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	date_from_L=new QLabel("Date from: ");
	date_to_DE=new QDateEdit();
	date_to_DE->setDate(QDate::currentDate());
	date_to_DE->setDisplayFormat("dd.MM.yyyy");
	date_to_DE->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	date_to_L=new QLabel("Date to: ");
	item_LE=new QLineEdit();
	item_LE->clear();
	item_LE->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	item_L=new QLabel("Item");
	item_L->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	search_dialog_button_layout->addWidget(item_L);
	search_dialog_button_layout->addWidget(item_LE);
	search_dialog_button_layout->addWidget(date_from_L);
	search_dialog_button_layout->addWidget(date_from_DE);
	search_dialog_button_layout->addWidget(date_to_L);
	search_dialog_button_layout->addWidget(date_to_DE);
	search_dialog_button_layout->addWidget(search_PB);
	search_dialog_button_layout->addStretch();
	search_dialog_button_layout->setAlignment(Qt::AlignTop);
	search_dialog_layout->addLayout(search_dialog_button_layout);
	search_dialog_layout->addWidget(search_view);

	this->setAutoFillBackground(true);
	QPalette palette=this->palette();
	palette.setColor(QPalette::Window,QColor("#0F828C"));
	this->setPalette(palette);
	
	connect(search_PB,&QPushButton::clicked,this,&search_item_dialog::slot_search_filtered);
}

void search_item_dialog::slot_search_filtered() {
	if(item_LE->text().isEmpty()) {
    QMessageBox::information(nullptr,"Warning message","Please, set the 'Cell' field!");
		return;
	}
	QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);
	query.prepare("select * from operations where status like 'SAVED' and date>=? and date<=? and item REGEXP ?");
	query.addBindValue(date_from_DE->date().toString("yyyy-MM-dd"));
	query.addBindValue(date_to_DE->date().toString("yyyy-MM-dd"));
	query.addBindValue(item_LE->text());
	if(query.exec()) {
		if(!query.next()) {
			search_view->setVisible(false);
			QMessageBox::information(nullptr,"Info","There is no movement related with specified item");
			return;
		}
	}	else
		qDebug()<<"FAILED to exec query in slot_search_filtered!";
	ptr_search_model->setQuery(query);
	if(ptr_search_model->lastError().isValid())
		qDebug()<<"select from operations FAILED"<<ptr_search_model->lastError().text();
  search_view->setColumnHidden(0,true);
  search_view->setColumnHidden(4,true);
  search_view->setColumnWidth(1,70);	//date
  search_view->setColumnWidth(2,70);	//op_num
  search_view->setColumnWidth(3,80);	//op_type
  search_view->setColumnWidth(5,70);	//item
  search_view->setColumnWidth(6,320);	//item
  search_view->setColumnWidth(7,30);	//quantity
	search_view->setVisible(true);
//	USE DELEGATES TO PAINT INCOMES AND OUTCOMES NI COLORS
}
