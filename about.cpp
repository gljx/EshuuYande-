#include "about.h"


about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    connect(ui->okbutton,SIGNAL(clicked(bool)),this,SLOT(quit()));
}

about::~about()
{
    delete ui;
}
