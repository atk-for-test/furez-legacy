#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QTextEdit>

#include <Fuzzer.h>

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = 0, Fuzzer *fuzzer = 0, QTextEdit *loggerZone = 0);

signals:

public slots:
    void logFuzzer();
    void logInput(QString);

private:
    Fuzzer *fuzzer;
    QTextEdit *loggerZone;
};

#endif // LOGGER_H
