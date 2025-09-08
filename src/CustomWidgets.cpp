#include "CustomWidgets.hpp"

void ResizeableLabel::set_original_pixmap(const QPixmap & pixmap)
{
	this->original_pixmap = pixmap.copy();
}

void ResizeableLabel::resizeEvent(QResizeEvent *event)
{
	Q_ASSERT(!this->original_pixmap.isNull());

	setPixmap(this->original_pixmap.scaledToWidth(width(), Qt::SmoothTransformation));
}
