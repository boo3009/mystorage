#include "mainWindow.h"

int main(int argc, char **argv)
{
  QApplication app(argc,argv);
  MainWindow mainwindow; 
	mainwindow.setStyleSheet("background-color: #fbefcc");
  mainwindow.show();
  return app.exec(); 
}
