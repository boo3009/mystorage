#include "search_dialog.h"

search_dialog::search_dialog(QSqlQueryModel *search_model,int mode,QWidget *parent) : 
	QDialog(parent), ptr_search_model(search_model), cell_or_item_mode(mode) { 
	setup_dialog(ptr_search_model); 
}

void search_dialog::setup_dialog(QSqlQueryModel *ptr_search_model) {
  setWindowTitle("Search");
  setFixedSize(700,700);
	this->setObjectName("borders_for_search_dialog");
	this->setStyleSheet("QWidget#borders_for_search_dialog {" 
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
	cell_or_item_LE=new QLineEdit();
	cell_or_item_LE->clear();
	cell_or_item_LE->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	cell_or_item_L=new QLabel();
	cell_or_item_L->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	if(cell_or_item_mode==0)
		cell_or_item_L->setText("Cell");
	if(cell_or_item_mode==1)
		cell_or_item_L->setText("Item");
	balance_button_layout=new QHBoxLayout();
	
	inc_balance_L=new QLabel("Incomes: ");
	inc_balance_L->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	inc_balance_LE=new QLineEdit();
	inc_balance_LE->setReadOnly(true);
	inc_balance_LE->setAlignment(Qt::AlignCenter);
	inc_balance_LE->setFixedWidth(70);
	inc_balance_LE->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	inc_balance_LE->clear();
	
	out_balance_L=new QLabel("Outcomes: ");
	out_balance_L->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	out_balance_LE=new QLineEdit();
	out_balance_LE->setReadOnly(true);
	out_balance_LE->setAlignment(Qt::AlignCenter);
	out_balance_LE->setFixedWidth(70);
	out_balance_LE->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	out_balance_LE->clear();
	
	filtered_balance_L=new QLabel("Filtered: ");
	filtered_balance_L->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	filtered_balance_LE=new QLineEdit();
	filtered_balance_LE->setReadOnly(true);
	filtered_balance_LE->setAlignment(Qt::AlignCenter);
	filtered_balance_LE->setFixedWidth(70);
	filtered_balance_LE->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	filtered_balance_LE->clear();
	
	current_balance_L=new QLabel("Current: ");
	current_balance_L->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	current_balance_LE=new QLineEdit();
	current_balance_LE->setReadOnly(true);
	current_balance_LE->setAlignment(Qt::AlignCenter);
	current_balance_LE->setFixedWidth(70);
	current_balance_LE->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	current_balance_LE->clear();
	
	balance_button_layout->addWidget(inc_balance_L);
	balance_button_layout->addWidget(inc_balance_LE);
	balance_button_layout->addWidget(out_balance_L);
	balance_button_layout->addWidget(out_balance_LE);
	balance_button_layout->addWidget(filtered_balance_L);
	balance_button_layout->addWidget(filtered_balance_LE);
	balance_button_layout->addWidget(current_balance_L);
	balance_button_layout->addWidget(current_balance_LE);
	balance_button_layout->addStretch();
	balance_button_layout->setAlignment(Qt::AlignBottom);
	search_dialog_button_layout->addWidget(cell_or_item_L);
	search_dialog_button_layout->addWidget(cell_or_item_LE);
	search_dialog_button_layout->addWidget(date_from_L);
	search_dialog_button_layout->addWidget(date_from_DE);
	search_dialog_button_layout->addWidget(date_to_L);
	search_dialog_button_layout->addWidget(date_to_DE);
	search_dialog_button_layout->addWidget(search_PB);
	search_dialog_button_layout->addStretch();
	search_dialog_button_layout->setAlignment(Qt::AlignTop);
	search_dialog_layout->addLayout(search_dialog_button_layout);
	search_dialog_layout->addWidget(search_view);
	search_dialog_layout->addLayout(balance_button_layout);

	this->setAutoFillBackground(true);
	QPalette palette=this->palette();
	palette.setColor(QPalette::Window,QColor("#0F828C"));
	this->setPalette(palette);
	
	connect(search_PB,&QPushButton::clicked,this,&search_dialog::slot_search_filtered);
}

void search_dialog::slot_search_filtered() {
	if(cell_or_item_LE->text().isEmpty()) {
    QMessageBox::information(nullptr,"Warning message","Provide with cell or an item please");
		return;
	}
	QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery query(retrieveDB);
	if(cell_or_item_mode==0)
		query.prepare("select * from operations where status like 'SAVED'"
									"and date>=? and date<=? and cell like ?");
	if(cell_or_item_mode==1)
		query.prepare("select * from operations where status like 'SAVED'" 
									"and date>=? and date<=? and item REGEXP ?");
	query.addBindValue(date_from_DE->date().toString("yyyy-MM-dd"));
	query.addBindValue(date_to_DE->date().toString("yyyy-MM-dd"));
	query.addBindValue(cell_or_item_LE->text());
	if(query.exec()) {
		if(!query.next()) {
			search_view->setVisible(false);
			get_search_balance();
			QMessageBox::information(nullptr,"Info","No movement related with specified cell or item");
			return;
		}
	}	else
		qDebug()<<"FAILED to exec query in slot_search_filtered!";
	get_search_balance();
	ptr_search_model->setQuery(query);
	if(ptr_search_model->lastError().isValid())
		qDebug()<<"select from operations FAILED"<<ptr_search_model->lastError().text();
	search_view->setColumnHidden(0,true);
  search_view->setColumnHidden(4,true);
  search_view->setColumnWidth(1,70);	//date
  search_view->setColumnWidth(2,70);	//op_num
  search_view->setColumnWidth(3,80);	//op_type
  search_view->setColumnWidth(5,70);	//cell
  search_view->setColumnWidth(6,320);	//item
  search_view->setColumnWidth(7,30);	//quantity
	search_view->setVisible(true);
}

void search_dialog::get_search_balance() {
	QSqlDatabase retrieveDB=QSqlDatabase::database(DB_NAME);
	QSqlQuery inc_query(retrieveDB);
	QSqlQuery out_query(retrieveDB);
	QSqlQuery bal_cur_query(retrieveDB);
	QString inc_str,out_str,cur_bal_str;
	
	if(cell_or_item_mode==0) {
		inc_str=R"(select sum(case when operation_type like 'income operation' and cell like ? 
							 and date>= ? and date<= ? then quantity else 0 end) from operations 
							 where status like 'SAVED')";
		out_str=R"(select sum(case when operation_type like 'outcome operation' and cell like ? 
							 and date>= ? and date<= ? then quantity else 0 end) from operations
							 where status like 'SAVED')";
		cur_bal_str=R"(select
			sum(case when operation_type like 'income operation' and cell like ? then quantity else 0 end) -
			sum(case when operation_type like 'outcome operation' and cell like ? then quantity else 0 end)
			from operations where status like 'SAVED')";
	}
	if(cell_or_item_mode==1) {
		inc_str=R"(select sum(case when operation_type like 'income operation' and item REGEXP ? 
							 and date>= ? and date<= ? then quantity else 0 end) from operations
							 where status like 'SAVED')";
		out_str=R"(select sum(case when operation_type like 'outcome operation' and item REGEXP ? 
							 and date>= ? and date<= ? then quantity else 0 end) from operations
							 where status like 'SAVED')";
		cur_bal_str=R"(select
			sum(case when operation_type like 'income operation' and item REGEXP ? then quantity else 0 end) -
			sum(case when operation_type like 'outcome operation' and item REGEXP ? then quantity else 0 end)
			from operations where status like 'SAVED')";
	}
	inc_query.prepare(inc_str);
	inc_query.addBindValue(cell_or_item_LE->text());
	inc_query.addBindValue(date_from_DE->date().toString("yyyy-MM-dd"));
	inc_query.addBindValue(date_to_DE->date().toString("yyyy-MM-dd"));
	if(inc_query.exec()) {
		if(inc_query.next())
			inc_balance_LE->setText(inc_query.value(0).toString());
		else
			inc_balance_LE->setText("-");
	} else
		qDebug()<<"FAILED to sum incomes balance";
	out_query.prepare(out_str);
	out_query.addBindValue(cell_or_item_LE->text());
	out_query.addBindValue(date_from_DE->date().toString("yyyy-MM-dd"));
	out_query.addBindValue(date_to_DE->date().toString("yyyy-MM-dd"));
	if(out_query.exec()) {
		if(out_query.next())
			out_balance_LE->setText(out_query.value(0).toString());
		else
			out_balance_LE->setText("-");
	} else
		qDebug()<<"FAILED to sum outcomes balance";
	bal_cur_query.prepare(cur_bal_str);
	bal_cur_query.addBindValue(cell_or_item_LE->text());
	bal_cur_query.addBindValue(cell_or_item_LE->text());
	if(bal_cur_query.exec()) {
		if(bal_cur_query.next())
			current_balance_LE->setText(bal_cur_query.value(0).toString());
		else
			current_balance_LE->setText("-");
	} else
		qDebug()<<"FAILED to sum current balance";
	if(inc_query.value(0).toString()!="-" && out_query.value(0).toString()!="-")
		filtered_balance_LE->setText(QString::number(inc_query.value(0).toInt()-out_query.value(0).toInt()));
	else
		filtered_balance_LE->setText("-");
}
