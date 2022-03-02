#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWidget>
#include <QPainter>
#include <vector>

class renderWindow: public QWidget{
    Q_OBJECT //para poder crear slots

public:
    QList<int> pointsList;
    renderWindow(QWidget *parent=0); //constructor
    void paintEvent(QPaintEvent *event) override;
    void drawCubeEdges(QPainter &points, QList<int> pointsList);
    void lineDDA(QPainter & points, double p1x, double p1y, double p2x, double p2y);

public slots:
    void setPenColor(QColor color);


private:
    QPen pointPen;



};



#endif // RENDERWINDOW_H
