#include "nasadatagrabber.h"

NasaDataGrabber::NasaDataGrabber(QString url_, QString username_, QString pass_, QString outPath_)
{
    generateCookies(username_, pass_);
    QString cookieFile = QDir::homePath() + "/.urs_cookies";
    downloadFile(url_, outPath_, cookieFile);
}


void NasaDataGrabber::downloadFile(const QString& url, const QString& outputFileName, const QString& cookieFile) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        QFile file(outputFileName);
        if (!file.open(QIODevice::WriteOnly)) {
            return;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url.toStdString().c_str());

        // Set the cookie file for reading and storing cookies
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookieFile.toStdString().c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookieFile.toStdString().c_str());

        // Follow redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Set write data function to save data to the file
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](char* ptr, size_t size, size_t nmemb, void* userdata) -> size_t {
            QFile* file = static_cast<QFile*>(userdata);
            qint64 written = file->write(ptr, size * nmemb);
            return written == -1 ? 0 : static_cast<size_t>(written);
        });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            qDebug() << "curl_easy_perform() failed:" << curl_easy_strerror(res);
        } else {
            qDebug() << "File downloaded successfully: " << outputFileName;
        }
        curl_easy_cleanup(curl);
        file.close();
    }
    curl_global_cleanup();
}

void NasaDataGrabber::generateCookies(const QString& user_, const QString& pass_)
{
    QString netrcFilePath = QDir::homePath() + "/.netrc";
    QFile netrcFile(netrcFilePath);
    QString expectedContent = QString("machine urs.earthdata.nasa.gov login %1 password %2").arg(user_).arg(pass_);

    bool needsUpdate = false;

    if (netrcFile.exists()) {
        qDebug() << ".netrc exist";

    if (netrcFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&netrcFile);
        QString fileContent = in.readAll();
        netrcFile.close();

        if (fileContent != expectedContent) {
          qDebug() << ".netrc need update";
          needsUpdate = true;
          }
        } else {
            qDebug() << ".netrc don't exist";
            needsUpdate = true;
        }

        if (needsUpdate) {
            if (netrcFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                QTextStream out(&netrcFile);
                out << expectedContent;
                netrcFile.close();
            }

            if (isCookiesFileExist()) {
                QString cookieFile = QDir::homePath() + "/.urs_cookies";
                QFile file(cookieFile);
                file.remove();
            }
            if (curlCall())
                qDebug() << "cookies create";
        }
    }
}

bool NasaDataGrabber::isCookiesFileExist()
{
    bool existflag = false;
    QString cookieFile = QDir::homePath() + "/.urs_cookies";
    QFile file(cookieFile);
    if (!file.exists()) {
        existflag = false;
        qDebug() << "cookies don't exist";
            if (file.open(QIODevice::WriteOnly))
                file.close();
        } else
        existflag = true;

    return existflag;
}

bool NasaDataGrabber::curlCall()
{
    qDebug() << "curl call";
    bool result = false;
    QString command = "curl -b ~/.urs_cookies -c ~/.urs_cookies -L -n https://cddis.nasa.gov/archive/gnss/products/2333";
    QProcess process;

    process.start("bash", QStringList() << "-c" << command);
    process.waitForFinished(-1);

    QString output = process.readAllStandardOutput();
    QString error = process.readAllStandardError();

    if (!output.isEmpty())
        result = true;

     if (!error.isEmpty())
        result = false;

     return result;
}
