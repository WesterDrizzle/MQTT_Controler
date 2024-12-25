#include "imagebtn.h"
#include <QMouseEvent>
#include <QLabel>
ImageBtn::ImageBtn(QWidget *parent) : QLabel(parent)
{
}

void ImageBtn::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        // 触发clicked信号
        emit imgbtnPressed();
    }
    QLabel::mousePressEvent(e);
}
