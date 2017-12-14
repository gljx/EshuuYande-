#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"QString"
#include"regular.h"
#include"QUrl"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString code;
    regular regulars;
    QString path;
    QString getwebcode(QString temp);
    void getEshuushuu(QString);

private:
    Ui::MainWindow *ui;
private slots:
    QString savepath();
    void hadclicked();
    void getEshuushuuImage();
    void getPixivImage();
    void getYandeImage();
    void aboutus();
};


#endif // MAINWINDOW_H
