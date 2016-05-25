#ifndef FUZZER_H
#define FUZZER_H

#include <QObject>
#include <QNetworkReply>
#include <QProgressBar>

class Fuzzer : public QObject
{
    Q_OBJECT
public:
    explicit Fuzzer(QObject *parent = 0, QString h = "Nes", QString u = "http://localhost/hack/perl.php");
    void sendRequest(QString);
    QString uploadFile(QString, QString, QString);
    QString getResponse();
    QString getRequest();

signals:
    void finishRequest();

private:
    QString normalizeRequest(QString);
    QByteArray buildUploadString(QString, QString);

    QString header;
    QString url;
    QString response;
    QString request;
    bool error;
    QNetworkAccessManager *gManager;

private slots:
    void handleReturn();
    void handleError(QNetworkReply::NetworkError);
    void httpRequestFinished(QNetworkReply *);
};

#endif // FUZZER_H