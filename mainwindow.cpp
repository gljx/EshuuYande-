#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"QtNetwork/QNetworkAccessManager"
#include"QtNetwork/QNetworkReply"
#include"QFileDialog"
#include"QFileInfo"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pathaction,SIGNAL(triggered(bool)),this,SLOT(savepath()));
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(hadclicked()));
    connect(ui->actionEshuushuu,SIGNAL(triggered(bool)),this,SLOT(getEshuushuuImage()));
    connect(ui->actionPixiv,SIGNAL(triggered(bool)),this,SLOT(getPixivImage()));
    connect(ui->actionYande,SIGNAL(triggered(bool)),this,SLOT(getYandeImage()));
    connect(ui->actionabout,SIGNAL(triggered(bool)),this,SLOT(aboutus()));
    ui->spinBox->setRange(1,10);
    ui->spinBox->setValue(1);
    ui->lineEdit->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}
QString MainWindow::savepath()
{
    path=QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "/home",
                                                   QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
     qDebug()<<path<<endl;
     return path;
}
QString MainWindow::getwebcode(QString temp)
{
    //网页地址
    const QString URLSTR = temp;
    QUrl url(URLSTR);
    //https
    QNetworkRequest request;
    QSslConfiguration conf = request.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        conf.setProtocol(QSsl::TlsV1SslV3);
        request.setSslConfiguration(conf);
        request.setUrl(url);
    //储存网页代码的文件
    const QString FILE_NAME = "code.txt";


    QNetworkAccessManager manager;
    QEventLoop loop;
    qDebug() << "Reading code form " << URLSTR;
    //发出请求
    QNetworkReply *reply = manager.get(request);
    //请求结束并下载完成后，退出子事件循环
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //开启子事件循环
    loop.exec();

    //将读到的信息写入文件
    code = reply->readAll();

    QFile file(FILE_NAME);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << code << endl;
    file.close();
    qDebug() << "Finished, the code have written to " << FILE_NAME;
    return code;
}
void MainWindow::getEshuushuu(QString tempstr)
{
    qDebug()<<"getimg called"<<endl;
    QStringList imglist=regulars.searchimg(tempstr);
    qDebug()<<imglist<<endl;
    qDebug()<<imglist.size()<<endl;
    int i=0;
    QString temps;
    while(!imglist.isEmpty())
   {
       qDebug()<<"imglist"<<i<<"  is    "<<imglist[0]<<endl;
        imglist[0].replace("<a class=\"thumb_image\" href=\"","http://e-shuushuu.net");
        imglist[0].replace("\"","");
        temps=imglist.takeFirst();
   qDebug()<<temps<<endl;
   qDebug()<<"The download url is "<<temps;
    QUrl url(temps);
    QNetworkRequest requset;
    requset.setUrl(url);
    QNetworkAccessManager manager;
    qDebug() << "Download img form  " << url;
    //发出请求
    QEventLoop loop;
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QString temp;
    temp=path+"/"+url.fileName();
    qDebug()<<"url.fileName is :   "<<url.fileName()<<endl;
    qDebug()<<"temp path is :   "<<temp<<endl;
    QFile file(temp);
    file.open(QIODevice::Append);
    file.write(reply->readAll());
    file.close();
    qDebug()<<"img had download"<<endl;
    i++;
   }

}
void MainWindow::hadclicked()
{
    QString t("https://yande.re/post");
    t=t+"/show/"+ui->lineEdit->text();
    QUrl url(t);
    QNetworkRequest request;
    QSslConfiguration conf = request.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        conf.setProtocol(QSsl::TlsV1SslV3);
        request.setSslConfiguration(conf);

    QNetworkAccessManager manager;
    QString tempcode=getwebcode(t);
    QStringList imglist=regulars.search("<a class=\"original-file-changed\" id=\"highres\" href=\"(.*).(jpg|png)",tempcode);
    qDebug()<<imglist<<endl;
    if(!imglist.isEmpty())

    {
        imglist[0].replace("<a class=\"original-file-changed\" id=\"highres\" href=\"","");
        imglist[0].replace("\"","");
        url=imglist.takeFirst();
        request.setUrl(url);
        qDebug() << "Download img form  " << url;
        //发出请求
        QEventLoop loop;
        QNetworkReply *reply = manager.get(request);
      QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        QString temp;
        temp=path+"/"+url.fileName();
        qDebug()<<"url.fileName is :   "<<url.fileName()<<endl;
        qDebug()<<"temp path is :   "<<temp<<endl;
        QFile file(temp);
        file.open(QIODevice::Append);
        file.write(reply->readAll());
        file.close();
        qDebug()<<"img had download"<<endl;
   }
    else
        qDebug()<<"Fail"<<endl;
}
void MainWindow::getEshuushuuImage()
{
    QString next;
    QStringList nextlist;
    int page;
    page=ui->spinBox->value();
    int i;
    getwebcode("http://e-shuushuu.net/");
    getEshuushuu(code);
    qDebug()<<"next page ready"<<endl;
    nextlist=regulars.searchnext(code);
    qDebug()<<nextlist<<endl;
    if(!nextlist.isEmpty())
  {  next=nextlist.takeFirst();
     next.replace("<li class=\"next\"><a href=\"","http://e-shuushuu.net/");
     next.replace("\"","");
    for(i=2;i<=page;i++)
    {
        qDebug()<<next<<endl;
        getEshuushuu(getwebcode(next));
        nextlist=regulars.searchnext(next);
        if(!nextlist.isEmpty())
        {next=nextlist.takeFirst();
        next.replace("<li class=\"next\"><a href=\"","http://e-shuushuu.net");
        next.replace("\"","");
        }
    }
   }
}
void MainWindow::getPixivImage()
{


}
void MainWindow::getYandeImage()
{
    ui->lineEdit->show();
    ui->label->hide();
    ui->textBrowser->hide();


}
void MainWindow::aboutus()
{

}
