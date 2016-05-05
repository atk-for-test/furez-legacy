#ifndef CMD_H
#define CMD_H

#include "QFuzzer.h"

class Cmd
{
private:
    QFuzzer *fuzzer;
    QString context;
    QString executer;
    bool isWindows;

    void normalizeRequest(QString * = nullptr, bool = true);

public:
    Cmd(QFuzzer *f = 0, QString = "shell_exec", bool = false);

    std::vector<std::string> listDir(QString = "");
    QString getFileContent(QString);
    void changeDir(QString = "");
    QString vim(QString,QString);
    QString raw(QString);

    //GETTERS
    QString getContext();

    //SETTERS
    void setContext(QString);
};

#endif // CMD_H