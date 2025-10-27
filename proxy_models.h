#ifndef PROXY_MODELS_SENTRY_H
#define PROXY_MODELS_SENTRY_H

#include <QSortFilterProxyModel>

#define QUANTITY 3

class Proxy_model_non_empty_rows: public QSortFilterProxyModel
{
	Q_OBJECT
public:
	explicit Proxy_model_non_empty_rows(QObject *parent=nullptr) 
		: QSortFilterProxyModel(parent) {/*---*/}	
protected:
	bool filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const override {
		enum { column_to_filter=QUANTITY };
		QModelIndex index=sourceModel()->index(sourceRow,column_to_filter,sourceParent);
		if(index.isValid() && sourceModel()->data(index).toInt()!=0)
			return true;
	return false;
	}
};

#endif
