#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWidget>
#include <QPainter>

class renderWindow: public QWidget{
    Q_OBJECT //para poder crear slots

public:
    QList<int> pointsList;
    renderWindow(QWidget *parent=0); //constructor
    void paintEvent(QPaintEvent *event) override;



public slots:
    void setPenColor(QColor color);


private:
    QPen pointPen;



};



#endif // RENDERWINDOW_H
