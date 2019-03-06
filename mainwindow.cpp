#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QInputDialog>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>
#include <array>
#include <cmath>

QString fileName;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),

    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("File Name:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty())
        fileName = text + ".csv";
        //textLabel->setText(text);

    QFile file;
    file.setFileName(fileName);
    //QFile file(fileName);
    if (file.open(QIODevice::ReadWrite)) {
        qDebug() << "File opened!\n";
        QTextStream stream(&file);
        for (int i = 0; i < ui->tableWidget->columnCount() - 1; ++i){
            stream << ui->tableWidget->horizontalHeaderItem(i)->text() << ",";
        }

        stream << ui->tableWidget->horizontalHeaderItem(ui->tableWidget->columnCount() - 1)->text() << "\n";


    }

    file.close();

}

MainWindow::~MainWindow()
{
//    bool ok;
//    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
//                                         tr("Enter file name:"), QLineEdit::Normal,
//                                         QDir::home().dirName(), &ok);
//    if (ok && !text.isEmpty())
//        text = "test";

//    text = text + ".csv" ;

//    const QString fileName = "test4.csv";
//    QFile file;
//    file.setFileName(fileName);
//    //QFile file(fileName);
//    if (file.open(QIODevice::ReadWrite)) {
//        qDebug() << "File opened!\n";
//        QTextStream stream(&file);
//        for (int i = 0; i < ui->tableWidget->columnCount() - 1; ++i){
//            stream << ui->tableWidget->horizontalHeaderItem(i)->text() << ",";
//        }

//        stream << ui->tableWidget->horizontalHeaderItem(ui->tableWidget->columnCount() - 1)->text() << "\n";


//    }

//    file.close();


//    bool checker = QFile::copy(fileName, text);

//    if (!checker) {
//        QMessageBox::warning(this, "Rename Unsuccesful", "Fix");
//    }

//    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    const double pi = 3.141592653589793238463; //pi
    double i = 0.0;
    double RGate = 0.0;
    double VWork = 0.0;
    double VOut = 0.0;
    double r = 0.0; //radius
    double h = 0.0; //height
    double x = 0.0; //width
    double y = 0.0; //length
    double d = 0.0; //distance between units
    double Er = 0.0; //relative static permittivity
    const double E0 = 8.854e-12; //electric constant

    double numOfTubes = 0;				//number of nanotubes possible on one structure
    double SAofTube = 0.0;			//surface area of nanotube
    double SAofStructure = 0.0;		//surface area of entire structure
    double C = 0.0;					//capacitance
    double gap = 0.0;


    r = (ui->lineEdit_R->text()).toDouble() * pow(10, ui->rBox->text().toDouble());
    h = (ui->lineEdit_H->text()).toDouble() * pow(10, ui->hBox->text().toDouble());
    x = (ui->lineEdit_W->text()).toDouble() * pow(10, ui->wBox->text().toDouble());
    y = (ui->lineEdit_L->text()).toDouble() * pow(10, ui->lBox->text().toDouble());
    d = (ui->lineEdit_D->text()).toDouble() * pow(10, ui->dBox->text().toDouble());
    Er = (ui->lineEdit_Er->text()).toDouble();
    gap = (ui->lineEdit_G->text()).toDouble() * pow(10, ui->gBox->text().toDouble());

    i = (ui->lineEdit_i->text()).toDouble();
    RGate = (ui->lineEdit_RGate->text()).toDouble();
    VWork = (ui->lineEdit_VWork->text()).toDouble();
    VOut = (ui->lineEdit_VOut->text()).toDouble();

    if (ui->findVOut->isChecked()) {
        VOut = i * RGate + VWork;
        ui->lineEdit_VOut->setText(QString::number(VOut));
    }
    else if (ui->findI->isChecked()) {
        i = (VOut - VWork)/RGate;
        ui->lineEdit_i->setText(QString::number(i));
    }
    else{
        ui->lineEdit_i->setText("0");
        ui->lineEdit_VOut->setText("0");
    }





    //calculate output values

    //number of tubes:
    if (ui->checkBox->isChecked()) {
        numOfTubes = ((x - gap) / (gap + (2.0 * r))) * ((y - gap) / (gap + (2.0 * r)));
    }
    else {
        numOfTubes = floor((x - gap) / (gap + (2.0 * r))) * floor((y - gap) / (gap + (2.0 * r)));
    }


    //Surface area of one tube
    //		   area of top  +   area of rectangle
    SAofTube = pi*r*r       +   2*r*pi*h;

    //Surface area of structure = area of rectangular base + (number of tubes * (area of one tube - nanotube top area))

    ui->lineEdit_SAofB->setText(QString::number(x*y));

    SAofStructure = (x * y) - numOfTubes * pi*r*r + numOfTubes * SAofTube;

    //Capacitance
    C = (E0*Er * SAofStructure) / d;

    ui->lineEdit_SAofNT->setText(QString::number(SAofTube));
    ui->lineEdit_SAofS->setText(QString::number(SAofStructure));
    ui->lineEdit_NofNT->setText(QString::number(numOfTubes));
    ui->lineEdit_C->setText(QString::number(C));

    //Section 4: Electrochemistry calculations

//    double molOfCharge = 0;  These two are unneeded
//    double molOfMatter = 0;
    double n = (ui->molOfCharge->text().toDouble())/(ui->molOfMatter->text().toDouble());
    double moles = ui->moles->text().toDouble();
    double faradayConstant = ui->faradayConstant->text().toDouble();
    double electronsTransferred = ui->electronsTransferred->text().toDouble();
    double diffusionCoefficient = ui->diffusionCoefficient->text().toDouble();
    double concentration = ui->concentration->text().toDouble();
    double i0 = ui->i0->text().toDouble();
    double time = ui->time->text().toDouble();

    double arr[] = {n, moles, electronsTransferred, diffusionCoefficient, concentration, time, SAofTube};

    int index = -1;
    int count = 0;

    for (int k = 0; k < 7; ++k){
        if (((arr[k] < .00001) && (arr[k] > .00001)) || (arr[k] == NULL)) {
            index = k;
            count++;
        }
    }


    if (count <= 1){
        switch (index) {
            case 1:
                moles = 4*electronsTransferred*diffusionCoefficient*concentration*r*SAofTube*time/n;
                ui->moles->setText(QString::number(moles));
                break;

            case 2:
                electronsTransferred = n*moles/(4*diffusionCoefficient*concentration*r*SAofTube*time);
                ui->electronsTransferred->setText(QString::number(electronsTransferred));
                break;

            case 4:
                concentration = n*moles/(SAofTube*time*4*electronsTransferred*diffusionCoefficient*r);
                ui->concentration->setText(QString::number(concentration));
                break;

        }
    }







}

