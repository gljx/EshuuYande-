#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include"ui_about.h"

namespace Ui {
class about;
}

class about : public QDialog,Ui::about
{
    Q_OBJECT

public:
    explicit about(QWidget *parent = 0);
    ~about();

private:
    Ui::about *ui;
};

#endif // ABOUT_H
