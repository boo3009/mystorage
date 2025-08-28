#ifndef ITEMDIALOG_SENTRY_H
#define ITEMDIALOG_SENTRY_H

#include <QDialog>
#include <QSqlTableModel>
#include <QTableView>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlQuery>
#include <QDebug>

class ItemDialog : public QDialog {
  Q_OBJECT
public:
  explicit ItemDialog(QSqlTableModel *model, QTableView *view,int row=-1,
										  bool needCopy=false,QWidget *parent=0);
private:
  QSqlTableModel *ptr_itemsModel;
  QTableView *ptr_itemsView;
  QDataWidgetMapper *mapper;
  
  QVBoxLayout *mainLayout;
  QHBoxLayout *buttonsLayout;
  QLabel *label;
  QLineEdit *lineEdit;
  QPushButton *savePB;
  QPushButton *cancelPB;

  void setup_Widget();
  void setup_ModelandMapper();
signals:
  void signal_ready();
private slots:
  void slot_saveItem();
  void slot_cancelItem();
};

#endif
