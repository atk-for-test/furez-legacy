#ifndef QFUZZER_H
#define QFUZZER_H

#include <QObject>
#include <QNetworkReply>

class QFuzzer : public QObject
{
    Q_OBJECT
public:
    explicit QFuzzer(QObject *parent = 0, QString h = "Nes", QString u = "http://localhost/hack/perl.php");
    QString sendRequest(QString);
    QString uploadFile(QString, QString, QString);
    QString getHeader();
    QString getResponse();

signals:

private:
    void normalizeRequest(QString *);
    QByteArray buildUploadString(QString, QString);
    QString header;
    QString url;
    QString response;
    bool error;

public slots:
    void handleReturn();
    void handleError(QNetworkReply::NetworkError);
    void httpRequestFinished(QNetworkReply *);
};

#endif // QFUZZER_H