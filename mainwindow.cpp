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
    clearSession();
}

void MainWindow::initWindow()
{
    QPixmap pixmap = QPixmap(":/res/logo.png");
    ui->label_logo->setScaledContents(true);
    ui->label_logo->setPixmap(pixmap);

    QString homepath = QDir::homePath();
    ui->lineEdit_path->setText(homepath);
    readPsw();

}

void MainWindow::readPsw()
{
    QString filePath = QDir::homePath() + "/.netrc";
    QFile file(filePath);
        if (file.exists()) {
            if (file.open(QIODevice::ReadOnly)) {
                QTextStream in(&file);
                QString fileContent = in.readAll();
                file.close();
                QRegularExpression re(R"(machine\s+urs\.earthdata\.nasa\.gov\s+login\s+(\S+)\s+password\s+(\S+))");
                QRegularExpressionMatch match = re.match(fileContent);

                if (match.hasMatch()) {
                    ui->lineEdit_login->setText(match.captured(1));
                    ui->lineEdit_pass->setText(match.captured(2));
                }
            }
        }
}

void MainWindow::clearSession()
{
    if (!ui->checkBox_save->isChecked()){
        QString cookieFile = QDir::homePath() + "/.urs_cookies";
        QFile cfile(cookieFile);
        if (cfile.exists())
            cfile.remove();

        QString netFile = QDir::homePath() + "/.netrc";
        QFile nfile(netFile);
        if (nfile.exists())
            nfile.remove();
    }
}

void MainWindow::on_pushButton_browse_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(!directory.isEmpty()) {
            ui->lineEdit_path->setText(directory + "/");
        }
}


void MainWindow::on_pushButton_getdata_clicked()
{
    QString url = "https://cddis.nasa.gov/archive/gnss/products/2333/COD0OPSULT_20242670000_02D_05M_ORB.SP3.gz";
    QString outputFileName = ui->lineEdit_path->text() /*+ "/" + "COD0OPSULT_20242670000_02D_05M_ORB.SP3.gz"*/;
    QString cookieFile = QDir::homePath() + "/.urs_cookies";
    ui->lineEdit_path->setText(outputFileName);
    nasaData = new NasaDataGrabber(ui->lineEdit_url->text(), ui->lineEdit_login->text(), ui->lineEdit_pass->text(), ui->lineEdit_path->text());
}




