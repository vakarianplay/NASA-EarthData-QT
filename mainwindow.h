#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <nasadatagrabber.h>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_browse_clicked();

    void on_pushButton_getdata_clicked();



    void on_lineEdit_url_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    NasaDataGrabber *nasaData;

    void initWindow();
    void readPsw();
    void clearSession();
    QString extractFileName(QString url);
};
#endif // MAINWINDOW_H
