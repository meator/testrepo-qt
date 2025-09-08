#pragma once

#include <QLabel>

// https://stackoverflow.com/a/22618496
class ResizeableLabel : public QLabel
{
	Q_OBJECT

public:
	explicit ResizeableLabel(QWidget * parent = nullptr);

	void set_original_pixmap(const QPixmap & pixmap);

	[[nodiscard]] int heightForWidth(int width) const override;
	[[nodiscard]] QSize sizeHint() const override;

protected:
	void resizeEvent(QResizeEvent * event) override;

private:
	QPixmap original_pixmap;
};
