#ifndef PROXY_MODELS_SENTRY_H
#define PROXY_MODELS_SENTRY_H

#include <QSortFilterProxyModel>
#include <QVariant>


class Proxy_op_number_income: public QSortFilterProxyModel
{
	Q_OBJECT
public:
	explicit Proxy_op_number_income(QObject *parent=nullptr) : QSortFilterProxyModel(parent) {/*---*/}	
	void setFilterPattern(const QVariant &pattern);
protected:
	bool filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const override;
private:
	QVariant filterPattern;
};

#endif
