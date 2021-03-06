#include "Downloader.h"

#include "editorwindow.h"

Downloader::Downloader(QObject *parent, QString h, QString u, QString d, EditorWindow *e) :
    QObject(parent)
{
    manager = new QNetworkAccessManager;
    this->header = h;
    this->url = u;
    this->downloadDir = d;
    this->editorWindow = e;
}

Downloader::~Downloader()
{
    manager->deleteLater();
}

QString Downloader::downloadFile(QString saveFilePath, QString fileName, bool isEditQuery)
{
    QString request = "$file='" + saveFilePath + "';header('Content-Description: File Transfer');header('Content-Type: application/octet-stream');header('Content-Disposition: attachment');readfile ($file);";

    QNetworkRequest nRequest;
    nRequest.setUrl(QUrl(this->url));
    nRequest.setRawHeader(this->header.toLatin1(), request.toLatin1());
    reply = manager->get(nRequest);

    absolutePath = this->downloadDir + fileName;

    file = new QFile;
    file->setFileName(absolutePath);
    file->open(QIODevice::WriteOnly);

    connect(reply,SIGNAL(readyRead()),this, SLOT(onReadyRead()));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this, SLOT(onFinished(QNetworkReply*)));
    connect(reply,SIGNAL(finished()),this, SLOT(onReplyFinished()));

    if(isEditQuery) {
        connect(reply, SIGNAL(finished()), this, SLOT(hydrateContent()));
    }

    return absolutePath;
}

void Downloader::onFinished(QNetworkReply * reply)
{
    switch(reply->error())
    {
        case QNetworkReply::NoError:
        {
            qDebug("file is downloaded successfully.");
        }
            break;
        default:
        {
            qDebug(reply->errorString().toLatin1());
        };
    }
}

void Downloader::hydrateContent()
{
    QFile *fileOpen = new QFile;
    fileOpen->setFileName(absolutePath);
    fileOpen->open(QIODevice::ReadOnly);

    if(fileOpen->isOpen()) {
        QTextStream in(fileOpen);

        editorWindow->setContent(in.readAll());
        editorWindow->show();

        fileOpen->close();
        fileOpen->deleteLater();
    }
}

void Downloader::onReadyRead()
{
    file->write(reply->readAll());
}

void Downloader::onReplyFinished()
{
    if(file->isOpen())
    {
        file->close();
        file->deleteLater();
    }
}
