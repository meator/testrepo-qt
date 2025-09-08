#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "CustomWidgets.hpp"

ResizeableLabel::ResizeableLabel(QWidget * parent) : QLabel(parent)
{
	setMinimumSize(1, 1);
	setScaledContents(false);
}

void ResizeableLabel::set_original_pixmap(const QPixmap & pixmap)
{
	this->original_pixmap = pixmap.copy();
}

int ResizeableLabel::heightForWidth(int width) const
{
	if (this->original_pixmap.isNull())
		return height();
	return ((qreal)this->original_pixmap.height() * width) /
		   this->original_pixmap.width();
}

QSize ResizeableLabel::sizeHint() const
{
	int w = width();
	return {w, heightForWidth(w)};
}

void ResizeableLabel::resizeEvent(QResizeEvent * event)
{
	Q_ASSERT(!this->original_pixmap.isNull());

	setPixmap(
		this->original_pixmap.scaledToWidth(width(), Qt::SmoothTransformation));
}

MainWindow::MainWindow(QWidget * parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QPixmap pixmap(":/images/Qt_logostrap_CMYK.jpg", "JPG");
	Q_ASSERT(!pixmap.isNull());
	ui->image->setPixmap(pixmap);
	ui->image->set_original_pixmap(pixmap);
}

MainWindow::~MainWindow()
{
	delete ui;
}
