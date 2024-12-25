#ifndef IMAGEBTN_H
#define IMAGEBTN_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
class ImageBtn:public QLabel
{
    Q_OBJECT
public:
    explicit ImageBtn(QWidget *parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent *e);
signals:
    void imgbtnPressed();
};

#endif // IMAGEBTN_H
