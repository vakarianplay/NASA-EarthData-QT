#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initWindow();




//    nasaData = new NasaDataGrabber(url, "aaaa", "123", outputFileName);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initWindow()
{
    QPixmap pixmap = QPixmap(":/res/logo.png");
    ui->label_logo->setScaledContents(true);
    ui->label_logo->setPixmap(pixmap);

    QString homepath = QDir::homePath();
    ui->lineEdit_path->setText(homepath);
}

void MainWindow::readPsw()
{

}

void MainWindow::savePsw()
{

}

void MainWindow::on_pushButton_browse_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(!directory.isEmpty()) {
            ui->lineEdit_path->setText(directory);
        }
}


void MainWindow::on_pushButton_getdata_clicked()
{
    QString url = "https://cddis.nasa.gov/archive/gnss/products/2333/COD0OPSULT_20242670000_02D_05M_ORB.SP3.gz";
    QString outputFileName = ui->lineEdit_path->text() + "/" + "COD0OPSULT_20242670000_02D_05M_ORB.SP3.gz";
    QString cookieFile = QDir::homePath() + "/.urs_cookies";
    ui->lineEdit_path->setText(outputFileName);
    nasaData = new NasaDataGrabber(url, "aaaa", "123", outputFileName);

}

