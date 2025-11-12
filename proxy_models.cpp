#include "proxy_models.h"


void Proxy_op_number_income::setFilterPattern(const QVariant &pattern) {
	if(filterPattern!=pattern) {
		filterPattern=pattern;
		invalidateFilter();
	}
}

bool Proxy_op_number_income::filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const {
	if(!filterPattern.isValid() || filterPattern.toString().isEmpty())
		return true;//   no filter applied
	QModelIndex index=sourceModel()->index(sourceRow,2,sourceParent);
	QString data=sourceModel()->data(index).toString();
	return data==filterPattern.toString();//---exact match
}
