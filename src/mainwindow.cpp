#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget * parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QPixmap pixmap(":/images/Qt_logostrap_CMYK.jpg", "JPG");
	Q_ASSERT(!pixmap.isNull());
	ui->image->setPixmap(pixmap);
}

MainWindow::~MainWindow()
{
	delete ui;
}
