#include "QDownloader.h"

#include "editorwindow.h"

QDownloader::QDownloader(QObject *parent, QString h, QString u, QString d, EditorWindow *e) :
    QObject(parent)
{
    manager = new QNetworkAccessManager;
    this->header = h;
    this->url = u;
    this->downloadDir = d;
    this->editorWindow = e;
}

QDownloader::~QDownloader()
{
    manager->deleteLater();
}

QString QDownloader::setFile(QString saveFilePath, QString fileName)
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

    connect(reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(reply,SIGNAL(finished()),this,SLOT(onReplyFinished()));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));

    return absolutePath;
}

void QDownloader::onFinished(QNetworkReply * reply)
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

void QDownloader::hydrateContent()
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

void QDownloader::onReadyRead()
{
    file->write(reply->readAll());
}

void QDownloader::onReplyFinished()
{
    if(file->isOpen())
    {
        file->close();
        file->deleteLater();

        hydrateContent();
    }
}
