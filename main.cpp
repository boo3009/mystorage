#include "mainWindow.h"

int main(int argc, char **argv)
{
  QApplication app(argc,argv);
  MainWindow mainwindow; 
	mainwindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	mainwindow.setStyleSheet("background-color: #F0F8FF");
  mainwindow.show();
  return app.exec(); 
}
