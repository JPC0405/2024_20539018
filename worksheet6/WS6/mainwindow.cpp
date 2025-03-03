#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //( ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton );
    connect(this, &MainWindow::statusUpdateMessage,ui->statusbar, &QStatusBar::showMessage);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton(){
    /*QMessageBox msgBox;
    msgBox.setText("Add button was clicked");
    msgBox.exec();*/

    emit statusUpdateMessage( QString("Add button was clicked"),0);
}
