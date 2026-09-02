#ifndef SEARCH_QUERY_MODEL_SENTRY_H
#define SEARCH_QUERY_MODEL_SENTRY_H

#include <QSqlQueryModel>
#include <QColor>

class query_model_class : public QSqlQueryModel {
	Q_OBJECT
public:
	using QSqlQueryModel::QSqlQueryModel;  //inherit constructors
	QVariant data(const QModelIndex &index,int role) const override {
		QColor income_color("#F5F5DC");
		QColor outcome_color("#FF6347");
		if(role==Qt::BackgroundRole) {
			if(this->index(index.row(),3).data().toString() == "outcome operation")
				return outcome_color;
			else
				return income_color;
		}
		return QSqlQueryModel::data(index,role);
	}
};

#endif
