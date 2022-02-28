#include "mainwindow.h"
#include "ui_mainwindow.h"

#include"renderWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setFixedSize(200,300);
    ui->setupUi(this);

    renderwindow = new renderWindow;

    colorBtn = new QPushButton(this);
    colorBtn->setText("Change Color");
    connect(colorBtn, SIGNAL(clicked()),this, SLOT(pickColor()));


    pointLineEd = new QLineEdit(this);
    pointLineEd->setPlaceholderText("Enter points");
    connect(pointLineEd, SIGNAL(returnPressed()), this, SLOT(pickPosition()));

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(renderwindow);
    mainLayout->addWidget(colorBtn);
    mainLayout->addWidget(pointLineEd);
    centralWidget()->setLayout(mainLayout);

    setWindowTitle(tr("Draw Points CG"));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::pickColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    renderwindow->setPenColor(color);
}

void MainWindow::pickPosition()
{
    QList<int> array;

    QString in= pointLineEd->text();

    QStringList inSplit = in.split(",");


    foreach(QString num, inSplit){
            array.append(num.toInt());
        }
    qDebug() << array;

    renderwindow->pointsList = array;
    update();

}


