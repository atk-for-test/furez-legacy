#ifndef FILECREATEBOX_H
#define FILECREATEBOX_H

#include <QDialog>
#include <QFileSystemModel>

namespace Ui {
class FileCreateBox;
}

class FileCreateBox : public QDialog
{
    Q_OBJECT

public:
    explicit FileCreateBox(QWidget *parent = 0);
    ~FileCreateBox();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::FileCreateBox *ui;
    QFileSystemModel *dirModel;

};

#endif // FILECREATEBOX_H
