#pragma once

#include <QLabel>

class ResizeableLabel : public QLabel
{
    Q_OBJECT

public:
    using QLabel::QLabel;

    void set_original_pixmap(const QPixmap & pixmap);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QPixmap original_pixmap;
};
