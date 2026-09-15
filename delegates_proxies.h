#ifndef DELEGATES_PROXIES_SENTRY_H
#define DELEGATES_PROXIES_SENTRY_H

#include <QStyledItemDelegate>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QVariant>
#include <QLineEdit>
#include <QWidget>
#include <QString>
#include <QLocale>
#include <QDate>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QSortFilterProxyModel>


class date_delegate : public QStyledItemDelegate {
public:
	using QStyledItemDelegate::QStyledItemDelegate;
	QString displayText(const QVariant &value,const QLocale &locale) const override {
		if(value.canConvert<QDate>())
			return locale.toString(value.toDate(),"dd-MM-yyyy");
		return QStyledItemDelegate::displayText(value,locale);
	}
};

class Proxy_op_number : public QSortFilterProxyModel {
	Q_OBJECT
public:
	explicit Proxy_op_number(QObject *parent=nullptr) : QSortFilterProxyModel(parent) {/*---*/}	
	void setFilterPattern(const QVariant &pattern) {
		if(filterPattern!=pattern) {
			filterPattern=pattern;
			invalidateFilter();
		}
	}
protected:
	bool filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const override {
		enum { OPERATION_NUMBER_COLUMN=2  };
		if(!filterPattern.isValid() || filterPattern.toString().isEmpty())
			return true;//   no filter applied
		QModelIndex index=sourceModel()->index(sourceRow,OPERATION_NUMBER_COLUMN,sourceParent);
		QString data=sourceModel()->data(index).toString();
		return data==filterPattern.toString();//---exact match
	}
private:
	QVariant filterPattern;
};

class hypen_validator : public QRegularExpressionValidator {
	Q_OBJECT
public:
	explicit hypen_validator(const QRegularExpression &str,QObject* parent=nullptr) : 
													 QRegularExpressionValidator(str,parent) {}
	
	State validate(QString &input,int &pos) const override {
		Q_UNUSED(pos);
		if(input.isEmpty())
			return Intermediate;
		if(input.contains(' '))
			input.replace(' ','-');
		input=input.toUpper();
		State st=QRegularExpressionValidator::validate(input,pos);
		return st;
	}
};

class cell_delegate : public QStyledItemDelegate {
	Q_OBJECT
public:
	explicit cell_delegate(QObject* parent=nullptr) : QStyledItemDelegate(parent) {}
	
	QWidget* createEditor(QWidget* parent,const QStyleOptionViewItem &option,
												const QModelIndex &index) const {
		QWidget* editor=QStyledItemDelegate::createEditor(parent,option,index);
		if(QLineEdit* lineedit=qobject_cast<QLineEdit*>(editor)) {
			QRegularExpression regexp("^[a-zA-Z]{1}\\-[0-9]{2}\\-[0-9]{2}$");
			hypen_validator *validator=new hypen_validator(regexp,lineedit);
			lineedit->setValidator(validator);
		}
		return editor;
	}
};

#endif
