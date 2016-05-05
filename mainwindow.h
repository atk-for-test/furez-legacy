#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStandardItem>
#include <QMainWindow>

#include <QAbstractButton>

#include "Cmd.h"
#include "QFuzzer.h"
#include "Parser.h"
#include "filedownloader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_save_clicked();
    void on_treeView_clicked(const QModelIndex &index);

    void on_show_clicked();
    void on_edit_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *rootPathModel;
    Cmd *cmd;
    QFuzzer *fuzzer;
    Parser *parser;

    QString context;
    QString elemName;
    QString header;
    QString url;

    void handleTreeView(std::vector<std::string>);
    void prepareBlock(std::vector<std::string>, QStandardItem *);

    QString getUrl();
    QString getHeader();
    QAbstractButton* getExecuter();
};

#endif // MAINWINDOW_H
