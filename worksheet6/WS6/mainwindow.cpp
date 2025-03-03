#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //( ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton );

    this->partList = new ModelPartList("PartsList");

    ui->treeView->setModel(this->partList);

    ModelPart *rootItem = this->partList->getRootItem();

    for (int i =0; i<3; i++){
        QString name = QString("TopLevel %1").arg(1);
        QString visible("true");

        ModelPart *childItem = new ModelPart({name,visible});

        rootItem->appendChild(childItem);

        for (int j=0;j<5;j++){
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");

            ModelPart *childChildItem = new ModelPart({name , visible});

            childItem->appendChild(childChildItem);
        }
    }
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