void MainWindow::on_pushButton_2_clicked()
{

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(ui->lineEdit_R->text() + "*10^" + ui->rBox->text() + " m"));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(ui->lineEdit_H->text() + "*10^" + ui->hBox->text() + " m"));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(ui->lineEdit_G->text() + "*10^" + ui->gBox->text() + " m"));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, new QTableWidgetItem(ui->lineEdit_NofNT->text()));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, new QTableWidgetItem(ui->lineEdit_SAofNT->text() + " m^2"));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, new QTableWidgetItem(ui->lineEdit_W->text() + "*10^" + ui->wBox->text() + " m"));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 6, new QTableWidgetItem(ui->lineEdit_L->text() + "*10^" + ui->lBox->text() + " m"));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 7, new QTableWidgetItem(ui->lineEdit_D->text() + "*10^" + ui->dBox->text() + " m"));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 8, new QTableWidgetItem(ui->lineEdit_SAofB->text() + " m^2"));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 9, new QTableWidgetItem(ui->lineEdit_SAofS->text() + " m^2"));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 10, new QTableWidgetItem(ui->lineEdit_E0->text()));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 11, new QTableWidgetItem(ui->lineEdit_Er->text()));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 12, new QTableWidgetItem(ui->lineEdit_C->text()));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 13, new QTableWidgetItem(ui->lineEdit_RGate->text()));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 14, new QTableWidgetItem(ui->lineEdit_VWork->text()));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 15, new QTableWidgetItem(ui->lineEdit_VOut->text()));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 16, new QTableWidgetItem(ui->lineEdit_i->text()));

    if (ui->checkBox->isChecked()) {
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 17, new QTableWidgetItem("Yes"));
    }
    else {
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 17, new QTableWidgetItem("No"));
    }


//    QString fileName = "test4.csv";
    QFile file(fileName);
    if (file.open(QIODevice::Append)) {
        QTextStream stream(&file);

        qDebug() << "Line written!\n";

        stream << ui->lineEdit_R->text() + "*10^" + ui->rBox->text() + " m" << ",";
        stream << ui->lineEdit_H->text() + "*10^" + ui->hBox->text() + " m" << ",";
        stream << ui->lineEdit_G->text() + "*10^" + ui->gBox->text() + " m" << ",";
        stream << ui->lineEdit_NofNT->text() << ",";
        stream << ui->lineEdit_SAofNT->text() + " m^2" << ",";
        stream << ui->lineEdit_W->text() + "*10^" + ui->wBox->text() + " m" << ",";
        stream << ui->lineEdit_L->text() + "*10^" + ui->lBox->text() + " m" << ",";
        stream << ui->lineEdit_D->text() + "*10^" + ui->dBox->text() + " m" << ",";
        stream << ui->lineEdit_SAofB->text() + " m^2" << ",";
        stream << ui->lineEdit_SAofS->text() + " m^2" << ",";
        stream << ui->lineEdit_E0->text()  << ",";
        stream << ui->lineEdit_Er->text() << ",";
        stream << ui->lineEdit_C->text() << ",";
        stream << ui->lineEdit_RGate->text() << ",";
        stream << ui->lineEdit_VWork->text() << ",";
        stream << ui->lineEdit_VOut->text() << ",";
        stream << ui->lineEdit_i->text() << ",";

        if (ui->checkBox->isChecked()) {

            stream << "Yes" << "\n";
        }
        else {

            stream << "No" << "\n";
        }

        file.close();


        }



}

void MainWindow::on_pushButton_3_clicked()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}
