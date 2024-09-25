#include "nasadatagrabber.h"

NasaDataGrabber::NasaDataGrabber(QString url_, QString username_, QString pass_, QString outPath_)
{
    QString cookieFile = QDir::homePath() + "/.urs_cookies";
    downloadFile(url_, outPath_, cookieFile);
}


void NasaDataGrabber::downloadFile(const QString& url, const QString& outputFileName, const QString& cookieFile) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        QFile file(outputFileName);
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Error opening output file";
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

void NasaDataGrabber::generateCookies()
{

}


