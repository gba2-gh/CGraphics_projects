#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include<QPainter>
#include<QApplication>
#include<QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class renderWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void pickColor();
    void pickPosition();

private:
    Ui::MainWindow *ui;
    QPushButton *colorBtn;
    QLineEdit *pointLineEd;
    renderWindow *renderwindow;
};
#endif // MAINWINDOW_H
