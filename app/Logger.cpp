#include "Logger.h"

#include "Fuzzer.h"

Logger::Logger(QObject *parent, Fuzzer *fuzzer, QTextEdit *loggerZone) : QObject(parent)
{
    this->fuzzer = fuzzer;
    this->loggerZone = loggerZone;

    connect(this->fuzzer, SIGNAL(finishRequest()), this, SLOT(logFuzzer()), Qt::UniqueConnection);
}

void Logger::logFuzzer()
{
    QString request, response;

    request = this->fuzzer->getRequest();
    response =this->fuzzer->getResponse();

    this->logInput(request);
    this->logInput(response);
}

void Logger::logInput(QString input) {
    QString formerContent, newContent;

    formerContent = this->loggerZone->toPlainText();
    newContent = formerContent + "\n" + input;

    this->loggerZone->setText(newContent);
}
