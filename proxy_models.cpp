#include "proxy_models.h"


void Proxy_op_number::setFilterPattern(const QVariant &pattern) {
	if(filterPattern!=pattern) {
		filterPattern=pattern;
		invalidateFilter();
	}
}

bool Proxy_op_number::filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const {
	enum { OPERATION_NUMBER_COLUMN=2  };
	if(!filterPattern.isValid() || filterPattern.toString().isEmpty())
		return true;//   no filter applied
	QModelIndex index=sourceModel()->index(sourceRow,OPERATION_NUMBER_COLUMN,sourceParent);
	QString data=sourceModel()->data(index).toString();
	return data==filterPattern.toString();//---exact match
}


//void proxymodel_search_by_cell::setFilterPattern(const QVariant &pattern) {
//	if(filter_pattern!=pattern) {
//		filter_pattern=pattern;
//		invalidateFilter();
//	}
//}
//
//bool proxymodel_search_by_cell::filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const {
//	if(!filterPattern.isValid() || filterPattern.toString().isEmpty())
//		return true;//   no filter applied
