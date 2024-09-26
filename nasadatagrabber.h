#ifndef NASADATAGRABBER_H
#define NASADATAGRABBER_H

#include <QDebug>
#include <curl/curl.h>
#include <cstdio>
#include <string>
#include <QFile>
#include <QDir>
#include <QProcess>

class NasaDataGrabber
{
public:
    NasaDataGrabber(QString url_, QString username_, QString pass_, QString outPath_);

private:
    CURL* curl;
    CURLcode res;

    void downloadFile(const QString& url, const QString& outputFileName, const QString& cookieFile);
    size_t WriteFileCallback(void* ptr, size_t size, size_t nmemb, FILE* stream);
    void generateCookies(const QString &user_, const QString &pass_);
    bool isCookiesFileExist();
    bool curlCall();
};

#endif // NASADATAGRABBER_H
