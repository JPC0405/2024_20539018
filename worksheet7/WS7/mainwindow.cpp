#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "optiondialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDialog>
#include <vtkSmartPointer.h>
#include <vtkrenderWindow.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkProperty.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeView->addAction(ui->actionItems_Options);
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->widget->setRenderWindow(renderWindow);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1., 0., 0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);

    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();






    connect( ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton );

    connect( ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClick );


    connect(this, &MainWindow::statusUpdateMessage,ui->statusbar, &QStatusBar::showMessage);

    this->partList = new ModelPartList("PartsList");

    ui->treeView->setModel(this->partList);

    ModelPart *rootItem = this->partList->getRootItem();

    for (int i =0; i<3; i++){
        QString name = QString("TopLevel %1").arg(1);
        QString visible("true");
        qint64 R(0);
        qint64 G(0);
        qint64 B(0);

        ModelPart *childItem = new ModelPart({name,visible,R,G,B});

        rootItem->appendChild(childItem);

        for (int j=0;j<5;j++){
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");
            qint64 R(0);
            qint64 G(0);
            qint64 B(0);



            ModelPart *childChildItem = new ModelPart({name , visible,R,G,B});

            childItem->appendChild(childChildItem);
        }
    }


}




MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton(){
    //QMessageBox msgBox;
    //msgBox.setText("Add button was clicked");
    //msgBox.exec();

    emit statusUpdateMessage( QString("Add button was clicked"), 0);
}

void MainWindow::handleTreeClick(){
    QModelIndex index = ui->treeView->currentIndex();

    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is: ")+text,0);
}

void MainWindow::on_actionOpen_File_triggered()
{
    emit statusUpdateMessage(QString("Open File action triggered"),0);

    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files(*.stl);;Text Files(*.txt)"));

    emit statusUpdateMessage(QString(fileName),0);
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());
    selectedPart->setName(fileName.section('/', -1));

}


void MainWindow::on_pushButton_2_clicked()
{
    OptionDialog dialog(this);

    // Get the selected item
    //QString name = selectedPart->data(0).toString();
    //bool visible = selectedPart->visible();
    //emit statusUpdateMessage(name,0);
    // Call set functions in dialog to update dialog to match selected item

    QModelIndex index = ui->treeView->currentIndex();

    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    QString name = selectedPart->data(0).toString();
    bool vis = selectedPart->data(1).toBool();
    qint64 R = selectedPart->getColourR();
    qint64 G = selectedPart->getColourG();
    qint64 B = selectedPart->getColourB();

    dialog.setVisibility(vis);
    dialog.set_name(name);
    dialog.set_R(R);
    dialog.set_G(G);
    dialog.set_B(B);

    if (dialog.exec() == QDialog::Accepted){
        emit statusUpdateMessage(QString("Dialog accepted"), 0);


        // use get functions in dialog to get users choice
        bool n_vis = dialog.getVisibility();
        QString n_name = dialog.get_name();
        unsigned char n_R = dialog.get_R();
        unsigned char n_G = dialog.get_G();
        unsigned char n_B = dialog.get_B();

        selectedPart->setVisible(n_vis);
        selectedPart->setName(n_name);
        selectedPart->setColour(n_R,n_G,n_B);
        // update the selected item
    }

    else{
        emit statusUpdateMessage(QString("Dialog rejected"),0);
    }
}



void MainWindow::on_actionItems_Options_triggered()
{
    emit statusUpdateMessage(QString("Test action selected"),0);
}

