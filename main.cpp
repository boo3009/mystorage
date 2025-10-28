#include "mainWindow.h"

int main(int argc, char **argv)
{
  QApplication app(argc,argv);
  MainWindow mainwindow; 
	mainwindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	mainwindow.setStyleSheet("background-color: #fbefcc");
  mainwindow.show();
  return app.exec(); 
}
